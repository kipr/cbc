/**
 * @file microdia-queue.c
 * @author Brian Johnson
 *
 * @brief Buffer management
 *
 * @note Original code from UVC webcam driver
 * @note Copyright (C) 2005-2008 Laurent Pinchart (laurent.pinchart@skynet.be)
 * @note Copyright (C) 2008 Brian Johnson (brijohn@gmail.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * ------------------------------------------------------------------------
 *
 * Video buffers queue management.
 *
 * Video queues is initialized by microdia_queue_init(). The function performs
 * basic initialization of the microdia_video_queue struct and never fails.
 *
 * Video buffer allocation and freeing are performed by microdia_alloc_buffers()
 * and microdia_free_buffers() respectively. The former acquires the video queue
 * lock, while the later must be called with the lock held (so that allocation
 * can free previously allocated buffers). Trying to free buffers that are
 * mapped to user space will return -EBUSY.
 *
 * Video buffers are managed using two queues. However, unlike most USB video
 * drivers which use an in queue and an out queue, we use a main queue which
 * holds all queued buffers (both 'empty' and 'done' buffers), and an irq
 * queue which holds empty buffers. This design (copied from video-buf)
 * minimizes locking in interrupt, as only one queue is shared between
 * interrupt and user contexts.
 *
 * Use cases
 * ---------
 *
 * Unless stated otherwise, all operations which modify the irq buffers queue
 * are protected by the irq spinlock.
 *
 * 1. The user queues the buffers, starts streaming and dequeues a buffer.
 *
 *    The buffers are added to the main and irq queues. Both operations are
 *    protected by the queue lock, and the latert is protected by the irq
 *    spinlock as well.
 *
 *    The completion handler fetches a buffer from the irq queue and fills it
 *    with video data. If no buffer is available (irq queue empty), the handler
 *    returns immediately.
 *
 *    When the buffer is full, the completion handler removes it from the irq
 *    queue, marks it as ready (MICRODIA_BUF_STATE_DONE) and wake its wait
 *    queue. At that point, any process waiting on the buffer will be woken up.
 *    If a process tries to dequeue a buffer after it has been marked ready,
 *    the dequeing will succeed immediately.
 *
 * 2. Buffers are queued, user is waiting on a buffer and the device gets
 *    disconnected.
 *
 *    When the device is disconnected, the kernel calls the completion handler
 *    with an appropriate status code. The handler marks all buffers in the
 *    irq queue as being erroneous (MICRODIA_BUF_STATE_ERROR) and wakes them up
 *    so that any process waiting on a buffer gets woken up.
 *
 *    Waking up up the first buffer on the irq list is not enough, as the
 *    process waiting on the buffer might restart the dequeue operation
 *    immediately.
 *
 */

#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/usb.h>
#include <linux/videodev.h>
#include <linux/vmalloc.h>
#include <linux/wait.h>
#include <linux/mm.h>
#include <asm/atomic.h>

#include "microdia.h"

/**
 * @param queue
 *
 */
void microdia_queue_init(struct microdia_video_queue *queue)
{
	mutex_init(&queue->mutex);
	spin_lock_init(&queue->irqlock);
	INIT_LIST_HEAD(&queue->mainqueue);
	INIT_LIST_HEAD(&queue->irqqueue);
}

/**
 * @brief Allocate the video buffers.
 *
 * @param queue
 * @param nbuffers
 * @param buflength
 *
 * Pages are reserved to make sure they will not be swaped, as they will be
 * filled in URB completion handler.
 *
 * Buffers will be individually mapped, so they must all be page aligned.
 */
