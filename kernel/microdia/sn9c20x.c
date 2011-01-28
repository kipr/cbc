/**
 * @file sn9c20x.c
 * @author Dave Neuer
 * @date 2008-03-02
 *
 * @brief Common functions and data for the Sonix SN9C20x webcam bridge chips.
 *
 * @note Copyright (C) Dave Neuer
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

#include <linux/delay.h>
#include <linux/errno.h>
#include <linux/string.h>
#include "microdia.h"
#include "sn9c20x.h"


struct sn9c20x_win_size {
	int hstart;
	int hsize;
	int vstart;
	int vsize;
	int scale;
} sn9c20x_win_sizes[] = {
	{
		.hstart = 0,
		.hsize = 160,
		.vstart = 0,
		.vsize = 120,
		.scale = SN9C20X_1_4_SCALE,
	},
	{
		.hstart = 0,
		.hsize = 320,
		.vstart = 0,
		.vsize = 240,
		.scale = SN9C20X_1_2_SCALE,
	},
	{
		.hstart = 0,
		.hsize = 640,
		.vstart = 0,
		.vsize = 480,
		.scale = SN9C20X_NO_SCALE,
	},
};

/**
 * @brief This function initializes the SN9C20x bridge, these are the bare minimum writes that must be done for the bridge to work correctly.
 *
 * @author Neekhil
 *
 * @param dev Pointer to the device
 *
 * @return Zero for success or error value
 *
 */
int sn9c20x_initialize(struct usb_microdia *dev)
{
	int ret;
	__u16 reg;
	__u8 buf[9];

	/* Could these be the bridge ADC controls, video stream seems jagged w/o this write */
	reg = 0x1188;
	buf[0] = 0x86;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	/* No video stream if we don't do this. Could be wake up Bridge from soft sleep/suspend? */
	reg = 0x11b8;
	buf[0] = 0x38;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	/* No video stream w/o this write */
	reg = 0x1001;
	buf[0] = 0x84;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	/* Some sort of Brightness control */
	reg = 0x118b;
	buf[0] = 0x0c;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	/* All 3 writes below are required else NO video stream */
	reg = 0x10e0;
	buf[0] = 0x4b;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;
	reg = 0x1180;
	buf[0] = 0x01; buf[1] = 0x00; buf[2] = 0x01; buf[3] = 0x00; buf[4] = 0x28;
	buf[5] = 0x3c;
	ret = usb_microdia_control_write(dev, reg, buf, 6);
	if (ret < 0)
		goto err;

	/* Some sort of brightness control */
	reg = 0x11ba;
	buf[0] = 0x0b;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	/* we get only green bands if we don't do this write */
	reg = 0x1002;
	buf[0] = 0x18;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	/* Bridge Auto-Exposure setting ? */
	reg = 0x118a;
	buf[0] = 0x04;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	/* Bridge Auto-Exposure setting ? */
	reg = 0x0395;
	buf[0] = 0x04;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x1061;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		return ret;

	buf[0] |= 0x02;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	/* These 2 writes finally start the video stream */
	reg = 0x1007;
	buf[0] = 0x60;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;
	reg = 0x1006;
	buf[0] = 0x40;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

        UDIA_INFO("Initialized !!! \n\n");
	return 0;

err:
	UDIA_ERROR("r/w in bridge register %x failed (%d)!\n", reg, ret);
	return ret;
}

/**
 * @brief Initializes Micro-Controller's I2C interface
 *
 * @author Neekhil
 *
 * @param dev Pointer to the device
 *
 * @return Zero (success) or negative (USB-error value)
 *
 */
int sn9c20x_i2c_initialize(struct usb_microdia *dev)
{
	__u8 buf[9];
	int ret;

	buf[0] = dev->sensor_flags;
	buf[1] = dev->sensor_slave_address;
	buf[2] = 0x00;
	buf[3] = 0x00;
	buf[4] = 0x00;
	buf[5] = 0x00;
	buf[6] = 0x00;
	buf[7] = 0x00;
	buf[8] = 0x03;
	/*  Initialize I2C registers to avoid getting no ACK at first I2C operation: */
	/* Get green diagonal bands w/o this dummy write, Bridge does not know sensor address ? */
	ret = usb_microdia_control_write(dev, 0x10c0, buf, 9);
	if (ret < 0)
		return ret;
	else
		return 0;
}

