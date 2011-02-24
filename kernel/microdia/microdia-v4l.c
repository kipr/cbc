/**
 * @file microdia-v4l.c
 * @author Nicolas VIVIEN
 * @date 2008-02-01
 *
 * @brief V4L2 interface and functions
 *
 * @note Copyright (C) Nicolas VIVIEN
 *
 * @par Licences
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/kref.h>
#include <linux/vmalloc.h>
#include <linux/usb.h>
#include <linux/mm.h>
#include <media/v4l2-common.h>


#include "microdia.h"
#include "sn9c20x.h"


/* USER DEFINED V4L2-CONTROLS: */
#define V4L2_CID_SHARPNESS		(V4L2_CID_PRIVATE_BASE + 0)
#define V4L2_CID_AUTOEXPOSURE		(V4L2_CID_PRIVATE_BASE + 1)

static struct file_operations v4l_microdia_fops;

/**
 * format list
 */

#define NUM_V4L2_FORMATS 5

struct v4l2_pix_format microdia_fmts[] = {
	{
		.width = 640,
		.height = 480,
		.pixelformat = V4L2_PIX_FMT_YUV420,
		.field = V4L2_FIELD_NONE,
		.bytesperline = 960,
		.sizeimage = 460800,
		.colorspace = V4L2_COLORSPACE_SRGB,
		.priv = 0
	},
	{
		.width = 640,
		.height = 480,
		.pixelformat = V4L2_PIX_FMT_BGR24,
		.field = V4L2_FIELD_NONE,
		.bytesperline = 1920,
		.sizeimage = 921600,
		.colorspace = V4L2_COLORSPACE_SRGB,
		.priv = 0
	},
	{
		.width = 640,
		.height = 480,
		.pixelformat = V4L2_PIX_FMT_RGB24,
		.field = V4L2_FIELD_NONE,
		.bytesperline = 1920,
		.sizeimage = 921600,
		.colorspace = V4L2_COLORSPACE_SRGB,
		.priv = 0
	},
	{
		.width = 640,
		.height = 480,
		.pixelformat = V4L2_PIX_FMT_YUYV,
		.field = V4L2_FIELD_NONE,
		.bytesperline = 1280,
		.sizeimage = 614400,
		.colorspace = V4L2_COLORSPACE_SRGB,
		.priv = 0
	},
	{
		.width = 640,
		.height = 480,
		.pixelformat = V4L2_PIX_FMT_JPEG,
		.field = V4L2_FIELD_NONE,
		.bytesperline = 1280,
		.sizeimage = 614400,
		.colorspace = V4L2_COLORSPACE_SRGB,
		.priv = 0
	},
};

/**
 * @param dev
 * @param fmt v4l2 fmt id
 *
 * @returns index of requested format
 *
 * @brief check if v4l2 forat is supported by device.
 *
 * This function permits to check if v4l2 format is supported.
 */
int v4l2_format_supported(struct usb_microdia *dev, __u32 fmt)
{
	int i;
	for (i = 0; i < sizeof(dev->supported_fmts) * 8; i++) {
		if (i > (ARRAY_SIZE(microdia_fmts) - 1))
			break;
		if (dev->supported_fmts & (1 << i) &&
		    microdia_fmts[i].pixelformat == fmt)
			return i;
	}
	return -EINVAL;
}

/**
 * @param dev
 * @param index format index
 *
 * @returns reference to format structure
 *
 * @brief enumerate supported formats
 *
 * This function will enumerate all supported formats.
 */
struct v4l2_pix_format *v4l2_enum_supported_formats(struct usb_microdia *dev, __u8 index)
{
	int i, bit = 0;
	for (i = 0; i < sizeof(dev->supported_fmts) * 8; i++) {
		if (i > (ARRAY_SIZE(microdia_fmts) - 1))
			break;
		if (dev->supported_fmts & (1 << i)) {
                    if (bit == index){
                                UDIA_INFO("Pixel format index %d\n",i);
				return &microdia_fmts[i];
                            }
			bit++;
		}
	}
	return NULL;
}

/**
 * @var microdia_controls
 *   List of all V4Lv2 controls supported by the driver
 */