int microdia_alloc_buffers(struct microdia_video_queue *queue,
	unsigned int nbuffers, unsigned int buflength)
{
	unsigned int bufsize = PAGE_ALIGN(buflength);
	unsigned int i;
	void *mem = NULL;
	void *buffer = NULL;
	int ret;

	mutex_lock(&queue->mutex);

	ret = microdia_free_buffers(queue);
	if (ret < 0)
		goto done;

	/* Bail out if no buffers should be allocated. */
	if (nbuffers == 0)
		goto done;

	if (nbuffers < queue->min_buffers)
		nbuffers = queue->min_buffers;
	else if (nbuffers > queue->max_buffers)
		nbuffers = queue->max_buffers;

	queue->scratch = vmalloc_32(bufsize);

	if (queue->scratch == NULL) {
		ret = -ENOMEM;
		goto done;
	}

	/* Decrement the number of buffers until allocation succeeds. */
	for (; nbuffers >= queue->min_buffers; --nbuffers) {
		mem = vmalloc_32(nbuffers * bufsize);
		buffer = kzalloc(nbuffers * sizeof(struct microdia_buffer),
			GFP_KERNEL);
		if (mem != NULL && buffer != NULL)
			break;
		if (mem != NULL)
			vfree(mem);
		if (buffer != NULL)
			kfree(buffer);
	}

	if (mem == NULL || buffer == NULL) {
		ret = -ENOMEM;
		vfree(queue->scratch);
		goto done;
	}
	queue->buffer = buffer;

	for (i = 0; i < nbuffers; ++i) {
		memset(&queue->buffer[i], 0, sizeof queue->buffer[i]);
		queue->buffer[i].buf.index = i;
		queue->buffer[i].buf.m.offset = i * bufsize;
		queue->buffer[i].buf.length = buflength;
		queue->buffer[i].buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		queue->buffer[i].buf.sequence = 0;
		queue->buffer[i].buf.field = V4L2_FIELD_NONE;
		queue->buffer[i].buf.memory = V4L2_MEMORY_MMAP;
		queue->buffer[i].buf.flags = 0;
		init_waitqueue_head(&queue->buffer[i].wait);
	}

	queue->mem = mem;
	queue->count = nbuffers;
	queue->buf_size = bufsize;
	ret = nbuffers;

done:
	mutex_unlock(&queue->mutex);
	return ret;
}

/**
 * @brief Free the video buffers.
 *
 * @param queue
 *
 * This function must be called with the queue lock held.
 */
int microdia_free_buffers(struct microdia_video_queue *queue)
{
	unsigned int i;
	UDIA_INFO("Freeing %d v4l2 buffers\n", queue->count);

	for (i = 0; i < queue->count; ++i) {
		if (queue->buffer[i].vma_use_count != 0)
			return -EBUSY;
	}

	if (queue->count) {
		vfree(queue->mem);
		vfree(queue->scratch);
		kfree(queue->buffer);
		INIT_LIST_HEAD(&queue->mainqueue);
		INIT_LIST_HEAD(&queue->irqqueue);
		queue->count = 0;
	}

	return 0;
}

/**
 * @param buf
 * @param v4l2_buf
 *
 */
static void __microdia_query_buffer(struct microdia_buffer *buf,
	struct v4l2_buffer *v4l2_buf)
{
	memcpy(v4l2_buf, &buf->buf, sizeof *v4l2_buf);

	if (buf->vma_use_count)
		v4l2_buf->flags |= V4L2_BUF_FLAG_MAPPED;

	switch (buf->state) {
	case MICRODIA_BUF_STATE_ERROR:
	case MICRODIA_BUF_STATE_DONE:
		v4l2_buf->flags |= V4L2_BUF_FLAG_DONE;
		break;
	case MICRODIA_BUF_STATE_QUEUED:
	case MICRODIA_BUF_STATE_ACTIVE:
		v4l2_buf->flags |= V4L2_BUF_FLAG_QUEUED;
		break;
	case MICRODIA_BUF_STATE_IDLE:
	default:
		break;
	}
}

/**
 * @param queue
 * @param v4l2_buf
 *
 * @return 0 or negative error code
 *
 */
int microdia_query_buffer(struct microdia_video_queue *queue,
	struct v4l2_buffer *v4l2_buf)
{
	int ret = 0;

	mutex_lock(&queue->mutex);
	if (v4l2_buf->index >= queue->count) {
		ret = -EINVAL;
		goto done;
	}

	__microdia_query_buffer(&queue->buffer[v4l2_buf->index], v4l2_buf);

done:
       mutex_unlock(&queue->mutex);
       return ret;
}

/**
 * @brief Queue a video buffer.
 *
 * @param queue
 * @param v4l2_buf
 *
 * @return 0 or negative error code
 *
 * Attempting to queue a buffer that has already been
 * queued will return -EINVAL.
 */
int microdia_queue_buffer(struct microdia_video_queue *queue,
	struct v4l2_buffer *v4l2_buf)
{
	struct microdia_buffer *buf;
	unsigned long flags;
	int ret = 0;

