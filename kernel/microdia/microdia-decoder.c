/**
 * @file microdia-decoder.c
 * @author Nicolas VIVIEN
 * @date 2008-02-01
 *
 * @brief Decoder for device specific image formats
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

#include <linux/usb.h>
#include <media/v4l2-common.h>

#include "microdia.h"
#include "sn9c20x.h"

#define MAX(a, b)	((a) > (b) ? (a) : (b))
#define MIN(a, b)	((a) < (b) ? (a) : (b))
#define CLIP(a, low, high) MAX((low), MIN((high), (a)))

void raw6270_2i420(uint8_t *, uint8_t *, int, int, const int, const int);

void raw6270_2RGB24(uint8_t *raw, uint8_t *rgb, int width, int height, const int hflip, const int vflip);

void raw6270_2BGR24(uint8_t *raw, uint8_t *rgb, int width, int height, const int hflip, const int vflip);

void microdia_getraw(uint8_t *, uint8_t *, int);

void microdia_raw2i420(uint8_t *, uint8_t *, int, int, const int, const int);

void microdia_raw2bgr24(uint8_t *, uint8_t *, int, int, const int, const int);

void v4l_add_jpegheader(struct usb_microdia *dev, __u8 *buffer, __u32 buffer_size);
/**
 * @brief Decompress a frame
 *
 * This function permits to decompress a frame from the video stream.
 *
 * @param dev Device structure
 * @param buffer Image data
 *
 * @returns 0 if all is OK
 */
int microdia_decompress(struct usb_microdia *dev, struct v4l2_buffer *buffer)
{
	int factor;
	int vflip;
	int hflip;
	int width, height;
	int ret = 0;

	void *data;
	void *image;

	if (dev == NULL) {
		ret = -EFAULT;
		goto done;
	}

	if (dev->flip_detect)
		dev->flip_detect(dev);

	if (dev->set_hvflip) {
		hflip = 0;
		vflip = 0;
        }
        else {
		hflip = dev->vsettings.hflip;
		vflip = dev->vsettings.vflip;
	}

	image  = dev->queue.scratch;

	data = dev->queue.mem + buffer->m.offset;
	memcpy(image, data, MICRODIA_FRAME_SIZE);

	factor = 1;

	width = dev->vsettings.format.width;
	height = dev->vsettings.format.height;

        //UDIA_INFO("decompress format=%c%c%c%c\n",
        //        0xff & (dev->vsettings.format.pixelformat >>  0),
        //          0xff & (dev->vsettings.format.pixelformat >>  8),
        //          0xff & (dev->vsettings.format.pixelformat >> 16),
        //          0xff & (dev->vsettings.format.pixelformat >> 24));

	switch (dev->vsettings.format.pixelformat) {
	case V4L2_PIX_FMT_RGB24:
		if (dev->webcam_model ==
		    CAMERA_MODEL(USB_0C45_VID, USB_6270_PID) ||
		    dev->webcam_model ==
		    CAMERA_MODEL(USB_0C45_VID, USB_627B_PID) ||
		    dev->webcam_model ==
		    CAMERA_MODEL(USB_0C45_VID, USB_6288_PID) ||
		    dev->webcam_model ==
		    CAMERA_MODEL(USB_0C45_VID, USB_62B3_PID) ||
		    dev->webcam_model ==
		    CAMERA_MODEL(USB_0C45_VID, USB_62BB_PID) ||
		    dev->webcam_model ==
		    CAMERA_MODEL(USB_145F_VID, USB_013D_PID)) {

                        raw6270_2RGB24(image, data, width, height, hflip, vflip);
		}
		break;
	case V4L2_PIX_FMT_BGR24:
          //UDIA_INFO("V4L2_PIX_FMT_BGR24\n");
		if (dev->webcam_model ==
		    CAMERA_MODEL(USB_0C45_VID, USB_6270_PID) ||
		    dev->webcam_model ==
		    CAMERA_MODEL(USB_0C45_VID, USB_627B_PID) ||
		    dev->webcam_model ==
		    CAMERA_MODEL(USB_0C45_VID, USB_6288_PID) ||
		    dev->webcam_model ==
		    CAMERA_MODEL(USB_0C45_VID, USB_62B3_PID) ||
		    dev->webcam_model ==
		    CAMERA_MODEL(USB_0C45_VID, USB_62BB_PID) ||
		    dev->webcam_model ==
		    CAMERA_MODEL(USB_145F_VID, USB_013D_PID)) {

                        raw6270_2BGR24(image, data, width, height, hflip, vflip);
                }
                else {
                        microdia_raw2bgr24(image, data, width, height, hflip, vflip);
                        // Uncompressing doubles the amount of data
                        //UDIA_INFO("bytes used=%d\n", buffer->bytesused);
                        buffer->bytesused *= 2;
		}
		break;
	case V4L2_PIX_FMT_YUV420:
		if (dev->webcam_model ==
		    CAMERA_MODEL(USB_0C45_VID, USB_6270_PID) ||
		    dev->webcam_model ==
		    CAMERA_MODEL(USB_0C45_VID, USB_627B_PID) ||
		    dev->webcam_model ==
		    CAMERA_MODEL(USB_0C45_VID, USB_6288_PID) ||
		    dev->webcam_model ==
		    CAMERA_MODEL(USB_0C45_VID, USB_62B3_PID) ||
		    dev->webcam_model ==
		    CAMERA_MODEL(USB_0C45_VID, USB_62BB_PID) ||
		    dev->webcam_model ==
		    CAMERA_MODEL(USB_145F_VID, USB_013D_PID)) {
			raw6270_2i420(image, data, width,
				      height, hflip, vflip);
		} else {
			microdia_raw2i420(image, data, width,
					  height, hflip, vflip);
		}
		break;
	case V4L2_PIX_FMT_YUYV:
		break;
	case V4L2_PIX_FMT_JPEG:
		v4l_add_jpegheader(dev, data, buffer->bytesused);
		buffer->bytesused += 589;
	default:
		break;
	}
done:
	return ret;
}