static struct v4l2_queryctrl microdia_controls[] = {
	{
		.id      = V4L2_CID_BRIGHTNESS,
		.type    = V4L2_CTRL_TYPE_INTEGER,
		.name    = "Brightness",
		.minimum = 0,
		.maximum = 0xff00,
		.step    = 1,
		.default_value = 0x7f00,
	},
	{
		.id      = V4L2_CID_WHITENESS,
		.type    = V4L2_CTRL_TYPE_INTEGER,
		.name    = "Whiteness",
		.minimum = 0,
		.maximum = 0xff00,
		.step    = 1,
		.default_value = 0x7f00,
	},
/*
	{
		.id      = V4L2_CID_SATURATION,
		.type    = V4L2_CTRL_TYPE_INTEGER,
		.name    = "Saturation",
		.minimum = 0,
		.maximum = 0xff00,
		.step    = 1,
		.default_value = 0x7f00,
	},
*/
	{
		.id      = V4L2_CID_CONTRAST,
		.type    = V4L2_CTRL_TYPE_INTEGER,
		.name    = "Contrast",
		.minimum = 0,
		.maximum = 0xff00,
		.step    = 1,
		.default_value = 0x0000,
	},
	{
		.id	 = V4L2_CID_EXPOSURE,
		.type	 = V4L2_CTRL_TYPE_INTEGER,
		.name	 = "Exposure",
		.minimum = 0,
		.maximum = 0xff00,
		.step	 = 1,
		.default_value = 0x1000,
	},
	{
		.id	 = V4L2_CID_HFLIP,
		.type	 = V4L2_CTRL_TYPE_BOOLEAN,
		.name	 = "Horizontal flip",
		.minimum = 0,
		.maximum = 1,
		.step	 = 1,
		.default_value = 0,
	},
	{
		.id	 = V4L2_CID_VFLIP,
		.type	 = V4L2_CTRL_TYPE_BOOLEAN,
		.name	 = "Vertical flip",
		.minimum = 0,
		.maximum = 1,
		.step	 = 1,
		.default_value = 0,
	},
	{
		.id	 = V4L2_CID_SHARPNESS,
		.type	 = V4L2_CTRL_TYPE_INTEGER,
		.name	 = "Sharpness",
		.minimum = 0,
		.maximum = 0x3f,
		.step	 = 1,
		.default_value = 0x1f,
	},
	{
		.id	 = V4L2_CID_RED_BALANCE,
		.type	 = V4L2_CTRL_TYPE_INTEGER,
		.name	 = "Red Balance",
		.minimum = 0,
		.maximum = 0x7f,
		.step	 = 1,
		.default_value = 0x20,
	},
	{
		.id	 = V4L2_CID_BLUE_BALANCE,
		.type	 = V4L2_CTRL_TYPE_INTEGER,
		.name	 = "Blue Balance",
		.minimum = 0,
		.maximum = 0x7f,
		.step	 = 1,
		.default_value = 0x20,
	},
	{
		.id	 = V4L2_CID_AUTOEXPOSURE,
		.type	 = V4L2_CTRL_TYPE_BOOLEAN,
		.name	 = "Automatic exposure control",
		.minimum = 0,
		.maximum = 1,
		.step	 = 1,
		.default_value = 0,
	},
	{
		.id	 = V4L2_CID_AUTO_WHITE_BALANCE,
		.type	 = V4L2_CTRL_TYPE_BOOLEAN,
		.name	 = "Automatic whitbalance control",
		.minimum = 0,
		.maximum = 1,
		.step	 = 1,
		.default_value = 0,
	},
};

/**
 * @brief Get V4L privileges
 *
 * @param file
 *
 * @return 0 or negative error code
 *
 */
int v4l_get_privileges(struct file *file)
{
	struct usb_microdia *dev;
	int ret = 0;

	dev = video_get_drvdata(video_devdata(file));

	if (dev->owner == file)
		return 0;

	mutex_lock(&open_lock);
	if (dev->owner != NULL) {
		ret = -EBUSY;
		goto done;
	}
	dev->owner = file;
done:
	mutex_unlock(&open_lock);
	return ret;
}

/**
 * @brief Check whether there are V4L privileges
 *
 * @param file
 *
 * @return 0 or 1
 *
 */
int v4l_has_privileges(struct file *file)
{
	struct usb_microdia *dev;
	int ret = 0;

	dev = video_get_drvdata(video_devdata(file));

	if (dev->owner == file)
		ret = 1;

	return ret;
}

/**
 * @brief Drop V4L privileges
 *
 * @param file
 *
 */
void v4l_drop_privileges(struct file *file)
{
	struct usb_microdia *dev;

	dev = video_get_drvdata(video_devdata(file));

	if (dev->owner == file)
		dev->owner = NULL;
}

/**
 * @brief Enable video stream
 *
 * @param dev Pointer to device structure
 * @param mode Mode for video stream
 *
 * @returns 0 or negative error value
 *
 */
int v4l2_enable_video(struct usb_microdia *dev, int mode)
{
	int ret;

        if (mode == MICRODIA_MODE_IDLE)
        {
		dev_microdia_stop_stream(dev);
		usb_microdia_uninit_urbs(dev);
		microdia_queue_enable(&dev->queue, 0);
		dev->mode = mode;
		return 0;
	}

	if (dev->mode != MICRODIA_MODE_IDLE)
		return -EBUSY;

	if (microdia_queue_enable(&dev->queue, 1) < 0)
		return -EBUSY;

	ret = usb_microdia_init_urbs(dev);

	if (ret)
		return ret;

	dev_microdia_start_stream(dev);
	dev_microdia_camera_settings(dev);
	dev->mode = mode;

	return 0;
}

/**
 * @param inode Pointer on an inode
 * @param fp File pointer
 *
 * @returns 0 if all is OK
 *
 * @brief Open the video device
 *
 * This function permits to open a video device (/dev/videoX)
 */
static int v4l_microdia_open(struct inode *inode, struct file *fp)
{
	int ret = 0;

	struct usb_microdia *dev;
	struct video_device *vdev;

	mutex_lock(&open_lock);

	vdev = video_devdata(fp);
	dev = video_get_drvdata(video_devdata(fp));

	fp->private_data = vdev;

	kref_get(&dev->vopen);

	mutex_unlock(&open_lock);
	return ret;
}


/**
 * @param inode Pointer on inode
 * @param fp File pointer
 *
 * @returns 0 if all is OK
 *
 * @brief Release an opened file.
 *
 * This function permits to release an opened file with the 'open' method.
 */
