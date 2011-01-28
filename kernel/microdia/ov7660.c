/**
 * @file ov7660.c
 * @author Neekhil
 * @date 2008-08-24
 *
 * @brief Common control functions for the Omnivision OV7660 series Image Sensors.
 *
 * @note Copyright (C) Neekhil
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

#include "microdia.h"
#include "sn9c20x.h"
#include "ov7660.h"

int ov7660_sleep(struct usb_microdia *dev);
int ov7660_wakeup(struct usb_microdia *dev);
int ov7660_reset(struct usb_microdia *dev);

static __u8 ov7660_init[][2] = {
	/* System CLK selection, to get a higher Frame Rate */
	{OV7660_CTL_COM5, 0x80},
	/* OV7660 Wakeup */
	/* COM4 is Reserved : using default value 0x40 OR windows driver value 0x08 */
	{OV7660_CTL_COM4, 0x08},
	/* Enable HREF at optical black, Use optical black line as BLC signal
	    Reset all timing when format changes, Enable ADBLC option */
	{OV7660_CTL_COM6, 0xc3},
	/* windows 0x00, default 0x00, trying 0x60 to enable CCIR656 format */
	{OV7660_CTL_COM1, 0xc3},
	/* default is 0x40, windows sets it to 0x00 */
	{OV7660_CTL_AECH, 0x40},
	/* default is 0x00, windows sets it to 0x40 */
	{OV7660_CTL_CLKRC, 0x40},
	/* Set O/P format - RGB Selection, Set O/P format Raw RGB */
	{OV7660_CTL_COM7, 0x05},
	/* default is 0x8f, windows used 0xf8 */
	/* Enable fast AGC/AEC algorithm, AEC - Step size limit = 1/16 x AEC */
	/* Banding & Reserved are disabled. AGC, AEC enabled, 0x85 */
	{OV7660_CTL_COM8, 0xb8},
	/* video appears jagged w/o these  ADC writes */
	{OV7660_CTL_ADC, 0x0f},
	{OV7660_CTL_ACOM, 0x02},
	{OV7660_CTL_OFON, 0x43},
	/* video appears jagged w/o this write */
	/* Default 0x0c sets format to uYvY, Windows driver 0x00 sets format to YuYv */
	{OV7660_CTL_TSLB, 0x00},
	/* Not doing this write makes the video look green */
	/* Manual Banding Filter MSB , set B & R channel pre-gain */
	{OV7660_CTL_HV, 0x90},
	/*  No video stream w/o these ADVFL/ADVFH write totally black */
	{OV7660_CTL_ADVFL, 0xf6},
	{OV7660_CTL_ADVFH, 0x0b},
	/* Setting BLUE to 0x78; RED to 0x78 to get natural colors in artificial light */
	{OV7660_CTL_BLUE, 0x78},
	/* Setting RED to 0x50 to get natural colors in natural light */
	{OV7660_CTL_RED,  0x50},
};

int ov7660_initialize(struct usb_microdia *dev)
{
	int ret, i;
	__u8 value, reg;

	ret = sn9c20x_i2c_initialize(dev);
	ret = ov7660_reset(dev);

	ret = ov7660_wakeup(dev);

	for (i = 0; i < ARRAY_SIZE(ov7660_init); i++) {
		reg = ov7660_init[i][0];
		value = ov7660_init[i][1];
		ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
					     reg, dev->sensor_flags, &value);
		if (ret < 0) {
			UDIA_INFO("Sensor OV7660: Inititialization Error (%d) on array line %d\n", ret, i);
			break;
		}
	}

	return ret;
}

/**
 * @brief Set exposure for ov7660 sensors
 *
 * @param dev
 *
 * @return 0 or negative error value
 *
 * @author Neekhil
*/
int ov7660_set_exposure(struct usb_microdia *dev)
{
	int ret = 0;
	__u8 v1 = (dev->vsettings.exposure >> 4) & 0xff;
	__u8 v2 = dev->vsettings.exposure >> 12;

	ret |= sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1, OV7660_CTL_ADVFL,
		dev->sensor_flags, &v1);

	ret |= sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1, OV7660_CTL_ADVFH,
		dev->sensor_flags, &v2);

	return ret;
}

/**
 * @brief Set autoexposure for ov7660 sensors
 *
 * @param dev
 *
 * @return 0 or negative error value
 *
 * @author Neekhil
*/
int ov7660_set_autoexposure(struct usb_microdia *dev)
{
	__u8 buf[1];
	int ret = 0;

	/* Read current value of the I2C-register
	 * controlling AutoExposureControl:
	 */
	ret = sn9c20x_read_i2c_data(dev, dev->sensor_slave_address,
		1, OV7660_CTL_COM8, dev->sensor_flags, buf);
	if (ret < 0) {
		UDIA_ERROR("Error: setting of auto exposure failed: "
			"error while reading from I2C-register OV7660_CTL_COM8");
		return ret;
	}

	/* Determine new value for OV7660_CTL_COM8 register */
	if (dev->vsettings.auto_exposure == 1) {
		/* Enable automatic exposure: */
		buf[0] |= 0x01;
	} else if (dev->vsettings.auto_exposure == 0) {
		/* Disable automatic exposure: */
		buf[0] &= ~0x01;
	} else
		return -EINVAL;

	/* Write new value to I2C-register OV7660_CTL_COM8 */
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address,
		1, OV7660_CTL_COM8, dev->sensor_flags, buf);
	if (ret < 0) {
		UDIA_ERROR("Error: setting of auto exposure failed: "
			"error while writing to I2C-register OV7660_CTL_COM8");
		return ret;
	}
	return 0;
}

/**
 * @brief Set ov7660 sensors to soft-sleep mode
 * @param dev
 * @return 0 or negative error value
 * @author Neekhil
*/
int ov7660_sleep(struct usb_microdia *dev)
{
	__u8 buf[1];
	int ret = 0;

	/* Set the sensor to Soft Sleep mode, Set O/P drive capability to 4X*/
	buf[0] = 0x13;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
					OV7660_CTL_COM2, dev->sensor_flags, buf);
	if (ret < 0)
		goto errI2C;

	return 0;

errI2C:
	UDIA_ERROR("Setting Image Sensor to soft sleep  failed(%d)!\n", ret);
	return ret;
}

/**
 * @brief Get ov7660 sensors to wake up from soft-sleep mode
 * @param dev
 * @return 0 or negative error value
 * @author Neekhil
*/
int ov7660_wakeup(struct usb_microdia *dev)
{
	__u8 buf[1];
	int ret = 0;

	/* Wake the sensor from Soft Sleep mode, Set O/P drive capability to 4X */
	buf[0] = 0x03;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
					OV7660_CTL_COM2, dev->sensor_flags, buf);
	if (ret < 0)
		goto errI2C;

	return 0;

errI2C:
	UDIA_ERROR("Image Sensor failed to wake-up from soft sleep (%d)!\n", ret);
	return ret;
}

/**
 * @brief Resets all the registers on ov7660 sensor
 * @param dev
 * @return 0 or negative error value
 * @author Neekhil
*/
int ov7660_reset(struct usb_microdia *dev)
{
	__u8 buf[1];
	int ret = 0;

	/* Reset all registers to default value                                    */
	/* Set O/P format - RGB Selection, Set O/P format Raw RGB */
	buf[0] = 0x85;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
					OV7660_CTL_COM7, dev->sensor_flags, buf);
	if (ret < 0)
		goto errI2C;

	return 0;

errI2C:
	UDIA_ERROR("Failed to reset the Image Sensor (%d)!\n", ret);
	return ret;
}