void v4l_add_jpegheader(struct usb_microdia *dev, __u8 *buffer, __u32 buffer_size)
{
	__u8 jpeg_header[589] = {
		0xff, 0xd8, 0xff, 0xdb, 0x00, 0x84, 0x00, 0x06, 0x04, 0x05,
		0x06, 0x05, 0x04, 0x06, 0x06, 0x05, 0x06, 0x07, 0x07, 0x06,
		0x08, 0x0a, 0x10, 0x0a, 0x0a, 0x09, 0x09, 0x0a, 0x14, 0x0e,
		0x0f, 0x0c, 0x10, 0x17, 0x14, 0x18, 0x18, 0x17, 0x14, 0x16,
		0x16, 0x1a, 0x1d, 0x25, 0x1f, 0x1a, 0x1b, 0x23, 0x1c, 0x16,
		0x16, 0x20, 0x2c, 0x20, 0x23, 0x26, 0x27, 0x29, 0x2a, 0x29,
		0x19, 0x1f, 0x2d, 0x30, 0x2d, 0x28, 0x30, 0x25, 0x28, 0x29,
		0x28, 0x01, 0x07, 0x07, 0x07, 0x0a, 0x08, 0x0a, 0x13, 0x0a,
		0x0a, 0x13, 0x28, 0x1a, 0x16, 0x1a, 0x28, 0x28, 0x28, 0x28,
		0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28,
		0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28,
		0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28,
		0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28,
		0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0xff, 0xc4, 0x01, 0xa2,
		0x00, 0x00, 0x01, 0x05, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02,
		0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x01,
		0x00, 0x03, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
		0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x03,
		0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x10, 0x00,
		0x02, 0x01, 0x03, 0x03, 0x02, 0x04, 0x03, 0x05, 0x05, 0x04,
		0x04, 0x00, 0x00, 0x01, 0x7d, 0x01, 0x02, 0x03, 0x00, 0x04,
		0x11, 0x05, 0x12, 0x21, 0x31, 0x41, 0x06, 0x13, 0x51, 0x61,
		0x07, 0x22, 0x71, 0x14, 0x32, 0x81, 0x91, 0xa1, 0x08, 0x23,
		0x42, 0xb1, 0xc1, 0x15, 0x52, 0xd1, 0xf0, 0x24, 0x33, 0x62,
		0x72, 0x82, 0x09, 0x0a, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x25,
		0x26, 0x27, 0x28, 0x29, 0x2a, 0x34, 0x35, 0x36, 0x37, 0x38,
		0x39, 0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a,
		0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x63, 0x64,
		0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x73, 0x74, 0x75, 0x76,
		0x77, 0x78, 0x79, 0x7a, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88,
		0x89, 0x8a, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99,
		0x9a, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa,
		0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xc2,
		0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2, 0xd3,
		0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xe1, 0xe2, 0xe3,
		0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xf1, 0xf2, 0xf3,
		0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0x11, 0x00, 0x02,
		0x01, 0x02, 0x04, 0x04, 0x03, 0x04, 0x07, 0x05, 0x04, 0x04,
		0x00, 0x01, 0x02, 0x77, 0x00, 0x01, 0x02, 0x03, 0x11, 0x04,
		0x05, 0x21, 0x31, 0x06, 0x12, 0x41, 0x51, 0x07, 0x61, 0x71,
		0x13, 0x22, 0x32, 0x81, 0x08, 0x14, 0x42, 0x91, 0xa1, 0xb1,
		0xc1, 0x09, 0x23, 0x33, 0x52, 0xf0, 0x15, 0x62, 0x72, 0xd1,
		0x0a, 0x16, 0x24, 0x34, 0xe1, 0x25, 0xf1, 0x17, 0x18, 0x19,
		0x1a, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x35, 0x36, 0x37, 0x38,
		0x39, 0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a,
		0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x63, 0x64,
		0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x73, 0x74, 0x75, 0x76,
		0x77, 0x78, 0x79, 0x7a, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
		0x88, 0x89, 0x8a, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98,
		0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9,
		0xaa, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba,
		0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2,
		0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xe2, 0xe3,
		0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xf2, 0xf3, 0xf4,
		0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xff, 0xc0, 0x00, 0x11,
		0x08, 0x01, 0xe0, 0x02, 0x80, 0x03, 0x01, 0x21, 0x00, 0x02,
		0x11, 0x01, 0x03, 0x11, 0x01, 0xff, 0xda, 0x00, 0x0c, 0x03,
		0x01, 0x00, 0x02, 0x11, 0x03, 0x11, 0x00, 0x3f, 0x00
	};
	__u8 qtable1[128] = {
		0x0d, 0x08, 0x08, 0x0d, 0x08, 0x08, 0x0d, 0x0d,
		0x0d, 0x0d, 0x11, 0x0d, 0x0d, 0x11, 0x15, 0x21,
		0x15, 0x15, 0x11, 0x11, 0x15, 0x2a, 0x1d, 0x1d,
		0x19, 0x21, 0x32, 0x2a, 0x32, 0x32, 0x2e, 0x2a,
		0x2e, 0x2e, 0x36, 0x3a, 0x4b, 0x43, 0x36, 0x3a,
		0x47, 0x3a, 0x2e, 0x2e, 0x43, 0x5c, 0x43, 0x47,
		0x4f, 0x54, 0x58, 0x58, 0x58, 0x32, 0x3f, 0x60,
		0x64, 0x5c, 0x54, 0x64, 0x4b, 0x54, 0x58, 0x54,
		0x0d, 0x11, 0x11, 0x15, 0x11, 0x15, 0x26, 0x15,
		0x15, 0x26, 0x54, 0x36, 0x2e, 0x36, 0x54, 0x54,
		0x54, 0x54, 0x54, 0x54, 0x54, 0x54, 0x54, 0x54,
		0x54, 0x54, 0x54, 0x54, 0x54, 0x54, 0x54, 0x54,
		0x54, 0x54, 0x54, 0x54, 0x54, 0x54, 0x54, 0x54,
		0x54, 0x54, 0x54, 0x54, 0x54, 0x54, 0x54, 0x54,
		0x54, 0x54, 0x54, 0x54, 0x54, 0x54, 0x54, 0x54,
		0x54, 0x54, 0x54, 0x54, 0x54, 0x54, 0x54, 0x54
	};

	jpeg_header[6] = 0x00;
	jpeg_header[71] = 0x01;
	memcpy(jpeg_header + 7, qtable1, 64);
	memcpy(jpeg_header + 8 + 64, qtable1+64, 64);
	jpeg_header[564] = dev->vsettings.format.width & 0xFF;
	jpeg_header[563] = (dev->vsettings.format.width >> 8) & 0xFF;
	jpeg_header[562] = dev->vsettings.format.height & 0xFF;
	jpeg_header[561] = (dev->vsettings.format.height >> 8) & 0xFF;
	jpeg_header[567] = 0x21;

	memmove(buffer+589, buffer, buffer_size);
	memcpy(buffer, jpeg_header, 589);
}
/**
 * @brief This function permits to get the raw data. (without treatments)
 *
 * @param bayer Buffer with the bayer data
 * @param size Length of bayer buffer
 *
 * @retval raw Buffer with the data from video sensor
 */