static int v4l_microdia_release(struct inode *inode, struct file *fp)
{
	struct usb_microdia *dev;
	struct video_device *vdev;

	mutex_lock(&open_lock);

	vdev = video_devdata(fp);
	dev = video_get_drvdata(video_devdata(fp));

	if (v4l_has_privileges(fp)) {
		v4l2_enable_video(dev, MICRODIA_MODE_IDLE);

		mutex_lock(&dev->queue.mutex);
		microdia_free_buffers(&dev->queue);
		mutex_unlock(&dev->queue.mutex);
	}

	v4l_drop_privileges(fp);

	kref_put(&dev->vopen, usb_microdia_delete);

	mutex_unlock(&open_lock);
	return 0;
}


/**
 * @param fp File pointer
 *
 * @retval buf Buffer in user space
 * @retval count
 * @retval f_pos
 *
 * @returns Count value
 *
 * @brief Read the video device
 *
 * This function is called by the application is reading the video device.
 */
static ssize_t v4l_microdia_read(struct file *fp, char __user *buf, size_t count, loff_t *f_pos)
{
	int i, ret;
	int nbuffers;
	struct v4l2_buffer buffer;
	struct usb_microdia *dev;

	dev = video_get_drvdata(video_devdata(fp));

	ret = v4l_get_privileges(fp);
	if (ret < 0)
		return ret;

	if (dev->mode != MICRODIA_MODE_IDLE && dev->mode != MICRODIA_MODE_READ)
		return -EBUSY;

	buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buffer.memory = V4L2_MEMORY_MMAP;
	if (dev->mode == MICRODIA_MODE_IDLE) {
                nbuffers = microdia_alloc_buffers(&dev->queue, 2, MICRODIA_FRAME_SIZE);
		if (nbuffers < 0)
			return nbuffers;

		for (i = 0; i < nbuffers; i++) {
			buffer = dev->queue.buffer[i].buf;
			microdia_queue_buffer(&dev->queue, &buffer);
		}

		ret = v4l2_enable_video(dev, MICRODIA_MODE_READ);
		if (ret < 0)
			return ret;
	}

	if (dev->queue.read_buffer == NULL) {
                ret = microdia_dequeue_buffer(&dev->queue, &buffer, fp->f_flags & O_NONBLOCK);
		if (ret < 0)
			return ret;

		microdia_decompress(dev, &buffer);
		dev->queue.read_buffer = &dev->queue.buffer[buffer.index];
        }
        else {
		buffer = dev->queue.read_buffer->buf;
	}

	count = min((size_t)(buffer.bytesused - *f_pos), count);
	if (copy_to_user(buf, dev->queue.mem + buffer.m.offset + *f_pos, count))
		return -EFAULT;

	*f_pos += count;
	if (*f_pos >= buffer.bytesused) {
		dev->queue.read_buffer = NULL;
		microdia_queue_buffer(&dev->queue, &buffer);
		*f_pos = 0;
	}
	return count;
}


/**
 * @param fp File pointer
 * @param wait
 *
 * @returns 0 if all is OK
 *
 * @brief Polling function
 */
static unsigned int v4l_microdia_poll(struct file *fp, poll_table *wait)
{
	struct usb_microdia *dev;
	struct video_device *vdev;

	vdev = video_devdata(fp);
	dev = video_get_drvdata(video_devdata(fp));

	UDIA_STREAM("Poll\n");

	if (vdev == NULL || dev == NULL)
		return -EFAULT;

	return microdia_queue_poll(&dev->queue, fp, wait);
}

/**
 * @param vma
 *
 */
static void microdia_vm_open(struct vm_area_struct *vma)
{
	struct microdia_buffer *buffer = vma->vm_private_data;
	buffer->vma_use_count++;
}

/**
 * @param vma
 *
 */
static void microdia_vm_close(struct vm_area_struct *vma)
{
	struct microdia_buffer *buffer = vma->vm_private_data;
	buffer->vma_use_count--;
}

struct vm_operations_struct microdia_vm_ops = {
	.open	= microdia_vm_open,
	.close	= microdia_vm_close
};

/**
 * @param fp File pointer
 * @param vma VMA structure
 *
 * @returns 0 if all is OK
 *
 * @brief Memory map
 *
 * This function permits to map a memory space.
 */
static int v4l_microdia_mmap(struct file *fp, struct vm_area_struct *vma)
{
	struct page *page;
	unsigned long addr, start, size;
	unsigned int i;
	int ret = 0;

	struct usb_microdia *dev;
	struct video_device *vdev;
	struct microdia_buffer *buffer = NULL;

	vdev = video_devdata(fp);
	dev = video_get_drvdata(video_devdata(fp));

	UDIA_STREAM("mmap\n");

	start = vma->vm_start;
	size = vma->vm_end - vma->vm_start;

	mutex_lock(&dev->queue.mutex);

	for (i = 0; i < dev->queue.count; ++i) {
		buffer = &dev->queue.buffer[i];
		if ((buffer->buf.m.offset >> PAGE_SHIFT) == vma->vm_pgoff)
			break;
	}

	if (i == dev->queue.count || size != dev->queue.buf_size) {
		ret = -EINVAL;
		goto done;
	}

	vma->vm_flags |= VM_IO;

	addr = (unsigned long)dev->queue.mem + buffer->buf.m.offset;
	while (size > 0) {
		page = vmalloc_to_page((void *)addr);
		ret = vm_insert_page(vma, start, page);
		if (ret < 0)
			goto done;

		start += PAGE_SIZE;
		addr += PAGE_SIZE;
		size -= PAGE_SIZE;
	}

	vma->vm_ops = &microdia_vm_ops;
	vma->vm_private_data = buffer;
	microdia_vm_open(vma);
done:
	mutex_unlock(&dev->queue.mutex);
	return ret;
}

