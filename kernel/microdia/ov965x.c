/**
 * @file ov965x.c
 * @author Brian Johnson
 * @date 2008-08-06
 *
 * @brief Common functions and data for the Omnivision OV965x sensor series.
 *
 * @note Copyright (C) Brian Johnson
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
#include "ov965x.h"

/**
 * @var ov965x_init
 * @brief Addresses and values for the initialization of ov965x sensors
 *
 */
static __u8 ov965x_init[][2] = {
	{OV965X_CTL_COM7, OV965X_COM7_SCCB_RESET},
	{OV965X_CTL_GAIN, 0x00},
	{OV965X_CTL_BLUE, 0x78},
	{OV965X_CTL_RED, 0x78},
	{OV965X_CTL_VREF, OV965X_VREF_VSTOP_LOW3(0x06) |
			  OV965X_VREF_VSTART_LOW3(0x06)},
	{OV965X_CTL_COM1, 0x03},
	{OV965X_CTL_BAVE, 0x00}, /* default */
	{OV965X_CTL_GEAVE, 0x00}, /* default */
	{OV965X_CTL_RAVE, 0x00}, /* default */
	{OV965X_CTL_COM2, OV965X_COM2_OUTPUT_DRIVE_CAP_2X},
	{OV965X_CTL_COM3, 0x00},
	{OV965X_CTL_COM4, 0x00},
	{OV965X_CTL_COM5, OV965X_COM5_15FPS_48MHZ_RGB | 0x20},
	{OV965X_CTL_COM6, OV965X_COM6_TIMING_RESET_ON_FMT_CHANGE | 0x50},
	{OV965X_CTL_AECH, 0x7c},
	{OV965X_CTL_CLKRC, OV965X_CLKRC_DBL_CLK_ENABLE},
	{OV965X_CTL_COM7, OV965X_COM7_OUTPUT_VGA | OV965X_COM7_OUTPUT_RAW_RGB},
	{OV965X_CTL_COM8, OV965X_COM8_FAST_AGC_AEC |
			  OV965X_COM8_AEC_STEP_SIZE_NOLIMIT |
			  OV965X_COM8_AGC_ENABLE |
			  OV965X_COM8_AEC_ENABLE |
			  OV965X_COM8_AWB_ENABLE},
	{OV965X_CTL_COM9, OV965X_COM9_MAX_AGC_8X |
			  OV965X_COM9_RELAX_EXPOSURE_TIMING |
			  OV965X_COM9_DROP_VSYNC_ON_FRAME_DROP |
			  OV965X_COM9_DROP_FRAME_ON_BIG_AEC},
	{OV965X_CTL_COM10, 0x00},
	{0x16, 0x07}, /* reserved */
	{OV965X_CTL_HSTART, 0x24},
	{OV965X_CTL_HSTOP, 0xc5},
	{OV965X_CTL_VSTRT, 0x00},
	{OV965X_CTL_VSTOP, 0x3c},
	{OV965X_CTL_PSHIFT, 0x00}, /* default */
	{OV965X_CTL_MVFP, 0x04},
	{OV965X_CTL_LAEC, 0x00}, /* reserved */
	{OV965X_CTL_AEW, 0x78}, /* default */
	{OV965X_CTL_AEB, 0x68}, /* default */
	{OV965X_CTL_VPT, 0xd4}, /* default */
	{OV965X_CTL_BBIAS, OV965X_BIAS_SUBTRACT}, /* default */
	{OV965X_CTL_GbBIAS, OV965X_BIAS_SUBTRACT}, /* default */
	{OV965X_CTL_Gr_COM, OV965X_Gr_COM_BYPASS_ANALOG_BLC |
			    OV965X_Gr_COM_BYPASS_REGULATOR},
	{OV965X_CTL_EXHCH, 0x00}, /* default */
	{OV965X_CTL_EXHCL, 0x00}, /* default */
	{OV965X_CTL_RBIAS, OV965X_BIAS_SUBTRACT}, /* default */
	{OV965X_CTL_ADVFL, 0x00}, /* default */
	{OV965X_CTL_ADVFH, 0x00}, /* default */
	{OV965X_CTL_YAVE, 0x00}, /* default */
	{OV965X_CTL_HSYST, 0x08}, /* default */
	{OV965X_CTL_HSYEN, 0x30}, /* default */
	{OV965X_CTL_HREF, OV965X_HREF_EDGE_OFT_TO_DATA_OUT(2) |
			  OV965X_HREF_HSTOP_LOW3(0) |
			  OV965X_HREF_HSTART_LOW3(4)},
	{OV965X_CTL_CHLF, 0xe2}, /* reserved */
	{OV965X_CTL_ARBLM, 0xbf}, /* reserved */
	{0x35, 0x81}, /* reserved */
	{0x36, 0xf9}, /* reserved */
	{OV965X_CTL_ADC, 0x00}, /* reserved */
	{OV965X_CTL_ACOM, 0x93}, /* reserved */
	{OV965X_CTL_OFON, 0x50},
	{OV965X_CTL_TSLB, OV965X_TSLB_OUTPUT_SEQ_UYVY |
			  OV965X_TSLB_DIGITAL_BLC_ENABLE},
	{OV965X_CTL_COM11, OV965X_COM11_MANUAL_BANDING_FILTER},
	{OV965X_CTL_COM12, 0x73},
	{OV965X_CTL_COM13, OV965X_COM13_ENABLE_COLOR_MATRIX |
			   OV965X_COM13_DELAY_Y_CHANNEL |
			   OV965X_COM13_OUTPUT_DELAY(1)},
	{OV965X_CTL_COM14, OV965X_COM14_YUV_EDGE_ENHANCE |
			   OV965X_COM14_EDGE_ENHANCE_FACTOR_DBL | 0x0b},
	{OV965X_CTL_EDGE, OV965X_EDGE_EDGE_ENHANCE_LOW4(8) |
			  OV965X_EDGE_EDGE_ENHANCE_FACTOR(8)},
	{OV965X_CTL_COM15, OV965X_COM15_OUTPUT_RANGE_O0_TO_FF | 0x01},
	{OV965X_CTL_COM16, 0x00},
	{OV965X_CTL_COM17, 0x08},
	{OV965X_CTL_MANU, 0x80}, /* default */
	{OV965X_CTL_MANV, 0x80}, /* default */
	{OV965X_CTL_HV, 0x40},
	{OV965X_CTL_MBD, 0x00}, /* default */
	{OV965X_CTL_DBLV, 0x0a}, /* reserved */
	{OV965X_CTL_COM21, 0x06}, /* reserved */
	{OV965X_CTL_COM22, 0x20},
	{OV965X_CTL_COM23, 0x00}, /* default */
	{OV965X_CTL_COM24, 0x00}, /* reserved */
	{OV965X_CTL_DBLC1, 0xdf},
	{OV965X_CTL_DM_LNL, 0x00}, /* default */
	{OV965X_CTL_DM_LNH, 0x00}, /* default */
	{0x94, 0x88}, /* reserved */
	{0x95, 0x88}, /* reserved */
	{0x96, 0x04}, /* reserved */
	{OV965X_CTL_AECHM, 0x00},
	{OV965X_CTL_COM26, 0x80}, /* reserved */
	{0xa8, 0x80}, /* reserved */
	{0xa9, 0xb8}, /* reserved */
	{0xaa, 0x92}, /* reserved */
	{0xab, 0x0a}, /* reserved */
};