void microdia_getraw(uint8_t *bayer, uint8_t *raw,
		int size) {
	memcpy(raw, bayer, size);
}

/* Table to translate Y offset to UV offset */
static int UVTranslate[32] =	{
				0, 1, 2, 3,
				8, 9, 10, 11,
				16, 17, 18, 19,
				24, 25, 26, 27,
				4, 5, 6, 7,
				12, 13, 14, 15,
				20, 21, 22, 23,
				28, 29, 30, 31
				};

static int Y_coords_624x[128][2] = {
{ 0,  0}, { 1,  0}, { 2,  0}, { 3,  0}, { 4,  0}, { 5,  0}, { 6,  0}, { 7,  0},
{ 0,  1}, { 1,  1}, { 2,  1}, { 3,  1}, { 4,  1}, { 5,  1}, { 6,  1}, { 7,  1},
{ 0,  2}, { 1,  2}, { 2,  2}, { 3,  2}, { 4,  2}, { 5,  2}, { 6,  2}, { 7,  2},
{ 0,  3}, { 1,  3}, { 2,  3}, { 3,  3}, { 4,  3}, { 5,  3}, { 6,  3}, { 7,  3},

{ 0,  4}, { 1,  4}, { 2,  4}, { 3,  4}, { 4,  4}, { 5,  4}, { 6,  4}, { 7,  4},
{ 0,  5}, { 1,  5}, { 2,  5}, { 3,  5}, { 4,  5}, { 5,  5}, { 6,  5}, { 7,  5},
{ 0,  6}, { 1,  6}, { 2,  6}, { 3,  6}, { 4,  6}, { 5,  6}, { 6,  6}, { 7,  6},
{ 0,  7}, { 1,  7}, { 2,  7}, { 3,  7}, { 4,  7}, { 5,  7}, { 6,  7}, { 7,  7},

{ 8,  0}, { 9,  0}, {10,  0}, {11,  0}, {12,  0}, {13,  0}, {14,  0}, {15,  0},
{ 8,  1}, { 9,  1}, {10,  1}, {11,  1}, {12,  1}, {13,  1}, {14,  1}, {15,  1},
{ 8,  2}, { 9,  2}, {10,  2}, {11,  2}, {12,  2}, {13,  2}, {14,  2}, {15,  2},
{ 8,  3}, { 9,  3}, {10,  3}, {11,  3}, {12,  3}, {13,  3}, {14,  3}, {15,  3},

{ 8,  4}, { 9,  4}, {10,  4}, {11,  4}, {12,  4}, {13,  4}, {14,  4}, {15,  4},
{ 8,  5}, { 9,  5}, {10,  5}, {11,  5}, {12,  5}, {13,  5}, {14,  5}, {15,  5},
{ 8,  6}, { 9,  6}, {10,  6}, {11,  6}, {12,  6}, {13,  6}, {14,  6}, {15,  6},
{ 8,  7}, { 9,  7}, {10,  7}, {11,  7}, {12,  7}, {13,  7}, {14,  7}, {15,  7}
};