/**
 * @param file
 * @param priv
 * @param cap
 *
 * @return 0
 *
 */
int microdia_vidioc_querycap(struct file *file, void *priv, struct v4l2_capability *cap)
{
	struct usb_microdia *dev;

	dev = video_get_drvdata(priv);

	UDIA_DEBUG("VIDIOC_QUERYCAP\n");

	strlcpy(cap->driver, "microdia", sizeof(cap->driver));
	cap->capabilities = V4L2_CAP_VIDEO_CAPTURE | V4L2_CAP_STREAMING
		| V4L2_CAP_READWRITE;
	cap->version = (__u32) DRIVER_VERSION_NUM,
		strlcpy(cap->card, dev->vdev->name, sizeof(cap->card));

	if (usb_make_path(dev->udev, cap->bus_info, sizeof(cap->bus_info)) < 0)
		strlcpy(cap->bus_info, dev->vdev->name, sizeof(cap->bus_info));
	return 0;
}

/**
 * @param file
 * @param priv
 * @param input
 *
 * @return 0 or negative error code
 *
 */
int microdia_vidioc_enum_input(struct file *file, void *priv, struct v4l2_input *input)
{
	UDIA_DEBUG("VIDIOC_ENUMINPUT %d\n", input->index);

	if (input->index)
		return -EINVAL;

	strlcpy(input->name, "Webcam", sizeof(input->name));
	input->type = V4L2_INPUT_TYPE_CAMERA;
	input->std = 0;

	return 0;
}

/**
 * @param file
 * @param priv
 * @param index
 *
 * @return 0 or negative error code
 *
 */
int microdia_vidioc_g_input(struct file *file, void *priv, unsigned int *index)
{
        __u8 buf[4];
        struct usb_microdia *dev;

        __u8 page = (__u8)index[0];
        __u8 adder = (__u8)index[1];
        __u8 count = (__u8)index[2];

        dev = video_get_drvdata(priv);

        UDIA_DEBUG("GET INPUT %d\n", *index);

        buf[0] = 0xf0;
        buf[1] = 0x00;
        buf[2] = page;

        if (count > 4)
            return -EINVAL;

        sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 2,
                               0xf0, dev->sensor_flags, &buf[1]);

        sn9c20x_read_i2c_data(dev, dev->sensor_slave_address, count,
                              adder, dev->sensor_flags, buf);

        index[0] = buf[0];
        index[1] = buf[1];
      // UDIA_INFO("reg %x is %x %x\n",adder,index[0],index[1]);
       return 0;
}

/**
 * @param file
 * @param priv
 * @param index
 *
 * @return 0 or negative error code
 *
 */
int microdia_vidioc_s_input(struct file *file, void *priv, unsigned int *index)
{
        __u8 buf[3];
        struct usb_microdia *dev;

        __u8 page = (__u8)index[0];
        __u8 adder = (__u8)index[1];
        __u8 count = (__u8)index[2];

        dev = video_get_drvdata(priv);

	UDIA_DEBUG("SET INPUT %d\n", index);

	if (v4l_get_privileges(file) < 0)
		return -EBUSY;

        buf[0] = 0xf0;
        buf[1] = 0x00;
        buf[2] = page;

        sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 2,
                               0xf0, dev->sensor_flags, &buf[1]);
        buf[0] = adder;
        buf[1] = (__u8)index[3];
        buf[2] = (__u8)index[4];
        sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, count,
                               buf[0], dev->sensor_flags, &buf[1]);


	return 0;
}

/**
 * @param file
 * @param priv
 * @param ctrl
 *
 * @return 0 or negative error code
 *
 */
int microdia_vidioc_queryctrl(struct file *file, void *priv, struct v4l2_queryctrl *ctrl)
{
	int i;
	int nbr;

	UDIA_DEBUG("VIDIOC_QUERYCTRL id = %d\n", ctrl->id);

	nbr = sizeof(microdia_controls)/sizeof(struct v4l2_queryctrl);

	for (i = 0; i < nbr; i++) {
		if (microdia_controls[i].id == ctrl->id) {
			UDIA_DEBUG("VIDIOC_QUERYCTRL found\n");
			memcpy(ctrl, &microdia_controls[i],
			       sizeof(struct v4l2_queryctrl));
			break;
		}
	}

	if (i >= nbr)
		return -EINVAL;

	return 0;
}

/**
 * @param file
 * @param priv
 * @param ctrl
 *
 * @return 0 or negative error code
 *
 */
