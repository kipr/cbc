/**
 * @file microdia-sysfs.c
 * @author Nicolas VIVIEN
 * @date 2008-02-01
 *
 * @brief Sysfs interface for image settings
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
#include <linux/device.h>

#include <linux/usb.h>
#include <media/v4l2-common.h>

#include "microdia.h"
#include "sn9c20x.h"


/**
 * @brief show_release
 *
 * @param class Class device
 * @param attr
 * @retval buf Adress of buffer with the 'release' value
 *
 * @returns Size of buffer
 */
static ssize_t show_release(struct class_device *class, char *buf)
{
	struct video_device *vdev = to_video_device(class);
	struct usb_microdia *dev = video_get_drvdata(vdev);

	return sprintf(buf, "%d\n", dev->release);
}


/**
 * @brief show_videostatus
 *
 * @param class Class device
 * @param attr
 * @retval buf Adress of buffer with the 'videostatus' value
 *
 * @returns Size of buffer
 */
static ssize_t show_videostatus(struct class_device *class, char *buf)
{
	struct video_device *vdev = to_video_device(class);
	struct usb_microdia *dev = video_get_drvdata(vdev);

	return sprintf(buf,
		"Overflow frames    : %d\n"
		"Incomplete frames  : %d\n"
		"Dropped frames     : %d\n",
		dev->vframes_overflow,
		dev->vframes_incomplete,
		dev->vframes_dropped);
}


/**
 * @brief show_informations
 *
 * @param class Class device
 * @param attr
 * @retval buf Adress of buffer with the 'informations' value
 *
 * @returns Size of buffer
 */
static ssize_t show_informations(struct class_device *class, char *buf)
{
	char *pixelfmt = NULL;

	struct video_device *vdev = to_video_device(class);
	struct usb_microdia *dev = video_get_drvdata(vdev);

	char *palette_bgr24 = "BGR24 - BGR-8-8-8 - 24 bits";
	char *palette_rgb24 = "RGB24 - RGB-8-8-8 - 24 bits";
	char *palette_yuyv = "YUYV - YUV 4:2:2 - 16 bits";
	char *palette_i420 = "I420 - YUV 4:2:0 - 12 bits";


	switch (dev->vsettings.format.pixelformat) {
	case V4L2_PIX_FMT_BGR24:
		pixelfmt = palette_bgr24;
		break;

	case V4L2_PIX_FMT_RGB24:
		pixelfmt = palette_rgb24;
		break;

	case V4L2_PIX_FMT_YUYV:
		pixelfmt = palette_yuyv;
		break;

	case V4L2_PIX_FMT_YUV420:
		pixelfmt = palette_i420;
		break;
	}

	return sprintf(buf,
			"Driver Resolution : %dx%d\n"
			"\n"
			"%s\n"
			"\n"
			"Brightness        : 0x%X\n"
			"Contrast          : 0x%X\n"
			"Whiteness         : 0x%X\n"
			"Exposure          : 0x%X\n"
			"Sharpness         : 0x%X\n"
			"Horizontal flip   : %d\n"
			"Vertical flip     : %d\n"
			"Auto-exposure     : %d\n"
			"Auto-whitebalance : %d\n",
			dev->vsettings.format.width,
			dev->vsettings.format.height,
			pixelfmt,
			0xFFFF & dev->vsettings.brightness,
			0xFFFF & dev->vsettings.contrast,
			0xFFFF & dev->vsettings.whiteness,
			0xFFFF & dev->vsettings.exposure,
			0x3F & dev->vsettings.sharpness,
			dev->vsettings.hflip,
			dev->vsettings.vflip,
			dev->vsettings.auto_exposure,
			dev->vsettings.auto_whitebalance);
}


/**
 * @brief show_fps
 *
 * @param class Class device
 * @param attr
 * @retval buf Adress of buffer with the 'fps' value
 *
 * @returns Size of buffer
 */
static ssize_t show_fps(struct class_device *class, char *buf)
{
	struct video_device *vdev = to_video_device(class);
	struct usb_microdia *dev = video_get_drvdata(vdev);

	return sprintf(buf, "%d\n", dev->vsettings.fps);
}

/**
 * @brief show_rgb_gain
 *
 * @param class Class device
 * @param attr
 * @retval buf Address of buffer with the 'rgb_gain' value
 *
 * @returns Size of buffer
 */