/**
 * @brief Initialize ov965x sensors
 *
 * @param dev Pointer to device structure
 *
 * @return 0 or negative error code
 *
 * This function applies the default settings from #ov965x_init to
 * the ov965x sensor
 *
 */
int ov965x_initialize(struct usb_microdia *dev)
{
	int ret, i;
	__u8 value, reg;

	for (i = 0; i < ARRAY_SIZE(ov965x_init); i++) {
		reg = ov965x_init[i][0];
		value = ov965x_init[i][1];
		ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
					     reg, dev->sensor_flags, &value);
		if (ret < 0) {
			UDIA_INFO("Sensor Init Error (%d). line %d\n", ret, i);
			break;
		}
	}

	return ret;

}

/**
 * @var soi968_init
 * @brief Addresses and values for the initialization of SOI968 sensors
 * @author GWater
 *
 */
static __u8 soi968_init[][2] = {
	/* reset all registers */
	{0x12, 0x80},
	/* stop resetting */
	{0x12, 0x00},

	/* snapshot mode: off */
	{0x0c, 0x00},

	/* enable offset adjustment,
	 * full data range,
	 * analogue2digital control black-level calibration
	 */
	{0x0f, 0x1f},

	/* Clock: internal PLL on */
	{0x11, 0x80},


	/* Analoge Black-level calibration off , no anaolgue gain */
	{0x38, 0x52},

	/* reserved */
	{0x1e, 0x00},

	/* special system settings (voltage, analogue2digital, ...) */
	{0x33, 0x08},
	{0x35, 0x8c},
	{0x36, 0x0c},
	{0x37, 0x04},

	/*  next 7 are unknown/reserved */
	{0x45, 0x04},
	{0x47, 0xff},
	{0x3e, 0x00},
	{0x3f, 0x00},
	{0x3b, 0x20},
	{0x3a, 0x96},
	{0x3d, 0x0a},

	/* disable banding filter in dark environment,
	 * VSYNC is dropped when framerate is dropped,
	 * drop frmaes when exposure out of tolerance,
	 * unfreeze exposure and gain values
	 */
	{0x14, 0x4e},

	/* AEC, AGC, AWB disabled; fast AEC */
	{0x13, 0x88},

	/* output: VGA, master mode */
	{0x12, 0x40},

	/* set HSTART, HSTOP, VSTART and VSTOP */
	{0x17, 0x13},
	{0x18, 0x63},
	{0x19, 0x01},
	{0x1a, 0x79},
	{0x32, 0x24}, /* LSB for all four */

	/* this register contains the product ID, it is read-only
	{0x1b, 0x00}, */

	/* AWB update threshold,
	 * blue and red gain LSB
	 */
	{0x03, 0x00},

	/* CLock: internal PLL off */
	{0x11, 0x40},

	/* Line interval adjustment */
	{0x2a, 0x10},
	{0x2b, 0xe0},

	/* AEC MSB */
	{0x10, 0x32},

	/* gain - default*/
	{0x00, 0x00},

	/* blue and red gain - default*/
	{0x01, 0x80},
	{0x02, 0x80}
};