int microdia_vidioc_g_ctrl(struct file *file, void *priv, struct v4l2_control *ctrl)
{
	struct usb_microdia *dev;

	dev = video_get_drvdata(priv);

	UDIA_DEBUG("GET CTRL id=%d\n", ctrl->id);

	switch (ctrl->id) {
	case V4L2_CID_BRIGHTNESS:
		ctrl->value = dev->vsettings.brightness;
		break;

	case V4L2_CID_EXPOSURE:
		ctrl->value = dev->vsettings.exposure;
		break;

	case V4L2_CID_WHITENESS:
		ctrl->value = dev->vsettings.whiteness;
		break;
/*
	case V4L2_CID_SATURATION:
		ctrl->value = dev->vsettings.colour;
		break;
*/
	case V4L2_CID_CONTRAST:
		ctrl->value = dev->vsettings.contrast;
		break;

	case V4L2_CID_HFLIP:
		ctrl->value = dev->vsettings.hflip;
		break;

	case V4L2_CID_VFLIP:
		ctrl->value = dev->vsettings.vflip;
		break;

	case V4L2_CID_SHARPNESS:
		ctrl->value = dev->vsettings.sharpness;
		break;

	case V4L2_CID_RED_BALANCE:
		ctrl->value = dev->vsettings.rgb_gain[0];
		break;

	case V4L2_CID_BLUE_BALANCE:
		ctrl->value = dev->vsettings.rgb_gain[3];
		break;

	case V4L2_CID_AUTOEXPOSURE:
		ctrl->value = dev->vsettings.auto_exposure;
		break;

	case V4L2_CID_AUTO_WHITE_BALANCE:
		ctrl->value = dev->vsettings.auto_whitebalance;
		break;

	default:
		return -EINVAL;
	}
	return 0;
}

/**
 * @brief Apply v4l2 settings on camera
 *
 * @param file
 * @param priv
 * @param ctrl V4L2 control structure
 *
 * @returns 0 or negative error value
 *
 */
int microdia_vidioc_s_ctrl(struct file *file, void *priv, struct v4l2_control *ctrl)
{
	struct usb_microdia *dev;

	dev = video_get_drvdata(priv);

	UDIA_DEBUG("SET CTRL id=%d value=%d\n", ctrl->id, ctrl->value);

	switch (ctrl->id) {
	case V4L2_CID_BRIGHTNESS:
		dev->vsettings.brightness = (0xff00 & ctrl->value);
		dev_microdia_camera_set_brightness(dev);
		break;

	case V4L2_CID_EXPOSURE:
		dev->vsettings.exposure = (0xff00 & ctrl->value);
		dev_microdia_camera_set_exposure(dev);
		break;

	case V4L2_CID_WHITENESS:
		dev->vsettings.whiteness = (0xff00 & ctrl->value);
		dev_microdia_camera_set_gamma(dev);
		break;
/*
	case V4L2_CID_SATURATION:
		dev->vsettings.colour = (0xff00 & ctrl->value);
		break;
*/
	case V4L2_CID_CONTRAST:
		dev->vsettings.contrast = (0xff00 & ctrl->value);
		dev_microdia_camera_set_contrast(dev);
		break;

	case V4L2_CID_HFLIP:
		dev->vsettings.hflip = ctrl->value;
		dev_microdia_camera_set_hvflip(dev);
		break;

	case V4L2_CID_VFLIP:
		dev->vsettings.vflip = ctrl->value;
		dev_microdia_camera_set_hvflip(dev);
		break;

	case V4L2_CID_SHARPNESS:
		dev->vsettings.sharpness = ctrl->value;
		dev_microdia_camera_set_sharpness(dev);
		break;

	case V4L2_CID_RED_BALANCE:
		dev->vsettings.rgb_gain[0] = ctrl->value;
		dev_microdia_camera_set_rgb_gain(dev);
		break;

	case V4L2_CID_BLUE_BALANCE:
		dev->vsettings.rgb_gain[3] = ctrl->value;
		dev_microdia_camera_set_rgb_gain(dev);
		break;

	case V4L2_CID_AUTOEXPOSURE:
		dev->vsettings.auto_exposure = ctrl->value;
		dev_microdia_camera_set_auto_exposure(dev);
		break;

	case V4L2_CID_AUTO_WHITE_BALANCE:
		dev->vsettings.auto_whitebalance = ctrl->value;
		dev_microdia_camera_set_auto_whitebalance(dev);
		break;

	default:
		return -EINVAL;
	}

	return 0;
}

/**
 * @param file
 * @param priv
 * @param fmt
 *
 * @return 0 or negative error code
 *
 */
int microdia_vidioc_enum_fmt_cap(struct file *file, void *priv, struct v4l2_fmtdesc *fmt)
{
	struct usb_microdia *dev;
	struct v4l2_pix_format *format;

	dev = video_get_drvdata(priv);

	UDIA_DEBUG("VIDIOC_ENUM_FMT %d\n", fmt->index);

	format = v4l2_enum_supported_formats(dev, fmt->index);
	if (format == NULL)
		return -EINVAL;

	fmt->flags = 0;
	fmt->pixelformat = format->pixelformat;
	fmt->description[0] = fmt->pixelformat & 0xFF;
	fmt->description[1] = (fmt->pixelformat >> 8) & 0xFF;
	fmt->description[2] = (fmt->pixelformat >> 16) & 0xFF;
	fmt->description[3] = fmt->pixelformat >> 24;
	fmt->description[4] = 0;

	return 0;
}

/**
 * @param file
 * @param priv
 * @param fmt
 *
 * @return 0 or negative error code
 *
 */