/**
 *
 * @brief This function permits to convert an image from 624x raw format to bgr24
 * @param raw Buffer with the raw data
 * @param width Width of image
 * @param height Height of image
 * @param hflip Horizontal flip
 * @param vflip Vertical flip
 *
 * @retval rgb Buffer with the rgb data
 */
void microdia_raw2bgr24(uint8_t *raw, uint8_t *rgb, int width, int height, const int hflip, const int vflip)
{
	int i = 0, x = 0, y = 0;
	unsigned char *buf = raw;
	unsigned char *buf3 = rgb;
        int frameSize = width * height + (width * height)/2;

        //UDIA_INFO("microdia_raw2bgr24 width=%d height=%d\n", width, height);

        while (i < frameSize) {
		int tile = 0;
		for (tile = 0; tile < 4; tile++) {
			int subX = 0;
			int subY = 0;
			for (subY = 0; subY < 4; subY++) {
				for (subX = 0; subX < 8; subX++) {
					int subI = i + tile * 32 + 8 * subY + subX;
					int subU = i + 128 + UVTranslate[tile * 8 + 4 * (subY >> 1) + (subX >> 1)];
					int subV = subU + 32;

					int relX = x + (((tile == 0) || (tile == 1)) ? 0 : 8) + subX; /* tile 0, 1 to into left column*/
					int relY = y + (((tile == 0) || (tile == 2)) ? 0 : 4) + subY; /* tile 0, 2 go into top row */

					if (hflip)
						relX = width - relX;
					if (vflip)
						relY = height - relY;

					if ((relX < width) && (relY < height)) {
						unsigned char *ptr;
						int c, d, e;
						c = buf[subI] - 16;
						d = buf[subU] - 128;
						e = buf[subV] - 128;

						ptr = buf3 + relY * width * 3 + relX * 3;
						*ptr = CLIP((298 * c + 516 * d + 128) >> 8, 0, 255);
						ptr++;
						*ptr = CLIP((298 * c - 100 * d - 208 * e + 128) >> 8, 0, 255);
						ptr++;
						*ptr = CLIP((298 * c + 409 * e + 128) >> 8, 0, 255);
					}
				}
			}
		}

		i += 192;
		x += 16;
		if (x >= width) {
			x = 0;
			y += 8;
		}
	}
}

