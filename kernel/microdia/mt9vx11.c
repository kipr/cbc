/**
 * @file mt9vx11.c
 * @author Comer352l
 * @date 2008-04-25
 *
 * @brief Common functions and data for the Micron MT9Vx11 sensors.
 *
 * @note Copyright (C) Comer352l
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
#include "microdia.h"
#include "sn9c20x.h"
#include "mt9vx11.h"



int mt9v111_select_address_space(struct usb_microdia *dev, __u8 address_space)
{
	__u8 buf[2];
	int retI2C;
	int k;

	/* check if selection is valid: */
	if ((address_space != MT9V111_ADDRESSSPACE_IFP) && (address_space != MT9V111_ADDRESSSPACE_SENSOR)) {
		UDIA_INFO("Error: invalid register address space selection for sensor MT9V111/MI0360SOC !\n");
		return -1;
	}
	/* read address space slection register: */
	k = 0;
	retI2C = -1;
	while ((k < 3) && (retI2C != 0)) {
		retI2C = sn9c20x_read_i2c_data(dev, MT9V111_I2C_SLAVE_ADDRESS, 2, 0x01, SN9C20X_I2C_2WIRE, buf);
		if (retI2C != 0 && k < 2)
			udelay(1000);
		k++;
	}
	if (retI2C != 0) {
		UDIA_INFO("Error: MT9V111/MI0360SOC (I2C-slave 0x5c): read of reg0x01 (address space selection) failed !\n");
		return -1;
	}
	/* check current address space: */
	if ((buf[0] != 0x00) || (buf[1] != address_space)) {
		k = 0;
		retI2C = -1;
		while ((k < 3) && (retI2C != 0)) {
			/* switch address space: */
			buf[0] = 0x00; buf[1] = address_space;
			retI2C = sn9c20x_write_i2c_data(dev, MT9V111_I2C_SLAVE_ADDRESS, 2, 0x01, SN9C20X_I2C_2WIRE, buf);
			if (retI2C != 0 && k < 2)
				udelay(1000);
			k++;
		}
		if (retI2C != 0) {
			if (address_space == MT9V111_ADDRESSSPACE_IFP)
				UDIA_INFO("Error: MT9V111/MI0360SOC (I2C-slave 0x5c): switch to IFP address space failed !\n");
			else
				UDIA_INFO("Error: MT9V111/MI0360SOC (I2C-slave 0x5c): switch to sensor core address space failed !\n");
			return -1;
		}
	}
	return 0;
}


int mt9vx11_sensor_probe(struct usb_microdia *dev)
{
	__u8 buf[2];
	int ret;
	int retI2C;
	int k;

	/* *** Probe MT9V011/MI0360: */
	/* read chip version: */
	for (k = 0; k < 3; k++) {
		retI2C = sn9c20x_read_i2c_data(dev, MT9V011_I2C_SLAVE_ADDRESS, 2, 0xff, SN9C20X_I2C_2WIRE, buf);
		if (retI2C == 0) {
			if ((buf[0] == 0x82) && (buf[1] == 0x43)) {
				UDIA_INFO("Detected sensor: MT9V011/MI0360 (chip version: 0x%02X%02X)\n", buf[0], buf[1]);
				dev->sensor_slave_address = MT9V011_I2C_SLAVE_ADDRESS;
				return 0;
			} else
				UDIA_DEBUG("I2C-slave 0x5d returned invalid chip version: 0x%02X%02X\n", buf[0], buf[1]);
		} else
			udelay(500);
	}
	/* NOTE: DNT DigiMicro 1.3 (microscope camera):
	 * This device uses sensor MT9V111, but slave 0x5d is also successfully read.
	 * Registers 0x00, 0x36 and 0xff of slave 0x5d return chip version 0x0000.
	 */

	/* *** Probe MT9V111/MI0360SOC: */
	/* switch register address space: */
	ret = mt9v111_select_address_space(dev, MT9V111_ADDRESSSPACE_SENSOR);
	if (ret != 0)
		return -1;
	/* read chip version: */
	for (k = 0; k < 3; k++) {
		retI2C = sn9c20x_read_i2c_data(dev, MT9V111_I2C_SLAVE_ADDRESS, 2, 0xff, SN9C20X_I2C_2WIRE, buf);
		if (retI2C == 0) {
			if ((buf[0] == 0x82) && (buf[1] == 0x3a)) {
				UDIA_INFO("Detected sensor: MT9V111/MI0360SOC (chip version: 0x%02X%02X)\n", buf[0], buf[1]);
				dev->sensor_slave_address = MT9V111_I2C_SLAVE_ADDRESS;
				return 0;
			} else
				UDIA_DEBUG("I2C-slave 0x5c returned invalid chip version: 0x%02X%02X\n", buf[0], buf[1]);
		} else
			udelay(500);
	}

	/* FIXME: always switch back to last register address space */

	UDIA_INFO("Error: sensor probe failed !\n");
	dev->sensor_slave_address = 0;
	return -1;
}