static ssize_t show_rgb_gain(struct class_device *class, char *buf)
{
	int rgb_gain = 0;

	struct video_device *vdev = to_video_device(class);
	struct usb_microdia *dev = video_get_drvdata(vdev);
	rgb_gain |= dev->vsettings.rgb_gain[0] << 16;
	rgb_gain |= dev->vsettings.rgb_gain[1] << 8;
	rgb_gain |= dev->vsettings.rgb_gain[3];
	return sprintf(buf, "0x%06X\n", rgb_gain);
}

/**
 * @brief store_exposure
 *
 * @param class Class device
 * @param buf Buffer
 * @param count Counter
 * @param attr
 *
 * @returns Size of buffer
 */
static ssize_t store_rgb_gain(struct class_device *class,
	const char *buf, size_t count)
{
	char *endp;
	unsigned long rgb_gain;

	struct video_device *vdev = to_video_device(class);
	struct usb_microdia *dev = video_get_drvdata(vdev);

	rgb_gain = simple_strtoul(buf, &endp, 16);

	dev->vsettings.rgb_gain[0] = (rgb_gain >> 16) & 0x7f;
	dev->vsettings.rgb_gain[1] = (rgb_gain >> 8) & 0x7f;
	dev->vsettings.rgb_gain[2] = (rgb_gain >> 8) & 0x7f;
	dev->vsettings.rgb_gain[3] = rgb_gain & 0x7f;

	dev_microdia_camera_set_rgb_gain(dev);

	return strlen(buf);
}

/**
 * @brief show_exposure
 *
 * @param class Class device
 * @param attr
 * @retval buf Adress of buffer with the 'exposure' value
 *
 * @returns Size of buffer
 */
static ssize_t show_exposure(struct class_device *class, char *buf)
{
	struct video_device *vdev = to_video_device(class);
	struct usb_microdia *dev = video_get_drvdata(vdev);

	return sprintf(buf, "%X\n", dev->vsettings.exposure);
}

/**
 * @brief store_exposure
 *
 * @param class Class device
 * @param buf Buffer
 * @param count Counter
 * @param attr
 *
 * @returns Size of buffer
 */
static ssize_t store_exposure(struct class_device *class, const char *buf, size_t count)
{
	char *endp;
	unsigned long value;

	struct video_device *vdev = to_video_device(class);
	struct usb_microdia *dev = video_get_drvdata(vdev);

	value = simple_strtoul(buf, &endp, 16);

	dev->vsettings.exposure = (int) value;

	dev_microdia_camera_set_exposure(dev);

	return strlen(buf);
}

/**
 * @brief show_brightness
 *
 * @param class Class device
 * @param attr
 * @retval buf Adress of buffer with the 'brightness' value
 *
 * @returns Size of buffer
 */
static ssize_t show_brightness(struct class_device *class, char *buf)
{
	struct video_device *vdev = to_video_device(class);
	struct usb_microdia *dev = video_get_drvdata(vdev);

	return sprintf(buf, "%X\n", dev->vsettings.brightness);
}

/**
 * @brief store_brightness
 *
 * @param class Class device
 * @param buf Buffer
 * @param count Counter
 * @param attr
 *
 * @returns Size of buffer
 */
static ssize_t store_brightness(struct class_device *class, const char *buf, size_t count)
{
	char *endp;
	unsigned long value;

	struct video_device *vdev = to_video_device(class);
	struct usb_microdia *dev = video_get_drvdata(vdev);

	value = simple_strtoul(buf, &endp, 16);

	dev->vsettings.brightness = (int) value;

	dev_microdia_camera_set_brightness(dev);

	return strlen(buf);
}

/**
 * @brief show_contrast
 *
 * @param class Class device
 * @param attr
 * @retval buf Adress of buffer with the 'contrast' value
 *
 * @returns Size of buffer
 */
static ssize_t show_contrast(struct class_device *class, char *buf)
{
	struct video_device *vdev = to_video_device(class);
	struct usb_microdia *dev = video_get_drvdata(vdev);

	return sprintf(buf, "%X\n", dev->vsettings.contrast);
}


/**
 * @brief store_contrast
 *
 * @param class Class device
 * @param buf Buffer
 * @param count Counter
 * @param attr
 *
 * @returns Size of buffer
 */