/**
 *
 * @brief This function permits to convert an image from 624x raw format to i420
 * @param raw Buffer with the raw data
 * @param width Width of image
 * @param height Height of image
 * @param hflip Horizontal flip
 * @param vflip Vertical flip
 *
 * @retval i420 Buffer with the i420 data
 */
void microdia_raw2i420(uint8_t *raw, uint8_t *i420,
			int width, int height, const int hflip,
			const int vflip)
{
	int i = 0, x = 0, y = 0, j, relX, relY, x_div2, y_div2;
	unsigned char *buf = raw, *ptr;
	unsigned char *buf3 = i420;
	int view_size = width * height;
	int view_size_div4 = view_size >> 2;
	int image_x_div2 = width >> 1;
	int image_y_div2 = height >> 1;
	int view_x_div2 = width >> 1;

	while (i < (width * height + (width * height) / 2)) {
		for (j = 0; j < 128; j++) {
			relX = x + Y_coords_624x[j][0];
			relY = y + Y_coords_624x[j][1];
			if (hflip)
				relX = width - relX - 1;
			if (vflip)
				relY = height - relY - 1;

			if ((relX < width) && (relY < height)) {
				ptr = buf3 + relY * width + relX;
				*ptr = buf[i + j];
			}

		}
		x_div2 = x >> 1;
		y_div2 = y >> 1;
		for (j = 0; j < 32; j++) {
			relX = (x_div2) + (j & 0x07);
			relY = (y_div2) + (j >> 3);
			if (hflip)
				relX = image_x_div2 - relX - 1;
			if (vflip)
				relY = image_y_div2 - relY - 1;

			if ((relX < width) && (relY < height)) {
				ptr = buf3 + view_size +
					relY * (view_x_div2) + relX;
				*ptr = buf[i + 128 + j];
				ptr += view_size_div4;
				*ptr = buf[i + 160 + j];
			}

		}

		i += 192;
		x += 16;
		if (x >= width) {
			x = 0;
			y += 8;
		}
	}
}