int mt9v111_setup_autoexposure(struct usb_microdia *dev)
{
	__u16 buf[1];
	int ret = 0;
	int ret2 = 0;

	/* Check if sensor is MT9V111: */
	if (dev->sensor_slave_address != MT9V111_I2C_SLAVE_ADDRESS)
		return -ENODEV;
	/* Switch to IFP-register address space: */
	ret = mt9v111_select_address_space(dev, MT9V111_ADDRESSSPACE_IFP);
	if (ret < 0)
		return -11;	/* -EAGAIN */
	/* Set target luminance and tracking accuracy: */
	buf[0] = MT9V111_IFP_AE_TRACKINGACCURACY(12)		/* 0-255 (default: 16) */
		 | MT9V111_IFP_AE_TARGETLUMINANCE(100);		/* 0-255 (default: 100) */
	ret = sn9c20x_write_i2c_data16(dev, dev->sensor_slave_address, 1,
					MT9V111_IFPREG_AE_TARGETLUMCTL, dev->sensor_flags, buf);
	/* Set speed and sensitivity: */
	buf[0] = MT9V111_IFP_AE_DELAY(4) 			/* 0-7 (fastest-slowest) (default: 4) */
		 | MT9V111_IFP_AE_SPEED(4)			/* 0-7 (fastest-slowest) (default: 4) */
		 | MT9V111_IFP_AE_STEPSIZE_MEDIUM;
	ret += sn9c20x_write_i2c_data16(dev, dev->sensor_slave_address, 1,
					MT9V111_IFPREG_AE_SPEEDSENSCTL, dev->sensor_flags, buf);
	/* Horizontal boundaries of AE measurement window: */
	buf[0] = MT9V111_IFP_AE_WINBOUNDARY_RIGHT(1020)		/* 0-1020 (pixel) (default: 1020) */
		 | MT9V111_IFP_AE_WINBOUNDARY_LEFT(12);		/* 0-1020 (pixel) (default: 12) */
	ret += sn9c20x_write_i2c_data16(dev, dev->sensor_slave_address, 1,
					MT9V111_IFPREG_AE_HWINBOUNDARY, dev->sensor_flags, buf);
	/* Vertical boundaries of AE measurement window: */
	buf[0] = MT9V111_IFP_AE_WINBOUNDARY_BOTTOM(510)		/* 0-510 (pixel) (default: 510) */
		 | MT9V111_IFP_AE_WINBOUNDARY_TOP(32);		/* 0-510 (pixel) (default: 32) */
	ret += sn9c20x_write_i2c_data16(dev, dev->sensor_slave_address, 1,
					MT9V111_IFPREG_AE_VWINBOUNDARY, dev->sensor_flags, buf);
	/* Horizontal boundaries of AE measurement window for back light compensation: */
	buf[0] = MT9V111_IFP_AE_WINBOUNDARY_RIGHT_BLC(480)	/* 0-1020 (pixel) (default: 480) */
		 | MT9V111_IFP_AE_WINBOUNDARY_LEFT_BLC(160);	/* 0-1020 (pixel) (default: 160) */
	ret += sn9c20x_write_i2c_data16(dev, dev->sensor_slave_address, 1,
					MT9V111_IFPREG_AE_HWINBOUNDARY_BLC, dev->sensor_flags, buf);
	/* Vertical boundaries of AE measurement window for back light compensation: */
	buf[0] = MT9V111_IFP_AE_WINBOUNDARY_BOTTOM_BLC(360)	/* 0-510 (pixel) (default: 360) */
		 | MT9V111_IFP_AE_WINBOUNDARY_TOP_BLC(120);	/* 0-510 (pixel) (default: 120) */
	ret += sn9c20x_write_i2c_data16(dev, dev->sensor_slave_address, 1,
					MT9V111_IFPREG_AE_VWINBOUNDARY_BLC, dev->sensor_flags, buf);
	/* Set digital gains limit: */
	buf[0] = MT9V111_IFP_AE_MAXGAIN_POSTLS(64)		/* 0-255 (default: 64) */
		 | MT9V111_IFP_AE_MAXGAIN_PRELS(16);		/* 0-255 (default: 16) */
	ret += sn9c20x_write_i2c_data16(dev, dev->sensor_slave_address, 1,
					MT9V111_IFPREG_AE_DGAINSLIMITS, dev->sensor_flags, buf);
	/* Read current value of IFP-register 0x06: */
	ret2 = sn9c20x_read_i2c_data16(dev, dev->sensor_slave_address, 1,
					MT9V111_IFPREG_OPMODECTL, dev->sensor_flags, buf);
	if (ret2 == 0) {
		/* Set new value for register 0x06: */
		buf[0] = (buf[0] & 0xffe3) | MT9V111_IFP_AE_WIN_COMBINED | MT9V111_IFP_OPMODE_BYPASSCOLORCORR;
		/* NOTE: BYPASS COLOR CORRECTION HAS TO BE SET FOR PERMANENT AE ! */
		/* Write new value to IFP-register 0x06:*/
		ret += sn9c20x_write_i2c_data16(dev, dev->sensor_slave_address, 1,
						MT9V111_IFPREG_OPMODECTL, dev->sensor_flags, buf);
	}

	if (ret < 0 || ret2 < 0) {
		UDIA_INFO("One or more errors occured during setup of AE registers.\n");
		return -1;
	}
	return 0;
}