static ssize_t store_contrast(struct class_device *class, const char *buf, size_t count)
{
	char *endp;
	unsigned long value;

	struct video_device *vdev = to_video_device(class);
	struct usb_microdia *dev = video_get_drvdata(vdev);

	value = simple_strtoul(buf, &endp, 16);

	dev->vsettings.contrast = (int) value;

	dev_microdia_camera_set_contrast(dev);
/*	dev_microdia_set_camera_quality(dev); */

	return strlen(buf);
}


/**
 * @brief show_whitebalance
 *
 * @param class Class device
 * @param attr
 * @retval buf Adress of buffer with the 'whitebalance' value
 *
 * @returns Size of buffer
 */
static ssize_t show_whitebalance(struct class_device *class, char *buf)
{
	struct video_device *vdev = to_video_device(class);
	struct usb_microdia *dev = video_get_drvdata(vdev);

	return sprintf(buf, "%X\n", dev->vsettings.whiteness);
}


/**
 * @brief store_whitebalance
 *
 * @param class Class device
 * @param buf Buffer
 * @param count Counter
 * @param attr
 *
 * @returns Size of buffer
 */
static ssize_t store_whitebalance(struct class_device *class, const char *buf, size_t count)
{
	char *endp;
	unsigned long value;

	struct video_device *vdev = to_video_device(class);
	struct usb_microdia *dev = video_get_drvdata(vdev);

	value = simple_strtoul(buf, &endp, 16);

	dev->vsettings.whiteness = (int) value;

	dev_microdia_camera_set_gamma(dev);
/*	dev_microdia_set_camera_quality(dev); */

	return strlen(buf);
}


/*
 * @brief show_colour
 *
 * @param class Class device
 * @param attr
 * @retval buf Adress of buffer with the 'colour' value
 *
 * @returns Size of buffer
 */
/*
static ssize_t show_colour(struct class_device *class, char *buf)
{
	struct video_device *vdev = to_video_device(class);
	struct usb_microdia *dev = video_get_drvdata(vdev);

	return sprintf(buf, "%X\n", dev->vsettings.colour);
}
*/

/*
 * @brief store_colour
 *
 * @param class Class device
 * @param buf Buffer
 * @param count Counter
 * @param attr
 *
 * @returns Size of buffer
 */
/*
static ssize_t store_colour(struct class_device *class, const char *buf, size_t count)
{
	char *endp;
	unsigned long value;

	struct video_device *vdev = to_video_device(class);
	struct usb_microdia *dev = video_get_drvdata(vdev);

	value = simple_strtoul(buf, &endp, 16);

	dev->vsettings.colour = (int) value;

/	dev_microdia_set_camera_quality(dev); /

	return strlen(buf);
}
*/


/**
 * @brief show_sharpness
 *
 * @param class Class device
 * @param attr
 * @retval buf Adress of buffer with the 'sharpness' value
 *
 * @returns Size of buffer
 */
static ssize_t show_sharpness(struct class_device *class, char *buf)
{
	struct video_device *vdev = to_video_device(class);
	struct usb_microdia *dev = video_get_drvdata(vdev);

	return sprintf(buf, "%X\n", dev->vsettings.sharpness);
}

/**
 * @brief store_sharpness
 *
 * @param class Class device
 * @param buf Buffer
 * @param count Counter
 * @param attr
 *
 * @returns Size of buffer
 */
static ssize_t store_sharpness(struct class_device *class, const char *buf, size_t count)
{
	char *endp;
	unsigned long value;

	struct video_device *vdev = to_video_device(class);
	struct usb_microdia *dev = video_get_drvdata(vdev);

	value = simple_strtoul(buf, &endp, 16);

	if (value < 0 || value > 0x3f)
		return -EINVAL;

	dev->vsettings.sharpness = (int) value;

	dev_microdia_camera_set_sharpness(dev);

	return strlen(buf);
}


/**
 * @brief show_hflip
 *
 * @param class Class device
 * @param attr
 * @retval buf Adress of buffer with the 'hflip' value
 *
 * @returns Size of buffer
 */
static ssize_t show_hflip(struct class_device *class, char *buf)
{
	struct video_device *vdev = to_video_device(class);
	struct usb_microdia *dev = video_get_drvdata(vdev);

	return sprintf(buf, "%d\n", dev->vsettings.hflip);
}


/**
 * @brief store_hflip
 *
 * @param class Class device
 * @param buf Buffer
 * @param count Counter
 * @param attr
 *
 * @returns Size of buffer
 */
