/**
 * @file ov7670.c
 * @author Phil Mitchell
 * @date 2008-05-09
 *
 * @brief Common functions and data for the OmniVision OV7670 sensor.
 *
 * @note Copyright (C) Phil Mitchell
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
#include "ov7670.h"

/**
 * @brief Initializes OV7670 Sensor Registers
 *
 * @param dev Pointer to the device
 *
 * @return Zero (success) or negative (USB-error value)
 *
 */
int ov7670_initialise(struct usb_microdia *dev)
{
	__u8 buf[32];
	int ret;

	/*
	 * Phase 1
	 */
	buf[0] = 0x80;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_COM7, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x80;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_CLKRC, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x04;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_TSLB, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x00;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_COM7, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0xb6;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_HREF, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x0a;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_VREF, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x00;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_COM3, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x00;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_COM14, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x3a;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_SCALING_XSC, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x35;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_SCALING_YSC, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x11;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_SCALING_DCWCTR, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0xf0;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_SCALING_PCLK_DIV, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x02;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_SCALING_PCLK_DELAY, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0xe0;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_COM8, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x00;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_GAIN, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x00;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_AECH, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x40;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_COM4, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x08;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_COM9, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x05;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_BD50MAX, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x07;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_BD60MAX, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x95;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_AEW, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x33;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_AEB, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0xe3;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_VPT, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x75;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_HAECC1, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x65;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_HAECC2, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x0b;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			0xa1, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0xd8;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_HAECC3, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0xd8;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_HAECC4, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0xf0;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_HAECC5, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x90;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_HAECC6, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x94;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_HAECC7, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0xe5;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_COM8, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x61;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_COM5, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x4b;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_COM6, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x02;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			0x16, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x27;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_MVFP, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x02;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_ADCCTR1, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x91;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_ADCCTR2, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x07;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			0x29, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x0b;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_CHLF, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x0b;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			0x35, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x1d;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_ADC, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x71;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_ACOM, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x2a;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_OFON, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x78;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_COM12, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x40;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			0x4d, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x20;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			0x4e, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x00;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_GFIX, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x19;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_REG74, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x4f;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			0x8d, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x00;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			0x8e, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x00;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			0x8f, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x00;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			0x90, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x00;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			0x91, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x00;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			0x96, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x80;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			0x9a, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x84;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			0xb0, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x0c;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_ABLC1, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x0e;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			0xb2, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x82;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_THL_ST, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x0a;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			0xb8, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x0a;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_AWBC1, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0xf0;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_AWBC2, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x20;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_AWBC3, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x7d;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_AWBC4, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x29;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_AWBC5, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x4a;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_AWBC6, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x8c;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			0x59, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0xa5;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			0x5a, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0xde;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			0x5b, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x96;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			0x5c, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x66;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			0x5d, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x10;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			0x5e, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x0a;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_AWBCTR3, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x55;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_AWBCTR2, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x11;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_AWBCTR1, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x9e;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_AWBCTR0, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x40;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_GGAIN, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x40;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_BLUE, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x40;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_RED, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0xe7;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_COM8, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x6e;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_MTX1, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x70;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_MTX2, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x02;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_MTX3, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x1d;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_MTX4, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x56;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_MTX5, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x73;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_MTX6, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x0a;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_BRIGHT, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x55;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_CONTRAS, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x80;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_CONTRAS_CENTER, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x9e;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_MTXS, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x08;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_COM16, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x02;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_EDGE, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x03;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_REG75, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x63;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_REG76, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x04;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_DNSTH, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x06;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_REG77, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0xc2;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_COM13, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x09;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_REG4B, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x30;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_SATCTR, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x08;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_COM16, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x48;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_CONTRAS, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x11;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_ARBLM, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0xc2;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_COM11, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x88;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_NT_CTRL, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x00;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			0x96, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x30;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			0x97, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x20;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			0x98, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x30;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			0x99, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x84;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			0x9a, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x29;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			0x9b, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x03;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			0x9c, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x99;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_BD50ST, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x7f;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_BD60ST, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x04;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			0x78, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x01;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			0x79, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0xf0;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			0xc8, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x0f;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			0x79, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x00;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			0xc8, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x10;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			0x79, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x7e;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			0xc8, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x0a;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			0x79, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x80;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			0xc8, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x0b;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			0x79, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x01;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			0xc8, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x0c;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			0x79, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x0f;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			0xc8, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x0d;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			0x79, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x20;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			0xc8, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x09;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			0x79, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x80;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			0xc8, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x02;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			0x79, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0xc0;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			0xc8, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x03;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			0x79, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x40;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			0xc8, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x05;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			0x79, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x30;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			0xc8, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x26;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			0x79, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x20;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_LCC1, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x00;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_LCC2, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x06;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_LCC3, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x00;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_LCC4, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x05;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_LCC5, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x05;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_LCC6, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x0a;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_LCC7, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x13;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_HSTART, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x01;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_HSTOP, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x02;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_VSTRT, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x7a;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_VSTOP, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x59;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_AWBC4, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x30;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_AWBC5, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x9a;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_MTXS, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x84;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			0x59, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x91;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			0x5a, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x57;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			0x5b, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x75;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			0x5c, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x6d;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			0x5d, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x13;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			0x5e, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x07;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_LCC3, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x07;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_LCC6, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x0d;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_LCC7, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0xdf;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_HAECC3, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0xdf;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_HAECC4, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x4d;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_AWBC6, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x00;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_MTX3, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	/*
	 * Phase 2
	 */
	buf[0] = 0x0a;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_DBLV, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x80;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_CLKRC, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x00;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_EXHCH, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x00;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_EXHCL, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x00;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_DM_LNL, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x00;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_DM_LNH, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0xc2;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_COM11, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x0a;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_BRIGHT, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x60;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_CONTRAS, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x6e;
	buf[1] = 0x70;
	buf[2] = 0x00;
	buf[3] = 0x1d;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 4,
			OV7670_CTL_MTX1, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x56;
	buf[1] = 0x73;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 2,
			OV7670_CTL_MTX5, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x9a;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_MTXS, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x6e;
	buf[1] = 0x70;
	buf[2] = 0x00;
	buf[3] = 0x1d;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 4,
			OV7670_CTL_MTX1, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x56;
	buf[1] = 0x73;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 2,
			OV7670_CTL_MTX5, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x9a;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_MTXS, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x01;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_EDGE, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x03;
	buf[1] = 0x09;
	buf[2] = 0x16;
	buf[3] = 0x38;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 4,
			OV7670_CTL_GAM1, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x47;
	buf[1] = 0x53;
	buf[2] = 0x5e;
	buf[3] = 0x6a;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 4,
			OV7670_CTL_GAM5, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x74;
	buf[1] = 0x80;
	buf[2] = 0x8c;
	buf[3] = 0x9b;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 4,
			OV7670_CTL_GAM9, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0xb2;
	buf[1] = 0xcc;
	buf[2] = 0xe5;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 3,
			OV7670_CTL_GAM13, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x24;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_SLOP, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0xc0;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_COM11, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0xc0;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_COM11, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x76;
	buf[1] = 0x65;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 2,
			OV7670_CTL_HAECC1, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0xe7;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_COM8, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	/*
	 * Phase 3
	 */
	buf[0] = 0x0a;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_DBLV, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x80;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_CLKRC, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x00;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_EXHCH, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x00;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_EXHCL, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x00;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_DM_LNL, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;
	buf[0] = 0x00;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_DM_LNH, dev->sensor_flags, buf);
	if (ret < 0)
		goto err;

	return ret;
/*
 * Error Handler
 */
err:
	UDIA_ERROR("R/W for sensor register failed. Ret code = %d\n", ret);
	return ret;
}

/**
 * @brief OV7670 Auto-Flip
 *
 * @param dev Pointer to the device
 * @param vflip Flag to indicate whether or not Camera is currently flipped
 *
 * @return Zero (success) or negative (USB-error value)
 *
 */
int ov7670_auto_flip(struct usb_microdia *dev, __u8 vflip)
{
	int ret;
	__u8 buf[2];

	ret = sn9c20x_read_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_MVFP, dev->sensor_flags, buf);
	if (ret < 0)
		return ret;

	if (vflip == 0)
		buf[0] = buf[0] & (0xff ^ OV7670_VFLIP_BIT);
	else
		buf[0] = buf[0] | OV7670_VFLIP_BIT;
	ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
			OV7670_CTL_MVFP, dev->sensor_flags, buf);

	return ret;
}