/**
 *
 * @brief This function permits to convert an image from 6270 raw format to i420
 * @param raw Buffer with the bayer data
 * @param width Width of image
 * @param height Height of image
 * @param hflip Horizontal flip - not implemented
 * @param vflip Vertical flip - not implemented
 *
 * @retval i420 Buffer with the i420 data
 * Format of stream is uvyy,uvyy - repeated 320 times for 640x480
 *	y - repeated 640 times for 640x480
 *	First 1280 bytes is maybe dummy ***FIXME***
 */
void raw6270_2i420(uint8_t *raw, uint8_t *i420, int width,
			int height, const int hflip,
			const int vflip)
{
	int i, j, YIndex = 0, UVIndex = 0;
	unsigned char *y, *u, *v;
	uint8_t *buf;
	/* For fast calculations */
	int x_div_2, y_div_2;
	/* Skip first 1280 bytes strange dummy bytes */
	raw += width * 2;

	x_div_2 = width / 2; /* for fast calculation if x=640 x_div_2=320 */
	y_div_2 = height / 2; /* for fast calculation if y=480 y_div_4=240 */
	buf = raw;
	y = i420;
	u = i420 + width * height;
	v = i420 + width * height + (width >> 1) * (height >> 1);

	YIndex = 0;
	UVIndex = 0;
	/* we skipped 1280 bytes, it's almost two lines */
	for (i = 0; i < y_div_2 - 1; i++) {
		for (j = 0; j < x_div_2; j++) {
			u[UVIndex] = *buf;
			buf++;
			v[UVIndex] = *buf;
			buf++;
			y[YIndex] = *buf;
			buf++;
			YIndex += 1;
			y[YIndex] = *buf;
			buf++;
			YIndex += 1;
			UVIndex += 1;
		}
		YIndex -= width;
		YIndex += width;

		for (j = 0; j < width; j++) {
			y[YIndex] = *buf;
			buf++;
			YIndex += 1;
		}
		YIndex -= width;
		YIndex += width;

		UVIndex -= x_div_2;
		UVIndex += (width >> 1);
	}
}

/**
 *
 * @brief This function permits to convert an image from 6270 rawformat to BGR24
 * @param raw Buffer with the bayer data
 * @param width Width of image
 * @param height Height of image
 * @param hflip Horizontal flip - not implemented yet
 * @param vflip Vertical flip - not implemented yet
 *
 * @retval rgb Buffer with the rgb data
 *	Format of stream is uvyy,uvyy - repeated 320 times for 640x480
 *	y - repeated 640 times for 640x480
 *	First 1280 bytes is maybe dumy ***FIXME***
 */