/**
 * @brief Initialize SOI968 sensors
 *
 * @param dev Pointer to device structure
 *
 * @return 0 or negative error code
 *
 * @author GWater
 *
 * This function applies the default settings from #soi968_init to
 * the SOI968 sensor
 *
 */
int soi968_initialize(struct usb_microdia *dev)
{
	int ret, i;
	__u8 value, reg;

	for (i = 0; i < ARRAY_SIZE(soi968_init); i++) {
		reg = soi968_init[i][0];
		value = soi968_init[i][1];
		ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
					     reg, dev->sensor_flags, &value);
		if (ret < 0) {
			UDIA_INFO("Sensor Init Error (%d). line %d\n", ret, i);
			break;
		}
	}

	return ret;

}

/**
 * @brief Set hflip and vflip in ov965x sensors
 *
 * @param dev Pointer to device structure
 *
 * @returns 0 or negative error code
 *
 */
int ov965x_set_hvflip(struct usb_microdia *dev)
{
	int ret;
	__u8 value;
	/**
	 * Changing hstop value seems to be necessary to keep correct
	 * colors during a vflip. The values don't seem to make much
	 * sense since to keep the correct color value i'm setting hstop
	 * to the same value as hstart is set for.
	 */
	__u8 hstop = 0xc5;
	ret = sn9c20x_read_i2c_data(dev, dev->sensor_slave_address, 1,
			OV965X_CTL_MVFP, dev->sensor_flags, &value);
	if (ret < 0)
		return ret;

	if (dev->vsettings.hflip)
		value |= OV965X_MVFP_MIRROR;
	else
		value &= ~OV965X_MVFP_MIRROR;

	if (dev->vsettings.vflip) {
		hstop = 0x24;
		value |= OV965X_MVFP_VFLIP;
	} else {
		value &= ~OV965X_MVFP_VFLIP;
	}

	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV965X_CTL_HSTOP, dev->sensor_flags, &hstop);

	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV965X_CTL_MVFP, dev->sensor_flags, &value);

	return ret;
}