/**
 * @brief Wait until the I2C slave is ready for the next operation
 *
 * @param dev Pointer to the device
 * @param highspeed
 * @param slave_error
 *
 * @return Zero for success or a negative error value
 *
 */
int sn9c20x_i2c_ack_wait(struct usb_microdia *dev, int highspeed, int *slave_error)
{
	int ret, i;
	__u8 readbuf;
	int delay = highspeed ? 100 : 400;

	for (i = 0; i < 5; i++) {
		ret = usb_microdia_control_read(dev, 0x10c0, &readbuf, 1);

		if (ret < 0)
			return ret;
		else if (readbuf & SN9C20X_I2C_ERROR) {
			*slave_error = 1;
			/* probably should come up w/ an error value and
			 * return it via the error return */
			return 0;
		} else if (readbuf & SN9C20X_I2C_READY)
			return 0;
		else
			udelay(delay);
	}
	return -EBUSY;
}

/**
 * @brief Read up to 5 bytes of data from an I2C slave
 *
 * @param dev Pointer to the device
 * @param slave The id of the I2C slave device
 * @param nbytes Number of bytes to read
 * @param address The address of the register on the slave to read
 * @param flags The appropriate flags for bus speed and physical connection
 * @param result A pointer to the location at which the result should be stored
 *
 * @return Zero for success or a negative error value
 *
 */
int sn9c20x_read_i2c_data(struct usb_microdia *dev, __u8 slave, __u8 nbytes, __u8 address,
	__u8 flags, __u8 *result)
{
	int ret, i, j;
	__u8 row[5];

	if (!dev || nbytes > 4)
		return -EINVAL;

	/* first, we must do a dummy write of just the address */
	ret = sn9c20x_write_i2c_data(dev, slave, 0, address, flags, NULL);
	if (ret < 0)
		return ret;

	memset(row, 0, 5);
	/* now we issue the same command but with the read bit set
	 * and no slave register address */
	ret = sn9c20x_write_i2c_data(dev, slave, nbytes - 1, 0, flags |
		SN9C20X_I2C_READ, row);
	if (ret < 0)
		return ret;

	/* finally, ask the bridge for the data */
	ret = usb_microdia_control_read(dev, 0x10c2, row, 5);
	if (ret < 0)
		return ret;

	for (i = 0, j = 5 - nbytes; i < nbytes; i++, j++)
		result[i] = row[j];

	return 0;
}

/**
 * @brief Read up to 4 bytes of data from an I2C slave an return them as 16bit values
 *
 * @param dev Pointer to the device
 * @param slave The id of the I2C slave device
 * @param datalen Number of 16bit values to read
 * @param address The address of the register on the slave to read
 * @param flags The appropriate flags for bus speed and physical connection
 * @param result A pointer to the location at which the result should be stored
 *
 * @return Zero for success or a negative error value
 *
 */
int sn9c20x_read_i2c_data16(struct usb_microdia *dev, __u8 slave, __u8 datalen, __u8 address,
	__u8 flags, __u16 *result)
{
	__u8 result8[4];
	__u8 k;
	int ret;

	if (datalen > 2)
		return -EINVAL;
	ret = sn9c20x_read_i2c_data(dev, slave, 2*datalen, address, flags, result8);
	for (k = 0; k < datalen; k++)
		result[k] = (result8[k*2] << 8) | result8[k*2+1];
	return ret;
}

static const char *wasread = "read from";
static const char *waswrite = "write to";

/**
 * @brief Write up to 5 bytes of data to an I2C slave
 *
 * @param dev Pointer to the device
 * @param slave The id of the I2C slave device
 * @param nbytes The number of bytes of data
 * @param address The address of the register on the slave to write
 * @param flags The appropriate flags for bus speed and physical connection
 * @param data An array containing the data to write
 *
 * @return Zero for success or a negative error value
 *
 */