int mt9v111_setup_autowhitebalance(struct usb_microdia *dev)
{
	__u16 buf[1];
	int ret = 0;

	/* Check if sensor is MT9V111: */
	if (dev->sensor_slave_address != MT9V111_I2C_SLAVE_ADDRESS)
		return -ENODEV;
	/* Switch to IFP-register address space: */
	ret = mt9v111_select_address_space(dev, MT9V111_ADDRESSSPACE_IFP);
	if (ret < 0)
		return -11;	/* -EAGAIN */
	/* Set AWB tint: */
	buf[0] = MT9V111_IFP_AWB_ADDON_RED(0)		/* 0-255 (default: 0) */
		 | MT9V111_IFP_AWB_ADDON_BLUE(0);	/* 0-255 (default: 0) */
	ret = sn9c20x_write_i2c_data16(dev, dev->sensor_slave_address, 1,
					MT9V111_IFPREG_AWB_TINT, dev->sensor_flags, buf);
	/* Set AWB speed and color saturation: */
	buf[0] = MT9V111_IFP_AWB_SATURATION_FULL | MT9V111_IFP_AWB_AUTOSATLOWLIGHT_ENABLE
		 | MT9V111_IFP_AWB_DELAY(4)		/* 0-7 (fastest-slowest) (default: 4) */
		 | MT9V111_IFP_AWB_SPEED(4);		/* 0-7 (fastest-slowest) (default: 4) */
	ret += sn9c20x_write_i2c_data16(dev, dev->sensor_slave_address, 1,
					MT9V111_IFPREG_AWB_SPEEDCOLORSATCTL, dev->sensor_flags, buf);
	/* Boundaries of AWB measurement window: */
	buf[0] = MT9V111_IFP_AWB_WINBOUNDARY_TOP(0)		/* 0-480 (pixel) (default: 0) */
		 | MT9V111_IFP_AWB_WINBOUNDARY_BOTTOM(480)	/* 0-480 (pixel) (default: 448) */
		 | MT9V111_IFP_AWB_WINBOUNDARY_LEFT(0)		/* 0-960 (pixel) (default: 0) */
		 | MT9V111_IFP_AWB_WINBOUNDARY_RIGHT(640);	/* 0-960 (pixel) (default: 640) */
	ret += sn9c20x_write_i2c_data16(dev, dev->sensor_slave_address, 1,
					MT9V111_IFPREG_AWB_WINBOUNDARY, dev->sensor_flags, buf);

	if (ret < 0) {
		UDIA_INFO("One or more errors occured during setup of AWB registers.\n");
		return -1;
	}
	return 0;
}