void raw6270_2BGR24(uint8_t *raw, uint8_t *rgb, int width,
			int height, const int hflip,
			const int vflip)
{
	int i, j, incX;
/* Maybe is not necesery to use long variables but I not sure in this moment */
	long y11, y12, y21, y22, u, v, y, C, D, E;
	long ScaleIncX, ScaleIncY;
	long pointerIncX, pointerIncY;
	uint8_t *bufUVYY;
	uint8_t *bufY;
	uint8_t *out_row1;
	uint8_t *out_row2;

	/* For fast calculations*/
	int x_div_2, y_div_2;
	/* Skip first 1280 bytes strange dummy bytes */
	raw += width * 2;

	x_div_2 = width / 2;	/* for fast calculation if x=640 x_div_2=320 */
	y_div_2 = height / 2;	/* for fast calculation if y=480 y_div_4=240 */
	incX = 3 * width;	/* Incrementation koeficient for next row*/
	bufUVYY = raw;
	bufY = raw + 2 * width;

	/*
	// Because I can't use float ratio is multiply by 1000
	// then 1000 is equal of increment image (X or Y) with 1
	*/
	ScaleIncX = (1000 * width) / width;
	ScaleIncY = (1000 * height) / height;

	out_row1 = rgb;
	out_row2 = out_row1 + incX;
	/* we skipped 1280 bytes, it's almost two lines*/
	pointerIncY = 1000;
	for (i = 0; i < y_div_2-1; i++) {
		pointerIncX = 1000;
		for (j = 0; j < x_div_2; j++) {
			pointerIncX += ScaleIncX;

			u = (unsigned char)(*bufUVYY);
			bufUVYY++;

			v = (unsigned char)(*bufUVYY);
			bufUVYY++;

			y11 = (unsigned char)(*bufUVYY);
			bufUVYY++;

			y12 = (unsigned char)(*bufUVYY);
			bufUVYY++;

			y21 = (unsigned char)(*bufY);
			bufY++;

			y22 = (unsigned char)(*bufY);
			bufY++;

			if ((pointerIncX > 999) && (pointerIncY > 499)) {
				pointerIncX -= 1001;
				y = y11;
				C = y - 16;
				D = u - 128;
				E = v - 128;

				*out_row1 = CLIP((298 * C + 516 * D + 128) >> 8, 0, 255);
				out_row1++;
				*out_row1 = CLIP((298 * C - 100 * D - 208 * E + 128) >> 8, 0, 255);
				out_row1++;
				*out_row1 = CLIP((298 * C + 409 * E + 128) >> 8, 0, 255);
				out_row1++;

				y = y12;

				C = y - 16;
				D = u - 128;
				E = v - 128;


				*out_row1 = CLIP((298 * C + 516 * D + 128) >> 8, 0, 255);
				out_row1++;
				*out_row1 = CLIP((298 * C - 100 * D - 208 * E + 128) >> 8, 0, 255);
				out_row1++;
				*out_row1 = CLIP((298 * C + 409 * E + 128) >> 8, 0, 255);
				out_row1++;

				/*
				// Second row of stream is displayed only
				// if image is greath than half
				*/
				if (ScaleIncY > 501) {
					y = y21;

					C = y - 16;
					D = u - 128;
					E = v - 128;

					*out_row2 = CLIP((298 * C + 516 * D + 128) >> 8, 0, 255);
					out_row2++;
					*out_row2 = CLIP((298 * C - 100 * D - 208 * E + 128) >> 8, 0, 255);
					out_row2++;
					*out_row2 = CLIP((298 * C + 409 * E + 128) >> 8, 0, 255);
					out_row2++;

					y = y22;

					C = y - 16;
					D = u - 128;
					E = v - 128;

					*out_row2 = CLIP((298 * C + 516 * D + 128) >> 8, 0, 255);
					out_row2++;
					*out_row2 = CLIP((298 * C - 100 * D - 208 * E + 128) >> 8, 0, 255);
					out_row2++;
					*out_row2 = CLIP((298 * C + 409 * E + 128) >> 8, 0, 255);
					out_row2++;
				}
			}
		}
		/*
		// this comparation can be optimized and moved 	in
		// if((pointerIncX>999)&&(pointerIncY>499))
		*/
		if (pointerIncY > 499)
			pointerIncY -= 500;
		/*
		// Use 2 rows only if
		// vertical ratio > 0.5 (increment is ratio*1000)
		*/
		if (ScaleIncY > 501) {
			out_row1 = out_row1 + incX;
			out_row2 = out_row2 + incX;
		}

		pointerIncY += ScaleIncY;
		bufUVYY = bufUVYY + width;
		bufY = bufY + 2 * width;
	}
}

/**
 *
 * @brief This function permits to convert an image from 6270 rawformat to RGB24
 * @brief The same function as convert to BGR24 but only B and R is change order
 * @param raw Buffer with the bayer data
 * @param width Width of image
 * @param height Height of image
 * @param hflip Horizontal flip
 * @param vflip Vertical flip
 *
 * @retval rgb Buffer with the rgb data
 *	Format of stream is uvyy,uvyy - repeated 320 times for 640x480
 *	y - repeated 640 times for 640x480
 *	First 1280 bytes is maybe dumy ***FIXME***
 */