int sn9c20x_write_i2c_data(struct usb_microdia *dev, __u8 slave, __u8 nbytes,
	__u8 address, __u8 flags, const __u8 data[nbytes])
{
	int ret, i;
	__u8 row[8];
	int slave_error = 0;

	if (!dev || (nbytes > 0 && !data) || nbytes > 4)
		return -EINVAL;

        switch (nbytes) {
        case 1:
          UDIA_DEBUG("I2C write(slave=%02X, addr=%02X, data=%02X)\n", slave, address, data[0]);
          break;
        case 2:
          UDIA_DEBUG("I2C write(slave=%02X, addr=%02X, data=%02X %02X)\n", slave, address, data[0], data[1]);
          break;
        case 3:
          UDIA_DEBUG("I2C write(slave=%02X, addr=%02X, data=%02X %02X %02X)\n", slave, address, data[0], data[1], data[2]);
          break;
        case 4:
          UDIA_DEBUG("I2C write(slave=%02X, addr=%02X, data=%02X %02X %02X %02X)\n", slave, address, data[0], data[1], data[2], data[3]);
          break;
        default:
          UDIA_DEBUG("I2C write(slave=%02X, addr=%02X)\n", slave, address);
          break;
        }
        
	/* from the point of view of the bridge, the length
	 * includes the address */
	row[0] = flags | ((nbytes + 1) << 4);
	row[1] = slave;
	row[2] = address;
	row[7] = 0x10; /* I think this means we want an ack */

	for (i = 0; i < 4; i++)
		row[i + 3] = i < nbytes ? data[i] : 0;

//	UDIA_DEBUG("I2C %s %02x: %02x %02x %02x %02x %02x %02x %02x %02x\n",
//		(flags & SN9C20X_I2C_READ ? wasread : waswrite), address,
//		row[0], row[1], row[2], row[3], row[4], row[5], row[6], row[7]);

	ret = usb_microdia_control_write(dev, 0x10c0, row, 8);
	if (ret >= 0)
                ret = sn9c20x_i2c_ack_wait(dev, flags & SN9C20X_I2C_400KHZ, &slave_error);

	if (slave_error) {
		UDIA_ERROR("I2C slave 0x%02x returned error during %s address 0x%02x\n",
			slave,
			(flags & SN9C20X_I2C_READ ? wasread : waswrite),
			address);
		return -1000;
		/* there should be no interference with USB errors */
	}

	if (ret < 0) {
		/* we got no ack */
		UDIA_ERROR("No ack from I2C slave 0x%02x for %s address 0x%02x\n",
			slave,
			(flags & SN9C20X_I2C_READ ? wasread : waswrite),
			address);
		return ret;
	}

	return 0;
}

/**
 * @brief Write up to 2 16bit values als single bytes to an I2C slave
 *
 * @param dev Pointer to the device
 * @param slave The id of the I2C slave device
 * @param datalen The number of 16bit data values to write
 * @param address The address of the register on the slave to write
 * @param flags The appropriate flags for bus speed and physical connection
 * @param data An array containing the data to write
 *
 * @return Zero for success or a negative error value
 *
 */
int sn9c20x_write_i2c_data16(struct usb_microdia *dev, __u8 slave, __u8 datalen,
	__u8 address, __u8 flags, const __u16 data[datalen])
{
	__u8 data8[4];
	__u8 k;
	int ret;

	if (datalen > 2)
		return -EINVAL;
	for (k = 0; k < datalen; k++) {
		data8[k*2] = data[k] >> 8;
		data8[k*2+1] = data[k] & 0xff;
	}
	ret = sn9c20x_write_i2c_data(dev, slave, 2*datalen, address, flags, data8);
	return ret;
}

/**
 * @brief Set contrast inside sn9c20x chip
 *
 * @author Comer352l
 *
 * @param dev Pointer to the device
 *
 * @return Zero (success) or negative (USB-error value)
 *
 */