	UDIA_DEBUG("Queuing buffer %u.\n", v4l2_buf->index);

	if (v4l2_buf->type != V4L2_BUF_TYPE_VIDEO_CAPTURE ||
	    v4l2_buf->memory != V4L2_MEMORY_MMAP) {
		UDIA_ERROR("[E] Invalid buffer type (%u) "
			"and/or memory (%u).\n", v4l2_buf->type,
			v4l2_buf->memory);
		return -EINVAL;
	}

	mutex_lock(&queue->mutex);
	if (v4l2_buf->index >= queue->count)  {
		UDIA_ERROR("[E] Out of range index.\n");
		ret = -EINVAL;
		goto done;
	}

	buf = &queue->buffer[v4l2_buf->index];
	if (buf->state != MICRODIA_BUF_STATE_IDLE) {
		UDIA_ERROR("[E] Invalid buffer state "
			"(%u).\n", buf->state);
		ret = -EINVAL;
		goto done;
	}

	buf->state = MICRODIA_BUF_STATE_QUEUED;
	buf->buf.bytesused = 0;
	list_add_tail(&buf->stream, &queue->mainqueue);
	spin_lock_irqsave(&queue->irqlock, flags);
	list_add_tail(&buf->queue, &queue->irqqueue);
	spin_unlock_irqrestore(&queue->irqlock, flags);

done:
	mutex_unlock(&queue->mutex);
	return ret;
}

/**
 * @param buf
 * @param nonblocking
 *
 */
static int microdia_queue_waiton(struct microdia_buffer *buf, int nonblocking)
{
	if (nonblocking) {
		return (buf->state != MICRODIA_BUF_STATE_QUEUED &&
			buf->state != MICRODIA_BUF_STATE_ACTIVE)
			? 0 : -EAGAIN;
	}

	return wait_event_interruptible(buf->wait,
		buf->state != MICRODIA_BUF_STATE_QUEUED &&
		buf->state != MICRODIA_BUF_STATE_ACTIVE);
}

/**
 * @brief Dequeue a video buffer.
 *
 * @param queue
 * @param v4l2_buf
 * @param nonblocking
 *
 * @return 0 or negative error code
 *
 * If nonblocking is false, block until a buffer is
 * available.
 */
int microdia_dequeue_buffer(struct microdia_video_queue *queue,
	struct v4l2_buffer *v4l2_buf, int nonblocking)
{
	struct microdia_buffer *buf;
	int ret = 0;

	if (v4l2_buf->type != V4L2_BUF_TYPE_VIDEO_CAPTURE ||
	    v4l2_buf->memory != V4L2_MEMORY_MMAP) {
		UDIA_ERROR("[E] Invalid buffer type (%u) "
			"and/or memory (%u).\n", v4l2_buf->type,
			v4l2_buf->memory);
		return -EINVAL;
	}

	mutex_lock(&queue->mutex);
	if (list_empty(&queue->mainqueue)) {
		UDIA_ERROR("[E] Empty buffer queue.\n");
		ret = -EINVAL;
		goto done;
	}

	buf = list_first_entry(&queue->mainqueue, struct microdia_buffer,
			       stream);

	ret = microdia_queue_waiton(buf, nonblocking);
	if (ret < 0)
		goto done;

	UDIA_DEBUG("Dequeuing buffer %u (%u, %u bytes).\n",
		buf->buf.index, buf->state, buf->buf.bytesused);

	switch (buf->state) {
	case MICRODIA_BUF_STATE_ERROR:
		UDIA_WARNING("[W] Corrupted data (transmission error).\n");
		ret = -EIO;
	case MICRODIA_BUF_STATE_DONE:
		buf->state = MICRODIA_BUF_STATE_IDLE;
		break;

	case MICRODIA_BUF_STATE_IDLE:
	case MICRODIA_BUF_STATE_QUEUED:
	case MICRODIA_BUF_STATE_ACTIVE:
	default:
		UDIA_ERROR("[E] Invalid buffer state %u "
			"(driver bug?).\n", buf->state);
		ret = -EINVAL;
		goto done;
	}

	list_del(&buf->stream);
	__microdia_query_buffer(buf, v4l2_buf);

done:
	mutex_unlock(&queue->mutex);
	return ret;
}