/**
 * @brief Set exposure for ov965x sensors
 *
 * @param dev
 *
 * @returns 0 or negative error value
 *
 * The used registers do not show up the datasheets.
 *
 */
int ov965x_set_exposure(struct usb_microdia *dev)
{
	int ret = 0;
	__u8 v1 = (dev->vsettings.exposure >> 4) & 0xff;
	__u8 v2 = dev->vsettings.exposure >> 12;

	ret |= sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1, 0x2d,
		dev->sensor_flags, &v1);

	ret |= sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1, 0x2e,
		dev->sensor_flags, &v2);

	return ret;
}

/**
 * @brief Set autoexposure for ov96xx sensors
 *
 * @param dev
 *
 * @returns 0 or negative error value
 *
 * @author GWater
 *
 * For all OV965x and SOI968 sensors.
 */
int ov965x_set_autoexposure(struct usb_microdia *dev)
{
	__u8 buf[1];
	int ret = 0;

	/* Read current value of the I2C-register
	 * controlling AutoExposureControl:
	 */
	ret = sn9c20x_read_i2c_data(dev, dev->sensor_slave_address,
		1, 0x13, dev->sensor_flags, buf);
	if (ret < 0) {
		UDIA_ERROR("Error: setting of auto exposure failed: "
			"error while reading from I2C-register 0x13");
		return ret;
	}

	/* Determine new value for register 0x13: */
	if (dev->vsettings.auto_exposure == 1) {
		/* Enable automatic exposure: */
		buf[0] |= 0x01;
	} else if (dev->vsettings.auto_exposure == 0) {
		/* Disable automatic exposure: */
		buf[0] &= ~0x01;
	} else
		return -EINVAL;

	/* Write new value to I2C-register 0x13: */
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address,
		1, 0x13, dev->sensor_flags, buf);
	if (ret < 0) {
		UDIA_ERROR("Error: setting of auto exposure failed: "
			"error while writing to I2C-register 0x13");
		return ret;
	}
	return 0;
}

/**
 * @brief Set exposure for SOI968 sensors
 *
 * @param dev
 *
 * @returns 0 or negative error value
 *
 * @author GWater
 *
 * For SOI968 sensors.
 */
int soi968_set_exposure(struct usb_microdia *dev)
{
	int value, ret;
	int exposure = dev->vsettings.exposure;
	__u8 buf1, buf2, buf3;

	/* Read current value of the I2C-register
	 * containing exposure LSB:
	 */
	ret = sn9c20x_read_i2c_data(dev, dev->sensor_slave_address,
		1, 0x04, dev->sensor_flags, &buf1);
	if (ret < 0) {
		UDIA_ERROR("Error: setting exposure failed: "
			"error while reading from I2C-register 0x04");
		return ret;
	}

	value = (exposure  * 0x07ff / 0xffff) & 0x07ff;
	buf2 = ((__u8) (value & 0x07)) | (buf1 & ~0x07);
	buf3 = (__u8) (value >> 3) & 0xff;

	/* Write new value to I2C-register 0x04: */
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address,
		1, 0x04, dev->sensor_flags, &buf2);
	if (ret < 0) {
		UDIA_ERROR("Error: setting exposure failed: "
			"error while writing to I2C-register 0x04");
		return ret;
	}

	/* Write new value to I2C-register 0x10: */
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address,
		1, 0x10, dev->sensor_flags, &buf3);
	if (ret < 0) {
		UDIA_ERROR("Error: setting exposure failed: "
			"error while writing to I2C-register 0x10");
		return ret;
	}

	return 0;
}