int sn9c20x_set_contrast(struct usb_microdia *dev)
{
	/* from 0x26 to 0x4b */
	__u8 brightness_contrast[21] = {0x16, 0x0, 0x2b, 0x0, 0x8, 0x0, 0xf6, 0x0f,
				0xd2, 0x0f, 0x38, 0x0, 0x34, 0x0, 0xcf, 0x0f,
				0xfd, 0x0f, 0x0, 0x0, 0x0};
	__u8 contrast_val = (dev->vsettings.contrast >> 8) * 0x25 / 0x100;
	__u8 brightness_val = dev->vsettings.brightness >> 8;

	brightness_val -= 0x80;
	brightness_contrast[18] = brightness_val;

	contrast_val += 0x26;
	brightness_contrast[2] = contrast_val;
	brightness_contrast[0] = 0x13 + (brightness_contrast[2] - 0x26) * 0x13 / 0x25;
	brightness_contrast[4] = 0x7 + (brightness_contrast[2] - 0x26) * 0x7 / 0x25;

	return usb_microdia_control_write(dev, 0x10e1, brightness_contrast, 21);
}

/**
 * @brief Set brightness inside sn9c20x chip
 *
 * @author Comer352l
 *
 * @param dev Pointer to the device
 *
 * @return Zero (success) or negative (USB-error value)
 *
 * Wrapper for sn9c20x_set_contrast
 *
 */
int sn9c20x_set_brightness(struct usb_microdia *dev)
{
	return dev_microdia_camera_set_contrast(dev);
}

/**
 * @brief Set gamma inside sn9c20x chip
 *
 * @author Comer352l
 *
 * @param dev Pointer to the device
 *
 * @return Zero (success) or negative (USB-error value)
 *
 */
int sn9c20x_set_gamma(struct usb_microdia *dev)
{
	int value = (dev->vsettings.whiteness >> 8) * 0xb8 / 0x100;
	int r = 0;

	__u8 gamma_val[17] = {0x0a, 0x13, 0x25, 0x37, 0x45, 0x55, 0x65, 0x74,
			0x83, 0x92, 0xa1, 0xb0, 0xbf, 0xce, 0xdf, 0xea, 0xf5};

	gamma_val[0] = 0x0a;
	gamma_val[1] = 0x13 + (value * (0xcb - 0x13) / 0xb8);
	gamma_val[2] = 0x25 + (value * (0xee - 0x25) / 0xb8);
	gamma_val[3] = 0x37 + (value * (0xfa - 0x37) / 0xb8);
	gamma_val[4] = 0x45 + (value * (0xfc - 0x45) / 0xb8);
	gamma_val[5] = 0x55 + (value * (0xfb - 0x55) / 0xb8);
	gamma_val[6] = 0x65 + (value * (0xfc - 0x65) / 0xb8);
	gamma_val[7] = 0x74 + (value * (0xfd - 0x74) / 0xb8);
	gamma_val[8] = 0x83 + (value * (0xfe - 0x83) / 0xb8);
	gamma_val[9] = 0x92 + (value * (0xfc - 0x92) / 0xb8);
	gamma_val[10] = 0xa1 + (value * (0xfc - 0xa1) / 0xb8);
	gamma_val[11] = 0xb0 + (value * (0xfc - 0xb0) / 0xb8);
	gamma_val[12] = 0xbf + (value * (0xfb - 0xbf) / 0xb8);
	gamma_val[13] = 0xce + (value * (0xfb - 0xce) / 0xb8);
	gamma_val[14] = 0xdf + (value * (0xfd - 0xdf) / 0xb8);
	gamma_val[15] = 0xea + (value * (0xf9 - 0xea) / 0xb8);
	gamma_val[16] = 0xf5;

	r = usb_microdia_control_write(dev, 0x1190, gamma_val, 17);

	return r;
}

/**
 * @brief Set sharpness inside sn9c20x chip
 *
 * @author Comer352l
 *
 * @param dev Pointer to the device
 *
 * @return Zero (success) or negative (USB-error value)
 *
 */