/**
 * @brief Poll the video queue.
 *
 * @param queue
 * @param file
 * @param wait
 *
 * This function implements video queue polling and is intended to be used by
 * the device poll handler.
 */
unsigned int microdia_queue_poll(struct microdia_video_queue *queue,
	struct file *file, poll_table *wait)
{
	struct microdia_buffer *buf;
	unsigned int mask = 0;

	mutex_lock(&queue->mutex);
	if (list_empty(&queue->mainqueue)) {
		mask |= POLLERR;
		goto done;
	}
	buf = list_first_entry(&queue->mainqueue, struct microdia_buffer,
			       stream);

	poll_wait(file, &buf->wait, wait);
	if (buf->state == MICRODIA_BUF_STATE_DONE ||
	    buf->state == MICRODIA_BUF_STATE_ERROR)
		mask |= POLLIN | POLLRDNORM;

done:
	mutex_unlock(&queue->mutex);
	return mask;
}

/**
 * @brief Enable or disable the video buffers queue.
 *
 * @param queue
 * @param enable
 *
 * @return 0 or negative error code
 *
 * The queue must be enabled before starting video acquisition and must be
 * disabled after stopping it. This ensures that the video buffers queue
 * state can be properly initialized before buffers are accessed from the
 * interrupt handler.
 *
 * Enabling the video queue initializes parameters (such as sequence number,
 * sync pattern, ...). If the queue is already enabled, return -EBUSY.
 *
 * Disabling the video queue cancels the queue and removes all buffers from
 * the main queue.
 *
 * This function can't be called from interrupt context. Use
 * microdia_queue_cancel() instead.
 */
int microdia_queue_enable(struct microdia_video_queue *queue, int enable)
{
	unsigned int i;
	int ret = 0;

	mutex_lock(&queue->mutex);
	if (enable) {
		if (queue->streaming) {
			ret = -EBUSY;
			goto done;
		}
		queue->sequence = 0;
		queue->streaming = 1;
	} else {
		microdia_queue_cancel(queue);
		INIT_LIST_HEAD(&queue->mainqueue);

		for (i = 0; i < queue->count; ++i)
			queue->buffer[i].state = MICRODIA_BUF_STATE_IDLE;

		queue->streaming = 0;
	}

done:
	mutex_unlock(&queue->mutex);
	return ret;
}

/**
 * @brief Cancel the video buffers queue.
 *
 * @param queue
 *
 * Cancelling the queue marks all buffers on the irq queue as erroneous,
 * wakes them up and remove them from the queue.
 *
 * This function acquires the irq spinlock and can be called from interrupt
 * context.
 */
void microdia_queue_cancel(struct microdia_video_queue *queue)
{
	struct microdia_buffer *buf;
	unsigned long flags;

	spin_lock_irqsave(&queue->irqlock, flags);
	while (!list_empty(&queue->irqqueue)) {
		buf = list_first_entry(&queue->irqqueue, struct microdia_buffer,
				       queue);
		list_del(&buf->queue);
		buf->state = MICRODIA_BUF_STATE_ERROR;
		wake_up(&buf->wait);
	}
	spin_unlock_irqrestore(&queue->irqlock, flags);
}

/**
 * @param queue
 * @param buf
 *
 */
struct microdia_buffer *microdia_queue_next_buffer(
	struct microdia_video_queue *queue,
	struct microdia_buffer *buf)
{
	struct microdia_buffer *nextbuf;
	unsigned long flags;

	if (queue->drop_incomplete && queue->frame_size != buf->buf.bytesused) {
		buf->state = MICRODIA_BUF_STATE_QUEUED;
		buf->buf.bytesused = 0;
		return buf;
	}

	spin_lock_irqsave(&queue->irqlock, flags);
	list_del(&buf->queue);
	if (!list_empty(&queue->irqqueue))
		nextbuf = list_first_entry(&queue->irqqueue,
					   struct microdia_buffer, queue);
	else
		nextbuf = NULL;
	spin_unlock_irqrestore(&queue->irqlock, flags);

	buf->buf.sequence = queue->sequence++;
	do_gettimeofday(&buf->buf.timestamp);

	wake_up(&buf->wait);
	return nextbuf;
}