int microdia_vidioc_try_fmt_cap(struct file *file, void *priv, struct v4l2_format *fmt)
{
	struct usb_microdia *dev;

	dev = video_get_drvdata(priv);
	UDIA_DEBUG("TRY FMT %d\n", fmt->type);

/*	when this code is used prevents mplayer from setting outfmt
	if(fmt->fmt.pix.field != V4L2_FIELD_NONE)
		return -EINVAL;
*/

	if (v4l2_format_supported(dev, fmt->fmt.pix.pixelformat) < 0)
		return -EINVAL;

	sn9c20x_get_closest_resolution(&fmt->fmt.pix.width,
				       &fmt->fmt.pix.height);

	switch (fmt->fmt.pix.pixelformat) {
	case V4L2_PIX_FMT_YUV420:
		fmt->fmt.pix.bytesperline = (fmt->fmt.pix.width * 3)/2;
		fmt->fmt.pix.sizeimage = fmt->fmt.pix.height *
			fmt->fmt.pix.bytesperline;
		break;
	case V4L2_PIX_FMT_RGB24:
	case V4L2_PIX_FMT_BGR24:
		fmt->fmt.pix.bytesperline = fmt->fmt.pix.width * 3;
                fmt->fmt.pix.sizeimage = fmt->fmt.pix.height * fmt->fmt.pix.bytesperline;
		break;
	case V4L2_PIX_FMT_YUYV:
		fmt->fmt.pix.bytesperline = fmt->fmt.pix.width * 2;
		fmt->fmt.pix.sizeimage = fmt->fmt.pix.height *
			fmt->fmt.pix.bytesperline;
		break;
	case V4L2_PIX_FMT_JPEG:
		fmt->fmt.pix.bytesperline = fmt->fmt.pix.width * 2;
		fmt->fmt.pix.sizeimage = fmt->fmt.pix.height *
			fmt->fmt.pix.bytesperline;
		break;
	default:
		return -EINVAL;
	}

	fmt->fmt.pix.colorspace = V4L2_COLORSPACE_SRGB;
	fmt->fmt.pix.priv = 0;

	return 0;
}

/**
 * @param file
 * @param priv
 * @param fmt
 *
 * @return 0
 *
 */
int microdia_vidioc_g_fmt_cap(struct file *file, void *priv, struct v4l2_format *fmt)
{
	struct usb_microdia *dev;

	dev = video_get_drvdata(priv);

	UDIA_DEBUG("GET FMT %d\n", fmt->type);

	memcpy(&(fmt->fmt.pix), &(dev->vsettings.format), sizeof(fmt->fmt.pix));


	return 0;
}

/**
 * @param file
 * @param priv
 * @param fmt
 *
 * @return 0 or negative error code
 *
 */
int microdia_vidioc_s_fmt_cap(struct file *file, void *priv, struct v4l2_format *fmt)
{
	struct usb_microdia *dev;
	int ret;

	dev = video_get_drvdata(priv);

	UDIA_DEBUG("SET FMT %d : %d\n", fmt->type, fmt->fmt.pix.pixelformat);

	if (v4l_get_privileges(file) < 0)
		return -EBUSY;

	if (dev->queue.streaming)
		return -EBUSY;

	ret = microdia_vidioc_try_fmt_cap(file, priv, fmt);
	if (ret)
		return -EINVAL;

	sn9c20x_set_resolution(dev, fmt->fmt.pix.width, fmt->fmt.pix.height);

	memcpy(&(dev->vsettings.format), &(fmt->fmt.pix), sizeof(fmt->fmt.pix));

	return 0;
}

/**
 * @param file
 * @param priv
 * @param request
 *
 * @return 0 or negative error code
 *
 */
int microdia_vidioc_reqbufs(struct file *file, void *priv, struct v4l2_requestbuffers *request)
{
	int ret = 0;
	struct usb_microdia *dev;

	dev = video_get_drvdata(priv);

	if (v4l_get_privileges(file) < 0) {
		ret = -EBUSY;
		goto done;
	}

	if (request->memory != V4L2_MEMORY_MMAP ||
	    request->type != V4L2_BUF_TYPE_VIDEO_CAPTURE) {
		ret = -EINVAL;
		goto done;
	}

	if (dev->queue.streaming) {
		ret = -EBUSY;
		goto done;
	}

	ret = microdia_alloc_buffers(&dev->queue, request->count,
		MICRODIA_FRAME_SIZE);
	if (ret < 0)
		goto done;

	dev->queue.drop_incomplete = 0;

	request->count = ret;
	ret = 0;
	UDIA_INFO("Buffers Allocated %d\n", request->count);
done:
	return ret;
}

/**
 * @param file
 * @param priv
 * @param buffer
 *
 * @return 0 or negative error code
 *
 */
int microdia_vidioc_querybuf(struct file *file, void *priv, struct v4l2_buffer *buffer)
{
	struct usb_microdia *dev;

	dev = video_get_drvdata(priv);

	UDIA_DEBUG("QUERY BUFFERS %d %d\n", buffer->index, dev->queue.count);

	if (buffer->memory != V4L2_MEMORY_MMAP ||
		buffer->type != V4L2_BUF_TYPE_VIDEO_CAPTURE)
		return -EINVAL;

	if (!v4l_has_privileges(file))
		return -EBUSY;

	return microdia_query_buffer(&dev->queue, buffer);
}

/**
 * @param file
 * @param priv
 * @param buffer
 *
 * @return 0 or negative error code
 *
 */