int sn9c20x_set_sharpness(struct usb_microdia *dev)
{
	__u8 val[1];
	int ret;

	ret = usb_microdia_control_read(dev, SN9C20X_SHARPNESS, val, 1);
	if (ret < 0)
		return ret;
	val[0] = (val[0] & 0xc0) | (dev->vsettings.sharpness & 0x3f);
	ret = usb_microdia_control_write(dev, SN9C20X_SHARPNESS, val, 1);
	if (ret < 0)
		return ret;
	else
		return 0;
}

/**
 * @brief Set colour gain inside sn9c20x chip
 *
 * @author Brian Johnson
 *
 * @param dev Pointer to the device
 *
 * @return Zero (success) or negative (USB-error value)
 *
 */
int sn9c20x_set_rgb_gain(struct usb_microdia *dev)
{
	__u8 val[4];
	int ret;

	memcpy(&val, &(dev->vsettings.rgb_gain), 4);
	ret = usb_microdia_control_write(dev, SN9C20X_RED_GAIN, val, 4);
	if (ret < 0)
		return ret;
	else
		return 0;
}

/**
 * @brief Calculate closest resolution to input from application
 *
 * @author Brian Johnson
 *
 * @param width Requested width of video stream
 * @param height Requested height of video stream
 *
 * @retval width Closest possible width of video stream
 * @retval height Closest possible height of video stream
 *
 * @return Number of the
 *
 */
int sn9c20x_get_closest_resolution(int *width, int *height)
{
	int i;

	for (i = ARRAY_SIZE(sn9c20x_win_sizes) - 1; i >= 0; i--) {
                if (*width >= sn9c20x_win_sizes[i].hsize && *height >= sn9c20x_win_sizes[i].vsize)
			break;
	}

	*width = sn9c20x_win_sizes[i].hsize;
	*height = sn9c20x_win_sizes[i].vsize;

	return i;
}

/**
 * @brief Set resolution inside sn9c20x chip
 *
 * @author Brian Johnson
 *
 * @param dev Pointer to the device
 * @param width Width
 * @param height Height
 *
 * @return 0
 *
 */
int sn9c20x_set_resolution(struct usb_microdia *dev, int width, int height)
{
	int ret;
	__u8 scale;
	__u8 window[5];
	struct sn9c20x_win_size *wsize;

	ret = sn9c20x_get_closest_resolution(&width, &height);
	wsize = &sn9c20x_win_sizes[ret];

	dev->vsettings.format.width = width;
	dev->vsettings.format.height = height;

	window[0] = wsize->hstart >> 2; window[1] = wsize->hsize >> 2;
	window[2] = wsize->vstart >> 1; window[3] = wsize->vsize >> 1;
	window[4] = 0x00;

	scale = wsize->scale;

	usb_microdia_control_write(dev, SN9C20X_HSTART, window, 5);
	usb_microdia_control_write(dev, SN9C20X_SCALE, &scale, 1);

	/*
	 * queue.frame_size is only ever used in isoc_handler
	 * to test if the ISOC data is the correct size
	 * This modificate pre calculates this rather
	 * than doing the calculating in the isoc_handler
	 */
	dev->queue.frame_size = width * height +
		(width * height) / dev->frame_size_divisor;


	UDIA_DEBUG("Set mode [%dx%d]\n", width, height);

	return 0;
}

/**
 * @brief Set exposure inside sn9c20x chip
 *
 * @param dev
 *
 * @returns 0 or negative error value
 *
 * @author GWater
 */
int sn9c20x_set_exposure(struct usb_microdia *dev)
{
	__u8 buf;
	int ret;
	int exposure = dev->vsettings.exposure;

	buf = (__u8) (exposure  * 0x00ff / 0xffff) & 0x00ff;

	/* exposure can't be 0 - below 0x04 the image freezes */
	if (buf < 0x05)
		buf = 0x05;

	/* write new value to register 0x118a */
	ret = usb_microdia_control_write(dev, 0x118a, &buf, 1);
	if (ret < 0) {
		UDIA_ERROR("Error: setting exposure failed: "
			"error while writing to register 0x118a\n");
		return ret;
	}

	return 0;
}