int mt9vx11_set_exposure(struct usb_microdia *dev)
{
	int ret = 0;
	__u8 buf[2];

	if (dev->sensor_slave_address == MT9V111_I2C_SLAVE_ADDRESS)
		ret = mt9v111_select_address_space(dev, MT9V111_ADDRESSSPACE_SENSOR);

	if (ret < 0)
		return -11;	/* -EAGAIN */

	buf[0] = (dev->vsettings.exposure >> 12);
	buf[1] = 0;
	ret |= sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 2, 0x09, dev->sensor_flags, buf);
	/* Maybe we have to disable AE/AWB/flicker avoidence (currently not used)
	   for MT9V111 sensor, because IFP controls this register if one of them
	   is enabled. */

	return ret;
}


int mt9vx11_set_hvflip(struct usb_microdia *dev)
{
	int ret = 0;
	__u8 buf[2];

	if ((dev->vsettings.hflip > 1) || (dev->vsettings.hflip < 0))
		return -EINVAL;
	if ((dev->vsettings.vflip > 1) || (dev->vsettings.vflip < 0))
		return -EINVAL;

	if (dev->sensor_slave_address == MT9V111_I2C_SLAVE_ADDRESS)
		ret = mt9v111_select_address_space(dev, MT9V111_ADDRESSSPACE_SENSOR);
	if (ret < 0)
		return -11;	/* -EAGAIN */

	ret = sn9c20x_read_i2c_data(dev, dev->sensor_slave_address, 2, 0x20, dev->sensor_flags, buf);
	if (ret < 0)
		return ret;

	if (dev->vsettings.hflip) {
		buf[0] |= 0x80;	/* (MSB) set bit 15: read out from bottom to top (upside down) */
		buf[1] |= 0x80;	/* (LSB) set bit 7: readout starting 1 row later */
	} else {
		buf[0] &= 0x7f;	/* (MSB) unset bit 15: normal readout */
		buf[1] &= 0x7f;	/* (LSB) unset bit 7: normal readout */
	}
	if (dev->vsettings.vflip) {
		buf[0] |= 0x40;	/* (MSB) set bit 14: read out from right to left (mirrored) */
		buf[1] |= 0x20;	/* (LSB) set bit 5: readout starting 1 column later */
	} else {
		buf[0] &= 0xbf;	/* (MSB) unset bit 14: normal readout */
		buf[1] &= 0xdf;	/* (LSB) unset bit 5: normal readout */
	}

	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 2, 0x20, dev->sensor_flags, buf);
	return ret;
}


int mt9v111_set_autoexposure(struct usb_microdia *dev)
{
	__u16 buf[1];
	int ret = 0;

	/* Check if sensor is MT9V111: */
	if (dev->sensor_slave_address != MT9V111_I2C_SLAVE_ADDRESS)
		return -ENODEV;
	/* Switch to IFP-register address space: */
	ret = mt9v111_select_address_space(dev, MT9V111_ADDRESSSPACE_IFP);
	if (ret < 0)
		return -11;	/* -EAGAIN */
	/* Read current value of IFP-register 0x06: */
	ret = sn9c20x_read_i2c_data16(dev, dev->sensor_slave_address, 1,
				MT9V111_IFPREG_OPMODECTL, dev->sensor_flags, buf);
	if (ret < 0) {
		UDIA_ERROR("Error: setting of auto exposure failed: error while reading from IFP-register 0x06\n");
		return ret;
	}
	/* Set new value for register 0x06: */
	if (dev->vsettings.auto_exposure == 1) {
		/* Enable automatic exposure: */
		buf[0] |= MT9V111_IFP_OPMODE_AUTOEXPOSURE;
	} else if (dev->vsettings.auto_exposure == 0) {
		/* Disable automatic exposure: */
		buf[0] &= ~MT9V111_IFP_OPMODE_AUTOEXPOSURE;
	} else
		return -EINVAL;
	/* Write new value to IFP-register 0x06: */
	ret = sn9c20x_write_i2c_data16(dev, dev->sensor_slave_address, 1,
					MT9V111_IFPREG_OPMODECTL, dev->sensor_flags, buf);
	if (ret < 0) {
		UDIA_ERROR("Error: setting of auto exposure failed: error while writing to IFP-register 0x06\n");
		return ret;
	}
	return 0;
}