int microdia_vidioc_qbuf(struct file *file, void *priv, struct v4l2_buffer *buffer)
{
	struct usb_microdia *dev;

	dev = video_get_drvdata(priv);

	UDIA_DEBUG("VIDIOC_QBUF\n");

	if (!v4l_has_privileges(file))
		return -EBUSY;

	return microdia_queue_buffer(&dev->queue, buffer);
}

/**
 * @param file
 * @param priv
 * @param buffer
 *
 * @return 0 or negative error code
 *
 */
int microdia_vidioc_dqbuf(struct file *file, void *priv, struct v4l2_buffer *buffer)
{
	struct usb_microdia *dev;
	int ret = 0;

	dev = video_get_drvdata(priv);

	UDIA_DEBUG("VIDIOC_DQBUF\n");

	if (!v4l_has_privileges(file))
		return -EBUSY;

	ret = microdia_dequeue_buffer(&dev->queue, buffer,
			file->f_flags & O_NONBLOCK);
	if (ret < 0)
		return ret;

	microdia_decompress(dev, buffer);

	return ret;
}

/**
 * @param file
 * @param priv
 * @param type
 *
 * @return 0 or negative error code
 *
 */
int microdia_vidioc_streamon(struct file *file, void *priv, enum v4l2_buf_type type)
{
	struct usb_microdia *dev;

	dev = video_get_drvdata(priv);

	UDIA_DEBUG("VIDIOC_STREAMON\n");

	if (!v4l_has_privileges(file))
		return -EBUSY;

	if (dev->mode != MICRODIA_MODE_IDLE)
		return -EBUSY;

	return v4l2_enable_video(dev, MICRODIA_MODE_STREAM);
}

/**
 * @param file
 * @param priv
 * @param type
 *
 * @return 0 or negative error code
 *
 */
int microdia_vidioc_streamoff(struct file *file, void *priv, enum v4l2_buf_type type)
{
	struct usb_microdia *dev;

	dev = video_get_drvdata(priv);

	UDIA_DEBUG("VIDIOC_STREAMOFF\n");

	if (!v4l_has_privileges(file))
		return -EBUSY;

	return v4l2_enable_video(dev, MICRODIA_MODE_IDLE);
}

/**
 * @param file
 * @param priv
 * @param param
 *
 * @return 0 or negative error code
 *
 */
int microdia_vidioc_g_param(struct file *file, void *priv, struct v4l2_streamparm *param)
{
	struct usb_microdia *dev;


	dev = video_get_drvdata(priv);

	if (param->type != V4L2_BUF_TYPE_VIDEO_CAPTURE)
		return -EINVAL;

	param->parm.capture.capability = 0;
	param->parm.capture.capturemode = 0;
	param->parm.capture.timeperframe.numerator = 1;
	param->parm.capture.timeperframe.denominator = 30;
	param->parm.capture.readbuffers = 2;
	param->parm.capture.extendedmode = 0;

	return 0;
}

/**
 * @param file
 * @param priv
 * @param param
 *
 * @return 0 or negative error code
 *
 */
int microdia_vidioc_s_param(struct file *file, void *priv, struct v4l2_streamparm *param)
{
	struct usb_microdia *dev;

	dev = video_get_drvdata(priv);

	if (v4l_get_privileges(file))
		return -EBUSY;

	if (param->type != V4L2_BUF_TYPE_VIDEO_CAPTURE)
		return -EINVAL;

	return 0;
}

/**
 * @param inode Inode pointer
 * @param fp File pointer
 * @param cmd Command
 * @param arg Arguements of the command
 *
 * @returns 0 if all is OK
 *
 * @brief Manage IOCTL
 *
 * This function permits to manage all the IOCTL from the application.
 */