static ssize_t store_hflip(struct class_device *class, const char *buf, size_t count)
{
	struct video_device *vdev = to_video_device(class);
	struct usb_microdia *dev = video_get_drvdata(vdev);

	if (strncmp(buf, "1", 1) == 0)
		dev->vsettings.hflip = 1;
	else if (strncmp(buf, "0", 1) == 0)
		dev->vsettings.hflip = 0;
	else
		return -EINVAL;

	dev_microdia_camera_set_hvflip(dev);

	return strlen(buf);
}


/**
 * @brief show_vflip
 *
 * @param class Class device
 * @param attr
 * @retval buf Adress of buffer with the 'vflip' value
 *
 * @returns Size of buffer
 */
static ssize_t show_vflip(struct class_device *class, char *buf)
{
	struct video_device *vdev = to_video_device(class);
	struct usb_microdia *dev = video_get_drvdata(vdev);

	return sprintf(buf, "%d\n", dev->vsettings.vflip);
}


/**
 * @brief store_vflip
 *
 * @param class Class device
 * @param buf Buffer
 * @param count Counter
 * @param attr
 *
 * @returns Size of buffer
 */
static ssize_t store_vflip(struct class_device *class, const char *buf, size_t count)
{
	struct video_device *vdev = to_video_device(class);
	struct usb_microdia *dev = video_get_drvdata(vdev);

	if (strncmp(buf, "1", 1) == 0)
		dev->vsettings.vflip = 1;
	else if (strncmp(buf, "0", 1) == 0)
		dev->vsettings.vflip = 0;
	else
		return -EINVAL;

	dev_microdia_camera_set_hvflip(dev);

	return strlen(buf);
}


/**
 * @brief show_autoexposure
 *
 * @param class Class device
 * @param attr
 * @retval buf Adress of buffer with the 'hflip' value
 *
 * @returns Size of buffer
 */
static ssize_t show_autoexposure(struct class_device *class, char *buf)
{
	struct video_device *vdev = to_video_device(class);
	struct usb_microdia *dev = video_get_drvdata(vdev);

	return sprintf(buf, "%d\n", dev->vsettings.auto_exposure);
}


/**
 * @brief store_autoexposure
 *
 * @param class Class device
 * @param buf Buffer
 * @param count Counter
 * @param attr
 *
 * @returns Size of buffer
 */
static ssize_t store_autoexposure(struct class_device *class, const char *buf, size_t count)
{
	struct video_device *vdev = to_video_device(class);
	struct usb_microdia *dev = video_get_drvdata(vdev);

	if (strncmp(buf, "1", 1) == 0)
		dev->vsettings.auto_exposure = 1;
	else if (strncmp(buf, "0", 1) == 0)
		dev->vsettings.auto_exposure = 0;
	else
		return -EINVAL;

	dev_microdia_camera_set_auto_exposure(dev);

	return strlen(buf);
}


/**
 * @brief show_autowhitebalance
 *
 * @param class Class device
 * @param attr
 * @retval buf Adress of buffer with the 'vflip' value
 *
 * @returns Size of buffer
 */
static ssize_t show_autowhitebalance(struct class_device *class, char *buf)
{
	struct video_device *vdev = to_video_device(class);
	struct usb_microdia *dev = video_get_drvdata(vdev);

	return sprintf(buf, "%d\n", dev->vsettings.auto_whitebalance);
}


/**
 * @brief store_autowhitebalance
 *
 * @param class Class device
 * @param buf Buffer
 * @param count Counter
 * @param attr
 *
 * @returns Size of buffer
 */
static ssize_t store_autowhitebalance(struct class_device *class, const char *buf, size_t count)
{
	struct video_device *vdev = to_video_device(class);
	struct usb_microdia *dev = video_get_drvdata(vdev);

	if (strncmp(buf, "1", 1) == 0)
		dev->vsettings.auto_whitebalance = 1;
	else if (strncmp(buf, "0", 1) == 0)
		dev->vsettings.auto_whitebalance = 0;
	else
		return -EINVAL;

	dev_microdia_camera_set_auto_whitebalance(dev);

	return strlen(buf);
}