int mt9v111_set_autowhitebalance(struct usb_microdia *dev)
{
	__u16 buf[1];
	int ret = 0;

	/* Check if sensor is MT9V111: */
	if (dev->sensor_slave_address != MT9V111_I2C_SLAVE_ADDRESS)
		return -ENODEV;
	/* Switch to IFP-register address space: */
	ret = mt9v111_select_address_space(dev, MT9V111_ADDRESSSPACE_IFP);
	if (ret < 0)
		return -11;	/* -EAGAIN */
	/* Read current value of IFP-register 0x06: */
	ret = sn9c20x_read_i2c_data16(dev, dev->sensor_slave_address, 1,
				MT9V111_IFPREG_OPMODECTL, dev->sensor_flags, buf);
	if (ret < 0) {
		UDIA_ERROR("Error: setting of auto whitebalance failed: error while reading from IFP-register 0x06\n");
		return ret;
	}
	/* Set new value for register 0x06: */
	if (dev->vsettings.auto_whitebalance == 1) {
		/* Enable automatic exposure: */
		buf[0] |= MT9V111_IFP_OPMODE_AUTOWHITEBALANCE;
	} else if (dev->vsettings.auto_whitebalance == 0) {
		/* Disable automatic exposure: */
		buf[0] &= ~MT9V111_IFP_OPMODE_AUTOWHITEBALANCE;
	} else
		return -EINVAL;
	/* Write new value to IFP-register 0x06:*/
	ret = sn9c20x_write_i2c_data16(dev, dev->sensor_slave_address, 1,
					MT9V111_IFPREG_OPMODECTL, dev->sensor_flags, buf);
	if (ret < 0) {
		UDIA_ERROR("Error: setting of auto whitebalance failed: error while writing to IFP-register 0x06\n");
		return ret;
	}
	return 0;
}


int mt9v111_set_autocorrections(struct usb_microdia *dev, int enable)
{
	__u16 buf[1];
	int ret = 0;

	/* Check if sensor is MT9V111: */
	if (dev->sensor_slave_address != MT9V111_I2C_SLAVE_ADDRESS)
		return -ENODEV;
	/* Switch to IFP-register address space: */
	ret = mt9v111_select_address_space(dev, MT9V111_ADDRESSSPACE_IFP);
	if (ret < 0)
		return -11;	/* -EAGAIN */
	/* Read current value of IFP-register 0x06: */
	ret = sn9c20x_read_i2c_data16(dev, dev->sensor_slave_address, 1,
				MT9V111_IFPREG_OPMODECTL, dev->sensor_flags, buf);
	if (ret < 0) {
		UDIA_ERROR("Error: setting of sensor auto-correction functions failed: error while reading from IFP-register 0x06\n");
		return ret;
	}
	/* Set new value for register 0x06: */
	if (enable == 1)
		buf[0] |= MT9V111_IFP_OPMODE_APERTURECORR | MT9V111_IFP_OPMODE_ONTHEFLYDEFECTCORR;
	else if (enable == 0)
		buf[0] &= ~(MT9V111_IFP_OPMODE_APERTURECORR | MT9V111_IFP_OPMODE_ONTHEFLYDEFECTCORR);
	else
		return -EINVAL;
	/* Write new value to IFP-register 0x06: */
	ret = sn9c20x_write_i2c_data16(dev, dev->sensor_slave_address, 1,
					MT9V111_IFPREG_OPMODECTL, dev->sensor_flags, buf);
	if (ret < 0) {
		UDIA_ERROR("Error: setting of sensor auto-correction functions failed: error while writing to IFP-register 0x06\n");
		return ret;
	}
	return 0;
}