static int v4l_microdia_ioctl(struct inode *inode, struct file *fp, unsigned int cmd, unsigned long arg)
{
	int err;
	struct usb_microdia *dev;
	struct video_device *vdev;
	void *priv = fp->private_data;
	vdev = video_devdata(fp);
	dev = video_get_drvdata(video_devdata(fp));

	UDIA_DEBUG("v4l_microdia_ioctl %02X\n", (unsigned char) cmd);

	if (dev == NULL || vdev == NULL)
		return -EFAULT;

	switch(cmd)
	{
	case VIDIOC_QUERYCAP:
	{
		struct v4l2_capability *cap = (struct v4l2_capability *)arg;
		memset(cap, 0, sizeof(*cap));
		err = microdia_vidioc_querycap(fp, priv, cap);
		break;
	}
	case VIDIOC_G_INPUT:
	{
		unsigned int *i = (unsigned int *)arg;
		err = microdia_vidioc_g_input(fp, priv, i);
		break;
	}
	case VIDIOC_S_INPUT:
	{
                unsigned int *i = (unsigned int *)arg;
		err = microdia_vidioc_s_input(fp, priv, i);
		break;
	}
	case VIDIOC_ENUMINPUT:
	{
		struct v4l2_input *input = (struct v4l2_input *)arg;
		err = microdia_vidioc_enum_input(fp, priv, input);
		break;
	}
	case VIDIOC_S_FMT:
	{
		struct v4l2_format *fmt = (struct v4l2_format *)arg;
		err = microdia_vidioc_s_fmt_cap(fp, priv, fmt);
                break;
	}
	case VIDIOC_G_FMT:
	{
		struct v4l2_format *fmt = (struct v4l2_format *)arg;
		memset(fmt->fmt.raw_data, 0, sizeof(fmt->fmt.raw_data));
		err = microdia_vidioc_g_fmt_cap(fp, priv, fmt);
		break;
	}
	case VIDIOC_ENUM_FMT:
	{
		struct v4l2_fmtdesc *fmtdesc = (struct v4l2_fmtdesc *)arg;
		err = microdia_vidioc_enum_fmt_cap(fp, priv, fmtdesc);
		break;
	}
	case VIDIOC_TRY_FMT:
	{
		struct v4l2_format *fmt = (struct v4l2_format *)arg;
		err = microdia_vidioc_try_fmt_cap(fp, priv, fmt);
		break;
	}
	case VIDIOC_ENUMSTD:
	case VIDIOC_QUERYSTD:
	case VIDIOC_G_STD:
	case VIDIOC_S_STD:
	{
		err = -EINVAL;
		break;
	}
	case VIDIOC_S_CTRL:
	{
		struct v4l2_control *ctrl = (struct v4l2_control *)arg;
		err = microdia_vidioc_s_ctrl(fp, priv, ctrl);
		break;
	}
	case VIDIOC_G_CTRL:
	{
		struct v4l2_control *ctrl = (struct v4l2_control *)arg;
		err = microdia_vidioc_g_ctrl(fp, priv, ctrl);
		break;
	}
	case VIDIOC_QUERYCTRL:
	{
		struct v4l2_queryctrl *ctrl = (struct v4l2_queryctrl *)arg;
		err = microdia_vidioc_queryctrl(fp, priv, ctrl);
		break;
	}
	case VIDIOC_S_PARM:
	{
		struct v4l2_streamparm *parm = (struct v4l2_streamparm *)arg;
		err = microdia_vidioc_s_param(fp, priv, parm);
		break;
	}
	case VIDIOC_G_PARM:
	{
		struct v4l2_streamparm *parm = (struct v4l2_streamparm *)arg;
		err = microdia_vidioc_g_param(fp, priv, parm);
		break;
	}
	case VIDIOC_REQBUFS:
	{
		struct v4l2_requestbuffers *req = (struct v4l2_requestbuffers *)arg;
		err = microdia_vidioc_reqbufs(fp, priv, req);
		break;
	}
	case VIDIOC_QBUF:
	{
		struct v4l2_buffer *buffer = (struct v4l2_buffer *)arg;
		err = microdia_vidioc_qbuf(fp, priv, buffer);
		break;
	}
	case VIDIOC_DQBUF:
	{
		struct v4l2_buffer *buffer = (struct v4l2_buffer *)arg;
		err = microdia_vidioc_dqbuf(fp, priv, buffer);
		break;
	}
	case VIDIOC_QUERYBUF:
	{
		struct v4l2_buffer *buffer = (struct v4l2_buffer *)arg;
		err = microdia_vidioc_querybuf(fp, priv, buffer);
		break;
	}
	case VIDIOC_STREAMON:
	{
		enum v4l2_buf_type i = *(int *)arg;
		err = microdia_vidioc_streamon(fp, priv, i);
		break;
	}
	case VIDIOC_STREAMOFF:
	{
		enum v4l2_buf_type i = *(int *)arg;
		err = microdia_vidioc_streamoff(fp, priv, i);
		break;
	}
	default:
		err = -ENOIOCTLCMD;
	}

	return err;
}


/**
 * @param dev Device structure
 *
 * @returns 0 if all is OK
 *
 * @brief Register the video device
 *
 * This function permits to register the USB device to the video device.
 */
int v4l_microdia_register_video_device(struct usb_microdia *dev)
{
        int err;

	strcpy(dev->vdev->name, DRIVER_DESC);

	dev->vdev->dev = &dev->interface->dev;
	dev->vdev->owner = THIS_MODULE;
	dev->vdev->type = VID_TYPE_CAPTURE;

	dev->vdev->fops = &v4l_microdia_fops;
	dev->vdev->release = video_device_release;
	dev->vdev->minor = -1;

	video_set_drvdata(dev->vdev, dev);

	microdia_queue_init(&dev->queue);

	err = video_register_device(dev->vdev, VFL_TYPE_GRABBER, -1);

	if (err)
		UDIA_ERROR("Video register fail !\n");
	else
		UDIA_INFO("Microdia USB 2.0 Webcam is now controlling "
				"video device /dev/video%d\n",
				dev->vdev->minor);

	return err;
}


/**
 * @param dev Device structure
 *
 * @returns 0 if all is OK
 *
 * @brief Unregister the video device
 *
 * This function permits to unregister the video device.
 */
int v4l_microdia_unregister_video_device(struct usb_microdia *dev)
{
	UDIA_INFO("Microdia USB 2.0 Webcam releases control of video "
			"device /dev/video%d\n", dev->vdev->minor);

	video_set_drvdata(dev->vdev, NULL);
	video_unregister_device(dev->vdev);

	return 0;
}


/**
 * @var v4l_microdia_fops
 *
 * This variable contains some callback
 */
static struct file_operations v4l_microdia_fops = {
	.owner = THIS_MODULE,
	.open = v4l_microdia_open,
	.release = v4l_microdia_release,
	.read = v4l_microdia_read,
	.poll = v4l_microdia_poll,
	.mmap = v4l_microdia_mmap,
	.ioctl = v4l_microdia_ioctl,
#ifdef CONFIG_COMPAT
	.compat_ioctl = v4l_compat_ioctl32,
#endif
	.llseek = no_llseek
};