static CLASS_DEVICE_ATTR(release, S_IRUGO, show_release, NULL);							/**< Release value */
static CLASS_DEVICE_ATTR(videostatus, S_IRUGO, show_videostatus, NULL);						/**< Video status */
static CLASS_DEVICE_ATTR(informations, S_IRUGO, show_informations, NULL);					/**< Informations */
static CLASS_DEVICE_ATTR(fps, S_IRUGO, show_fps, NULL);								/**< FPS value */
static CLASS_DEVICE_ATTR(brightness, S_IRUGO | S_IWUGO, show_brightness, store_brightness);			/**< Brightness value */
static CLASS_DEVICE_ATTR(rgb_gain, S_IRUGO | S_IWUGO, show_rgb_gain, store_rgb_gain);				/**< RGB Gain */
static CLASS_DEVICE_ATTR(exposure, S_IRUGO | S_IWUGO, show_exposure, store_exposure);				/**< Brightness exposure */
static CLASS_DEVICE_ATTR(contrast, S_IRUGO | S_IWUGO, show_contrast, store_contrast);				/**< Contrast value */
static CLASS_DEVICE_ATTR(whitebalance, S_IRUGO | S_IWUGO, show_whitebalance, store_whitebalance);		/**< Whitebalance value */
static CLASS_DEVICE_ATTR(sharpness, S_IRUGO | S_IWUGO, show_sharpness, store_sharpness);			/**< Sharpness value */
/*static CLASS_DEVICE_ATTR(colour, S_IRUGO | S_IWUGO, show_colour, store_colour);				/< Hue value */
static CLASS_DEVICE_ATTR(hflip, S_IRUGO | S_IWUGO, show_hflip, store_hflip);					/**< Horizontal flip value */
static CLASS_DEVICE_ATTR(vflip, S_IRUGO | S_IWUGO, show_vflip, store_vflip);					/**< Vertical flip value */
static CLASS_DEVICE_ATTR(auto_exposure, S_IRUGO | S_IWUGO, show_autoexposure, store_autoexposure);		/**< Automatic exposure control value */
static CLASS_DEVICE_ATTR(auto_whitebalance, S_IRUGO | S_IWUGO, show_autowhitebalance, store_autowhitebalance);	/**< Automatic whitebalance control value */

/**
 * @brief Create the 'sys' entries.
 *
 * This function permits to create all the entries in the 'sys' filesystem.
 *
 * @param vdev Video device structure
 *
 * @returns 0 if all is OK
 */
int microdia_create_sysfs_files(struct video_device *vdev)
{

	video_device_create_file(vdev, &class_device_attr_release);
	video_device_create_file(vdev, &class_device_attr_videostatus);
	video_device_create_file(vdev, &class_device_attr_informations);
	video_device_create_file(vdev, &class_device_attr_fps);
	video_device_create_file(vdev, &class_device_attr_brightness);
	video_device_create_file(vdev, &class_device_attr_rgb_gain);
	video_device_create_file(vdev, &class_device_attr_exposure);
	video_device_create_file(vdev, &class_device_attr_contrast);
	video_device_create_file(vdev, &class_device_attr_whitebalance);
	video_device_create_file(vdev, &class_device_attr_sharpness);
	video_device_create_file(vdev, &class_device_attr_hflip);
	video_device_create_file(vdev, &class_device_attr_vflip);
	video_device_create_file(vdev, &class_device_attr_auto_exposure);
	video_device_create_file(vdev, &class_device_attr_auto_whitebalance);
	return 0;
}


/**
 * @brief Remove the 'sys' entries.
 *
 * This function permits to remove all the entries in the 'sys' filesystem.
 *
 * @param vdev Video device structure
 *
 */
void microdia_remove_sysfs_files(struct video_device *vdev)
{
	video_device_remove_file(vdev, &class_device_attr_release);
	video_device_remove_file(vdev, &class_device_attr_videostatus);
	video_device_remove_file(vdev, &class_device_attr_informations);
	video_device_remove_file(vdev, &class_device_attr_fps);
	video_device_remove_file(vdev, &class_device_attr_brightness);
	video_device_remove_file(vdev, &class_device_attr_exposure);
	video_device_remove_file(vdev, &class_device_attr_rgb_gain);
	video_device_remove_file(vdev, &class_device_attr_contrast);
	video_device_remove_file(vdev, &class_device_attr_whitebalance);
	video_device_remove_file(vdev, &class_device_attr_sharpness);
	/*video_device_remove_file(vdev, &class_device_attr_colour);*/
	video_device_remove_file(vdev, &class_device_attr_hflip);
	video_device_remove_file(vdev, &class_device_attr_vflip);
	video_device_remove_file(vdev, &class_device_attr_auto_exposure);
	video_device_remove_file(vdev, &class_device_attr_auto_whitebalance);}

