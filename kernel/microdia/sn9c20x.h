/**
 * @file sn9c20x.h
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

#ifndef SN9C20X_H
#define SN9C20X_H

#define SN9C20X_I2C_400KHZ	0x01
#define SN9C20X_I2C_READ	0x02
#define SN9C20X_I2C_READY	0x04
#define SN9C20X_I2C_ERROR	0x08
#define SN9C20X_I2C_2WIRE	0x80

/* registers */
#define SN9C20X_HSTART		0x10fb
#define SN9C20X_HSIZE		0x10fc
#define SN9C20X_VSTART		0x10fd
#define SN9C20X_VSIZE		0x10fe
#define SN9C20X_SHARPNESS	0x10f6
#define SN9C20X_RED_GAIN	0x118c
#define SN9C20X_GREEN1_GAIN	0x118d
#define SN9C20X_GREEN2_GAIN	0x118e
#define SN9C20X_BLUE_GAIN	0x118f
#define SN9C20X_SCALE		0x1189

#define SN9C20X_NO_SCALE	0x00
#define SN9C20X_1_2_SCALE	0x10
#define SN9C20X_1_4_SCALE	0x20

int sn9c20x_initialize(struct usb_microdia *dev);
int sn9c20x_i2c_initialize(struct usb_microdia *dev);

int sn9c20x_write_i2c_data(struct usb_microdia *dev, __u8 slave, __u8 nbytes,
	__u8 address, __u8 flags, const __u8 [nbytes]);

int sn9c20x_read_i2c_data(struct usb_microdia *dev, __u8 slave, __u8 nbytes,
	__u8 address, __u8 flags, __u8 *result);

int sn9c20x_write_i2c_data16(struct usb_microdia *dev, __u8 slave, __u8 datalen,
	__u8 address, __u8 flags, const __u16 [datalen]);

int sn9c20x_read_i2c_data16(struct usb_microdia *dev, __u8 slave, __u8 datalen,
	__u8 address, __u8 flags, __u16 *result);

int sn9c20x_set_contrast(struct usb_microdia *dev);

int sn9c20x_set_brightness(struct usb_microdia *dev);

int sn9c20x_set_gamma(struct usb_microdia *dev);

int sn9c20x_set_sharpness(struct usb_microdia *dev);

int sn9c20x_set_rgb_gain(struct usb_microdia *dev);

int sn9c20x_set_resolution(struct usb_microdia *dev,
	int width, int height);

int sn9c20x_get_closest_resolution(int *width, int *height);

int sn9c20x_set_exposure(struct usb_microdia *dev);

#endif