void raw6270_2RGB24(uint8_t *raw, uint8_t *rgb, int width,
			int height, const int hflip,
			const int vflip)
{
	int i, j, incX;
/* Maybe is not necesery to use long variables but I not sure in this moment */
	long y11, y12, y21, y22, u, v, y, C, D, E;
	long ScaleIncX, ScaleIncY;
	long pointerIncX, pointerIncY;
	uint8_t *bufUVYY;
	uint8_t *bufY;
	uint8_t *out_row1;
	uint8_t *out_row2;

	/* For fast calculations */
	int x_div_2, y_div_2;
	/* Skip first 1280 bytes strange dummy bytes */
	raw += 1280;

	x_div_2 = width / 2;	/* for fast calculation if x=640 x_div_2=320 */
	y_div_2 = height / 2;	/* for fast calculation if y=480 y_div_4=240 */
	incX = 3 * width;
	bufUVYY = raw;
	bufY = raw + 2 * width;

	/*
	// Because I can't use float ratio is multiply by 1000 then
	// 1000 is equal of increment image (X or Y) with 1
	*/
	ScaleIncX = (1000 * width) / width;
	ScaleIncY = (1000 * height) / height;

	out_row1 = rgb;
	out_row2 = out_row1 + incX;
	/* we skipped 1280 bytes, it's almost two lines */
	pointerIncY = 1000;
	for (i = 0; i < y_div_2-1; i++) {
		pointerIncX = 1000;
		for (j = 0; j < x_div_2; j++) {
			pointerIncX += ScaleIncX;

			u = (unsigned char)(*bufUVYY);
			bufUVYY++;

			v = (unsigned char)(*bufUVYY);
			bufUVYY++;

			y11 = (unsigned char)(*bufUVYY);
			bufUVYY++;

			y12 = (unsigned char)(*bufUVYY);
			bufUVYY++;

			y21 = (unsigned char)(*bufY);
			bufY++;

			y22 = (unsigned char)(*bufY);
			bufY++;

			if ((pointerIncX > 999) && (pointerIncY > 499)) {
				pointerIncX -= 1001;
				y = y11;
				C = y - 16;
				D = u - 128;
				E = v - 128;

				*out_row1 = CLIP((298 * C + 409 * E + 128) >> 8, 0, 255);
				out_row1++;
				*out_row1 = CLIP((298 * C - 100 * D - 208 * E + 128) >> 8, 0, 255);
				out_row1++;
				*out_row1 = CLIP((298 * C + 516 * D + 128) >> 8, 0, 255);
				out_row1++;

				y = y12;

				C = y - 16;
				D = u - 128;
				E = v - 128;

				*out_row1 = CLIP((298 * C + 409 * E + 128) >> 8, 0, 255);
				out_row1++;
				*out_row1 = CLIP((298 * C - 100 * D - 208 * E + 128) >> 8, 0, 255);
				out_row1++;
				*out_row1 = CLIP((298 * C + 516 * D + 128) >> 8, 0, 255);
				out_row1++;

				/*
				// Second row of stream is displayed only
				// if image is greath than half
				*/
				if (ScaleIncY > 501) {
					y = y21;

					C = y - 16;
					D = u - 128;
					E = v - 128;

					*out_row2 = CLIP((298 * C + 409 * E + 128) >> 8, 0, 255);
					out_row2++;
					*out_row2 = CLIP((298 * C - 100 * D - 208 * E + 128) >> 8, 0, 255);
					out_row2++;
					*out_row2 = CLIP((298 * C + 516 * D + 128) >> 8, 0, 255);
					out_row2++;

					y = y22;

					C = y - 16;
					D = u - 128;
					E = v - 128;

					*out_row2 = CLIP((298 * C + 409 * E + 128) >> 8, 0, 255);
					out_row2++;
					*out_row2 = CLIP((298 * C - 100 * D - 208 * E + 128) >> 8, 0, 255);
					out_row2++;
					*out_row2 = CLIP((298 * C + 516 * D + 128) >> 8, 0, 255);
					out_row2++;
				}
			}
		}
		/*
		// this comparation can be optimized and moved 	in
		// if((pointerIncX>999)&&(pointerIncY>499))
		*/
		if (pointerIncY > 499)
			pointerIncY -= 500;
/* Use 2 rows only if vertical ratio > 0.5 (increment is ratio*1000) */
		if (ScaleIncY > 501) {
			out_row1 = out_row1 + incX;
			out_row2 = out_row2 + incX;
		}

		pointerIncY += ScaleIncY;
		bufUVYY = bufUVYY + width;
		bufY = bufY + 2 * width;
	}
}

