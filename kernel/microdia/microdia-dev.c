/**
 * @file microdia-dev.c
 * @author Nicolas VIVIEN
 * @date 2008-02-01
 *
 * @brief Device specific functions
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

#include <linux/errno.h>
#include <linux/string.h>
#include <stdarg.h>

#include "microdia.h"
#include "sn9c20x.h"
#include "ov7670.h"
#include "ov965x.h"
#include "mt9vx11.h"
#include "ov7660.h"

/**
 * @brief Wrapper function for camera-setting functions
 *
 * @param dev Pointer to device structure
 *
 * @returns 0
 */
int dev_microdia_camera_settings(struct usb_microdia *dev)
{
	dev_microdia_camera_set_contrast(dev);
	dev_microdia_camera_set_brightness(dev);
	dev_microdia_camera_set_gamma(dev);
	dev_microdia_camera_set_exposure(dev);
	dev_microdia_camera_set_hvflip(dev);
	dev_microdia_camera_set_sharpness(dev);
	dev_microdia_camera_set_rgb_gain(dev);
	dev_microdia_camera_set_auto_exposure(dev);
	dev_microdia_camera_set_auto_whitebalance(dev);
	return 0;
}

/**
 * @brief Wrapper function for device-specific contrast functions
 *
 * @param dev Pointer to device structure
 *
 * @returns 0 or negative error value
 *
 * This will execute a function determined by the array #cameras and
 * the function find_camera().
 *
 */
int dev_microdia_camera_set_contrast(struct usb_microdia *dev)
{
	int ret = -ENODEV;
	if (dev && dev->set_contrast)
		ret = dev->set_contrast(dev);
	return ret;
}

/**
 * @brief Wrapper function for device-specific brightness functions
 *
 * @param dev Pointer to device structure
 *
 * @returns 0 or negative error value
 *
 * This will execute a function determined by the array #cameras and
 * the function find_camera().
 *
 */
int dev_microdia_camera_set_brightness(struct usb_microdia *dev)
{
	int ret = -ENODEV;
	if (dev && dev->set_brightness)
		ret = dev->set_brightness(dev);
	return ret;
}

/**
 * @brief Wrapper function for device-specific gamma functions
 *
 * @param dev Pointer to device structure
 *
 * @returns 0 or negative error value
 *
 * This will execute a function determined by the array #cameras and
 * the function find_camera().
 *
 */
int dev_microdia_camera_set_gamma(struct usb_microdia *dev)
{
	int ret = -ENODEV;
	if (dev && dev->set_gamma)
		ret = dev->set_gamma(dev);
	return ret;
}

/**
 * @brief Wrapper function for device-specific exposure functions
 *
 * @param dev Pointer to device structure
 *
 * @returns 0 or negative error value
 *
 * This will execute a function determined by the array #cameras and
 * the function find_camera().
 *
 */
int dev_microdia_camera_set_exposure(struct usb_microdia *dev)
{
	if (dev->set_exposure != NULL)
		return dev->set_exposure(dev);

	return 0;
}

/**
 * @brief Wrapper function for device-specific hvflip functions
 *
 * @param dev Pointer to device structure
 *
 * @returns 0 or negative error value
 *
 * This will execute a function determined by the array #cameras and
 * the function find_camera().
 *
 */
int dev_microdia_camera_set_hvflip(struct usb_microdia *dev)
{
	int ret = -ENODEV;
	if (dev && dev->set_hvflip)
		ret = dev->set_hvflip(dev);

	return ret;
}

/**
 * @brief Wrapper function for device-specific sharpness functions
 *
 * @param dev Pointer to device structure
 *
 * @returns 0 or negative error value
 *
 * This will execute a function determined by the array #cameras and
 * the function find_camera().
 *
 */
int dev_microdia_camera_set_sharpness(struct usb_microdia *dev)
{
	int ret = -ENODEV;
	if (dev && dev->set_sharpness)
		ret = dev->set_sharpness(dev);

	return ret;
}

/**
 * @brief Wrapper function for device-specific rgb-gain functions
 *
 * @param dev Pointer to device structure
 *
 * @returns 0 or negative error value
 *
 * This will execute a function determined by the array #cameras and
 * the function find_camera().
 *
 */
int dev_microdia_camera_set_rgb_gain(struct usb_microdia *dev)
{
	int ret = -ENODEV;
	if (dev && dev->set_rgb_gain)
		ret = dev->set_rgb_gain(dev);

	return ret;
}

/**
 * @brief Wrapper function for device-specific auto-exposure functions
 *
 * @param dev Pointer to device structure
 *
 * @returns 0 or negative error value
 *
 * This will execute a function determined by the array #cameras and
 * the function find_camera().
 *
 */
int dev_microdia_camera_set_auto_exposure(struct usb_microdia *dev)
{
	int ret = -ENODEV;
	if (dev && dev->set_auto_exposure)
		ret = dev->set_auto_exposure(dev);

	return ret;
}

/**
 * @brief Wrapper function for device-specific auto-whitebalance functions
 *
 * @param dev Pointer to device structure
 *
 * @returns 0 or negative error value
 *
 * This will execute a function determined by the array #cameras and
 * the function find_camera().
 *
 */
int dev_microdia_camera_set_auto_whitebalance(struct usb_microdia *dev)
{
	int ret = -ENODEV;
	if (dev && dev->set_auto_whitebalance)
		ret = dev->set_auto_whitebalance(dev);

	return ret;
}


/**
 * @brief Wrapper function for device-specific initialization functions
 *
 * @param dev Pointer to device structure
 *
 * @returns 0 or negative error value
 *
 * This will execute a function determined by the array #cameras and
 * the function find_camera().
 *
 */
int dev_microdia_initialize_device(struct usb_microdia *dev)
{
	int ret = -ENODEV;

	if (dev && dev->initialize)
		ret = dev->initialize(dev);

	return ret;
}

/**
 * @brief From init-physical-replug.log
 *
 * @param dev
 *
 * @returns 0
 *
 * @author psnel
 *
 * For SN9C201 with MI1300.
 *
 */
int microdia_6240_initialize(struct usb_microdia *dev)
{
	return 0;
}

/**
 * @brief From start.log (14 jun 2008)
 *
 * @param dev
 *
 * @returns 0
 *
 * @author psnel
 *
 * For SN9C201 with MI1300.
 *
 */
int microdia_6240_start_stream(struct usb_microdia *dev)
{
	int ret = 0;
	int i;
	__u16 reg;
	__u8 value[1];

	/* Setup the bridge registers */
	static __u16 bridge6240_init[][2] = {
								{0x0395, 0x05},
								{0x1000, 0x7c},
								{0x1001, 0x44},
								{0x1002, 0x1c},
								{0x1006, 0x80},
								{0x1007, 0xa0},
								{0x1020, 0x80},
								{0x1066, 0x01},
								{0x1067, 0x40},
								{0x1068, 0x30},
								{0x1069, 0x20},
								{0x106a, 0x10},
								{0x106b, 0x08},
								{0x1070, 0x10},
								{0x1071, 0x08},
								{0x10e0, 0x07},
								{0x10f5, 0x60},
								{0x10f7, 0x07},
								{0x10f8, 0x14},
								{0x10f9, 0x00},
								{0x10fa, 0xff},
								{0x1100, 0x0d},
								{0x1101, 0x08},
								{0x1102, 0x08},
								{0x1103, 0x0d},
								{0x1104, 0x08},
								{0x1105, 0x08},
								{0x1106, 0x0d},
								{0x1107, 0x0d},
								{0x1108, 0x0d},
								{0x1109, 0x0d},
								{0x110a, 0x11},
								{0x110b, 0x0d},
								{0x110c, 0x0d},
								{0x110d, 0x11},
								{0x110e, 0x15},
								{0x110f, 0x21},
								{0x1110, 0x15},
								{0x1111, 0x15},
								{0x1112, 0x11},
								{0x1113, 0x11},
								{0x1114, 0x15},
								{0x1115, 0x2a},
								{0x1116, 0x1d},
								{0x1117, 0x1d},
								{0x1118, 0x19},
								{0x1119, 0x21},
								{0x111a, 0x32},
								{0x111b, 0x2a},
								{0x111c, 0x32},
								{0x111d, 0x32},
								{0x111e, 0x2e},
								{0x111f, 0x2a},
								{0x1120, 0x2e},
								{0x1121, 0x2e},
								{0x1122, 0x36},
								{0x1123, 0x3a},
								{0x1124, 0x4b},
								{0x1125, 0x43},
								{0x1126, 0x36},
								{0x1127, 0x3a},
								{0x1128, 0x47},
								{0x1129, 0x3a},
								{0x112b, 0x2e},
								{0x112c, 0x43},
								{0x112d, 0x5c},
								{0x112e, 0x43},
								{0x112f, 0x47},
								{0x1130, 0x4f},
								{0x1131, 0x54},
								{0x1132, 0x58},
								{0x1133, 0x58},
								{0x1134, 0x58},
								{0x1135, 0x32},
								{0x1136, 0x3f},
								{0x1137, 0x60},
								{0x1138, 0x64},
								{0x1139, 0x5c},
								{0x113a, 0x54},
								{0x113b, 0x64},
								{0x113c, 0x4b},
								{0x113d, 0x54},
								{0x113e, 0x58},
								{0x113f, 0x54},
								{0x1140, 0x0d},
								{0x1141, 0x11},
								{0x1142, 0x11},
								{0x1143, 0x15},
								{0x1144, 0x11},
								{0x1145, 0x15},
								{0x1146, 0x26},
								{0x1147, 0x15},
								{0x1148, 0x15},
								{0x1149, 0x26},
								{0x114a, 0x54},
								{0x114b, 0x36},
								{0x114c, 0x2e},
								{0x114d, 0x36},
								{0x114e, 0x54},
								{0x114f, 0x54},
								{0x1150, 0x54},
								{0x1151, 0x54},
								{0x1152, 0x54},
								{0x1153, 0x54},
								{0x1154, 0x54},
								{0x1155, 0x54},
								{0x1156, 0x54},
								{0x1157, 0x54},
								{0x1158, 0x54},
								{0x1159, 0x54},
								{0x115a, 0x54},
								{0x115b, 0x54},
								{0x115c, 0x54},
								{0x115d, 0x54},
								{0x115e, 0x54},
								{0x115f, 0x54},
								{0x1160, 0x54},
								{0x1161, 0x54},
								{0x1162, 0x54},
								{0x1163, 0x54},
								{0x1164, 0x54},
								{0x1165, 0x54},
								{0x1166, 0x54},
								{0x1167, 0x54},
								{0x1168, 0x54},
								{0x1169, 0x54},
								{0x116a, 0x54},
								{0x116b, 0x54},
								{0x116c, 0x54},
								{0x116d, 0x54},
								{0x116e, 0x54},
								{0x116f, 0x54},
								{0x1170, 0x54},
								{0x1171, 0x54},
								{0x1172, 0x54},
								{0x1173, 0x54},
								{0x1174, 0x54},
								{0x1175, 0x54},
								{0x1176, 0x54},
								{0x1177, 0x54},
								{0x1178, 0x54},
								{0x1179, 0x54},
								{0x117a, 0x54},
								{0x117b, 0x54},
								{0x117c, 0x54},
								{0x117d, 0x54},
								{0x117e, 0x54},
								{0x117f, 0x54},
								{0x1180, 0x02},
								{0x1181, 0x00},
								{0x1182, 0x02},
								{0x1183, 0x00},
								{0x1184, 0x28},
								{0x1185, 0x3c},
								{0x1188, 0x06},
								{0x118a, 0x05},
								{0x118b, 0x0a},
								{0x118c, 0x20},
								{0x118d, 0x20},
								{0x118e, 0x20},
								{0x118f, 0x20},
								{0x1190, 0x00},
								{0x1191, 0x24},
								{0x1192, 0x3b},
								{0x1193, 0x4f},
								{0x1194, 0x61},
								{0x1195, 0x71},
								{0x1196, 0x80},
								{0x1197, 0x8f},
								{0x1198, 0x9d},
								{0x1199, 0xab},
								{0x119a, 0xab},
								{0x119b, 0xb8},
								{0x119c, 0xc4},
								{0x119d, 0xd1},
								{0x119e, 0xdd},
								{0x119f, 0xe9},
								{0x11a0, 0xff},
								{0x11a1, 0x00},
								{0x11a2, 0x00},
								{0x11a3, 0x6a},
								{0x11a4, 0x50},
								{0x11a5, 0x2d},
								{0x11a6, 0x2d},
								{0x11a7, 0x3a},
								{0x11a8, 0x05},
								{0x11a9, 0x04},
								{0x11aa, 0x3f},
								{0x11ab, 0x00},
								{0x11ac, 0x00},
								{0x11ad, 0x50},
								{0x11ae, 0x3c},
								{0x11af, 0x28},
								{0x11b0, 0xd8},
								{0x11b1, 0x14},
								{0x11b2, 0xec},
								{0x11b3, 0x32},
								{0x11b4, 0xdd},
								{0x11b5, 0x2d},
								{0x11b6, 0xdd},
								{0x11b7, 0x60},
								{0x11b8, 0x78},
								{0x11b9, 0x00},
								{0x11ba, 0x0b},
								{0x11bb, 0x00},
								{0x11bc, 0x00},
								{0x11bd, 0x00},
								{0x11be, 0x00},
								{0x11bf, 0x00},
								{0x11c0, 0x00},
								{0x11c1, 0x00},
								{0x11c2, 0x00},
								{0x11c3, 0x00},
								{0x11c4, 0x00},
								{0x11c5, 0x00},
								{0x11c6, 0x00},
								{0x11c7, 0x00},
								{0x11c8, 0x00},
								{0x11c9, 0x00},
								{0x11ca, 0x00},
								{0x11cb, 0x00},
								{0x11cc, 0x00},
								{0x11cd, 0x00},
								{0x11ce, 0x00},
								{0x11cf, 0x00},
								{0x11d0, 0x00},
								{0x11d1, 0x00},
								{0x11d2, 0x00},
								{0x11d3, 0x00},
								{0x11d4, 0x00},
								{0x11d5, 0x00},
								{0x11d6, 0x00},
								{0x11d7, 0x00},
								{0x11d8, 0x00},
								{0x11d9, 0x00},
								{0x11da, 0x00},
								{0x11db, 0x00},
								{0x11dc, 0x00},
								{0x11dd, 0x00},
								{0x11de, 0x00},
								{0x11df, 0x00},
								{0x11e0, 0x0b},
								{0x11e1, 0x00},
								{0x11e2, 0x00},
								{0x11e3, 0x00},
								{0x11e4, 0x00},
								{0x11e5, 0x00},
								{0x11e6, 0x00},
								{0x11e7, 0x00},
								{0x11e8, 0x00},
								{0x11e9, 0x00},
								{0x11ea, 0x00},
								{0x11eb, 0x00},
								{0x11ec, 0x00},
								{0x11ed, 0x00},
								{0x11ee, 0x00},
								{0x11ef, 0x00},
								{0x11fb, 0x00},
								{0x11fc, 0xa0},
								{0x11fd, 0x00},
								{0x11fe, 0xf0},
								{0x11ff, 0x00},
								{0x1200, 0xf4},
								{0x1201, 0xff},
	};

	static __u8 mi1300_init[][2] = {
							{0x00, 0x00},
							{0x01, 0x00},
							{0x02, 0x0e},
							{0x03, 0x03},
							{0x04, 0xc1},
							{0x05, 0x00},
							{0x06, 0x13},
							{0x07, 0xf2},
							{0x08, 0x02},
							{0x09, 0x00},
							{0x0a, 0x00},
							{0x0b, 0x00},
							{0x0c, 0x00},
							{0x0d, 0x00},
							{0x0e, 0x00},
							{0x11, 0x00},
							{0x12, 0x00},
							{0x1e, 0x80},
							{0x1f, 0x00},
							{0x20, 0x11},
							{0x21, 0x1d},
							{0x2b, 0x00},
							{0x2c, 0x00},
							{0x2d, 0x1d},
							{0x2e, 0x00},
							{0x2f, 0x1d},
							{0x30, 0x10},
							{0x5f, 0x89},
							{0x60, 0x00},
							{0x61, 0x00},
							{0x62, 0x00},
							{0x63, 0x98},
							{0x64, 0x00},
							{0x65, 0x00},
							{0x68, 0x00},
							{0x69, 0x00},
	};

	/* initialize SN9C201 for 6240 */
	for (i = 0; i < ARRAY_SIZE(bridge6240_init); i++) {
		reg = bridge6240_init[i][0];
		value[0] = bridge6240_init[i][1];
		ret = usb_microdia_control_write(dev, reg, value, 1);
		if (ret < 0) {
			UDIA_INFO("6240 Bridge: Inititialization Error (%d) on array line %d\n", ret, i);
			break;
		}
	}

	/* initialize the Bridge to I2C interface */
	ret = sn9c20x_i2c_initialize(dev);

	/* initialize the Sensor MI 1300 */
	for (i = 0; i < ARRAY_SIZE(mi1300_init); i++) {
		reg = mi1300_init[i][0];
		value[0] = mi1300_init[i][1];
		ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
					     reg, dev->sensor_flags, value);
		if (ret < 0) {
			UDIA_INFO("Sensor MI 1300: Inititialization Error (%d) on array line %d\n", ret, i);
			break;
		}
	}

	UDIA_INFO("...start function completed without errors.\n");
	return ret;
}

/**
 * @brief From stop-close-amcap.log
 *
 * @param dev
 *
 * @returns 0
 *
 * @author psnel
 *
 * For SN9C201 with MI1300. Comments from parser-usbsnoop(updated_13Mar2008).pl output
 *
 */
int microdia_6240_stop_stream(struct usb_microdia *dev)
{
	int ret = 0;
	int i;
	__u16 reg;
	__u8  value[1];

	static __u8 mi1300_stop[][2] = {
	{0x07, 0x00},
	{0x08, 0x00},
	};

	static __u16 bridge6240_stop[][2] = {
	{0x1000, 0x3c},
	{0x11b8, 0x68},
	{0x1007, 0xa0},
	{0x1006, 0xa0},
	{0x1000, 0x98},
	{0x1001, 0xef},
	{0x1002, 0x11},
	{0x1066, 0x00},
	};

	UDIA_INFO("Stopping camera...\n");

	for (i = 0; i < ARRAY_SIZE(mi1300_stop); i++) {
		reg = mi1300_stop[i][0];
		value[0] = mi1300_stop[i][1];
		ret = sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 1,
					     reg, dev->sensor_flags, value);
		if (ret < 0) {
			UDIA_INFO("Sensor MI 1300: Stop Sequence Error (%d) on array line %d\n", ret, i);
			break;
		}
	}
    
	ret = usb_microdia_control_read(dev, 0x1061, value, 1);
	if (ret < 0)
		return ret;

	value[0] &= ~0x02;
	ret = usb_microdia_control_write(dev, 0x1061, value, 1);
	if (ret < 0)
		return ret;

	for (i = 0; i < ARRAY_SIZE(bridge6240_stop); i++) {
		reg = bridge6240_stop[i][0];
		value[0] = bridge6240_stop[i][1];
		ret = usb_microdia_control_write(dev, reg, value, 1);
		if (ret < 0) {
			UDIA_INFO("6240 Bridge: Stop Sequence Error (%d) on array line %d\n", ret, i);
			break;
		}
	}

	return ret;
}

/**
 * @brief From init.sequence.log
 *
 * @param dev
 *
 * @returns 0
 *
 * @author GWater
 *
 * For SN9C201 with SOI968.
 * Tests showed this function is actually unnecessary.
 * Nevertheless the reads may give us valuable values we don't know about yet.
 */
int microdia_624e_initialize(struct usb_microdia *dev)
{
	__u8 buf;

	usb_microdia_control_read(dev, 0x130d, &buf, 1);
	usb_microdia_control_read(dev, 0x1040, &buf, 1);

	return 0;
}

/**
 * @brief From UsbSnoop-plugin-parsed.log
 *
 * @param dev
 *
 * @returns
 */
int microdia_624f_initialize(struct usb_microdia *dev)
{
	/* Do nothing here */
	return 0;
}

/**
 * @brief From UsbSnoop-plugin-parsed.log
 *
 * @param dev
 *
 * @returns
 */
int microdia_6260_initialize(struct usb_microdia *dev)
{
	int ret = 0;
	int i;
	__u16 reg;
	__u8  value[1];

	static __u16 bridge6260_init[][2] = {
	{0x10c8, 0x00},
	{0x100a, 0x04},
	{0x100b, 0x04},
	{0x1001, 0xc7},
	{0x1040, 0x00},
	{0x1045, 0x04},
	{0x1046, 0x10},
	{0x1045, 0x14},
	{0x1040, 0x01},
	{0x1020, 0x80},
	};

	for (i = 0; i < ARRAY_SIZE(bridge6260_init); i++) {
		reg = bridge6260_init[i][0];
		value[0] = bridge6260_init[i][1];
		ret = usb_microdia_control_write(dev, reg, value, 1);
		if (ret < 0) {
			UDIA_INFO("6260 Bridge: Init Sequence Error (%d) on array line %d\n", ret, i);
			break;
		}
	}

	return 0;
}

/**
 * @brief From 6 init-logs
 *
 * @param dev
 *
 * @returns 0 (ok) or -1 (error)
 *
 * @author Comer352l
 *
 * Windows driver versions: 5.5.8.12 and 5.7.23.000
 * Windows versions: 2000 and XP
 * USB-Logger: SniffUSB 1.8 and 2.0, USBTrace 2.0, SnoopyPro
 *
 * Comments behind read oprations contain HEX-values that appeared in the logs
 */
int microdia_6270_initialize(struct usb_microdia *dev)
{
	/* int actual; */
	int ret = 0;
	int retSP = 0;
	int retASS = 0;
	__u8 buf[8];
	__u8 buf_0395[1];
	__u8 buf_1000[5];
	__u8 buf_1060[2];
	__u8 buf_11b8[1];

	ret += usb_microdia_control_read(dev, 0x130d, buf, 1);		/*  URB 5	0x81,0x85,0x04,0x8b,0x02,... */

	ret += usb_microdia_control_read(dev, 0x1040, buf, 1);		/*  URB 6	0xe4,0xc4,0xa4,0x44,... */

	/*
	 * THE OLD DRIVER 5.5.8.12 DOES AN ADDITIONAL WRITE HERE:
	 * buf[0]=0x00;
	 * usb_microdia_control_write(dev, 0x10c8, buf, 1);
	*/

	ret += usb_microdia_control_read(dev, 0x118a, buf, 1);		/*  URB 7	0x05,0x7b,0x01,0x04,... */

	ret += usb_microdia_control_read(dev, 0x0395, buf_0395, 1);	/*  URB 8	0xc2,0x82,0x38,0x98,0x04,... */
	/*  => this value will be rewritten to 0x118a and 0x0395 later (URBs 81+82) */

	buf[0] = 0x05;
	ret += usb_microdia_control_write(dev, 0x118a, buf, 1);		/*  URB 9 */

	ret += usb_microdia_control_write(dev, 0x0395, buf, 1);		/*  URB 10 */

	ret += usb_microdia_control_read(dev, 0x11b8, buf_11b8, 1);	/*  URB 11	0xa0,0x00,0x24,... */
	/*  => this value will be rewritten to 0x11b8 later (URB 83) */

	buf[0] = 0x10;
	ret += usb_microdia_control_write(dev, 0x11b8, buf, 1);		/*  URB 12 */

	ret += usb_microdia_control_read(dev, 0x1000, buf_1000, 5);	/*  URB 13	0x30 0xCF 0x00 0x03 (0x02 or 0x00) */
	/*  => these values will be rewritten to 0x1000 later (URB 84) */

	buf[0] = 0x70; buf[1] = 0x00; buf[2] = 0x18; buf[3] = 0x00; buf[4] = 0x00;
	ret += usb_microdia_control_write(dev, 0x1000, buf, 5);		/*  URB 14 */

	ret += usb_microdia_control_read(dev, 0x1060, buf_1060, 2);	/*  URB 15	0x08 0x00 (always  ???) */
	/*  => this value will be rewritten to 0x1060 later (URB 85) */

	buf[0] = 0x00; buf[1] = 0x03;
	ret += usb_microdia_control_write(dev, 0x1060, buf, 2);		/*  URB 16 */

	/*  initialize I2C registers to avoid getting no ACK at first I2C operation: */
	ret += sn9c20x_i2c_initialize(dev);

	/*  enable I2C communication (bit 0) */
	buf[0] = 0x21;	/*  THE OLD DRIVER 5.5.8.12 WRITES 0x5d */
	ret += usb_microdia_control_write(dev, 0x10c8, buf, 1);		/*  URB 17 */

	/*  probe sensor model: */
	retSP = mt9vx11_sensor_probe(dev);	/*  substitutes URBs 18-48 */
	if ((retSP == 0) && (dev->sensor_slave_address == MT9V111_I2C_SLAVE_ADDRESS))
		/*  switch (back) to IFP address space: */
		retASS = mt9v111_select_address_space(dev, MT9V111_ADDRESSSPACE_IFP);

	ret += usb_microdia_control_write(dev, 0x118a, buf_0395, 1);	/*  URB 81 => write value read with URB 8 */

	ret += usb_microdia_control_write(dev, 0x0395, buf_0395, 1);	/*  URB 82 => write value read with URB 8 */

	ret += usb_microdia_control_write(dev, 0x11b8, buf_11b8, 1);	/*  URB 83 => write value read with URB 11 */

	ret += usb_microdia_control_write(dev, 0x1000, buf_1000, 5);	/*  URB 84 => write value read with URB 13 */

	ret += usb_microdia_control_write(dev, 0x1060, buf_1060, 2);	/*  URB 85 => write value read with URB 15; (always 08 00 ???) */

	/*  USB-interrupt-message:	doesn't work yet (returns error -110) */
	/* usb_interrupt_msg(dev->udev, usb_rcvintpipe(dev->udev, dev->int_in_endpointAddr), dev->int_in_buffer, dev->int_in_size, &actual, 1000);	// URB 86 */

	/*
	 * THE OLD DRIVER 5.5.8.12 DOES AN ADDITIONAL WRITE HERE:
	 * wbuf[0]=0x80;
	 * usb_microdia_control_write(dev, 0x1020, wbuf, 1);
	*/

	if ((retSP != 0) || (retASS != 0) || (ret < 33)) {
		if (ret < 33)
			UDIA_INFO("One ore more errors occured during initialization !\n");
		return -1;
	} else {
		return 0;
	}
}


/**
 * @brief From UsbSnoop-plugin-parsed.log
 *
 * @param dev
 *
 * @returns
 */
int microdia_627b_initialize(struct usb_microdia *dev)
{
	/* Apparently we don't need to write anything here */
	return 0;
}

/**
 * @brief From init-start-stop-log (made by Víctor M. Hernández Rocamora)
 *
 * @param dev
 *
 * @returns 0 (ok) or -1 (error)
 *
 * @author Comer352l
 *
 * USB-Logger: SniffUSB 2.0
 *
 * Comments behind read oprations contain HEX-values that appeared in the log
 */
int microdia_627f_initialize(struct usb_microdia *dev)
{
	__u8 buf[1];
	int ret = 0;

	ret += usb_microdia_control_read(dev, 0x130d, buf, 1);				/* URB 5	0x81 */

	ret += usb_microdia_control_read(dev, 0x1040, buf, 1);				/* URB 6	0xe4 */

	buf[0] = 0x00;
	ret += usb_microdia_control_write(dev, 0x10c8, buf, 1);				/* URB 7 */

	buf[0] = 0x80;
	ret += usb_microdia_control_write(dev, 0x1020, buf, 1);				/* URB 8 */

	ret += usb_microdia_control_read(dev, 0x1045, buf, 1);				/* URB 9	0x0f */

	buf[0] = 0x1f;
	ret += usb_microdia_control_write(dev, 0x1045, buf, 1);				/* URB 10 */

	ret += usb_microdia_control_read(dev, 0x1045, buf, 1);				/* URB 11	0x1f */

	buf[0] = 0x0f;
	ret += usb_microdia_control_write(dev, 0x1045, buf, 1);				/* URB 12 */

	ret += usb_microdia_control_read(dev, 0x1045, buf, 1);				/* URB 13	0x0f */

	buf[0] = 0x1f;
	ret += usb_microdia_control_write(dev, 0x1045, buf, 1);				/* URB 14 */

	if (ret < 10) {
		UDIA_INFO("One ore more errors occured during initialization !\n");
		return -1;
	}
	return 0;
}

/**
 * @brief From UsbSnoop-plugin-parsed.log
 *
 * @param dev
 *
 * @returns 0 (OK) or <0 (Error)
 *
 * @author Vincent, Kuzja
 */
int microdia_6288_initialize(struct usb_microdia *dev)
{

	int ret;
	__u16 reg;
	__u8 buf[2];

	reg = 0x130d ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x1040 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	if (ret < 0)
		return ret;
	else
		return 0;
err:
	UDIA_ERROR("r/w in Register %x failed (%d)!\n", reg, ret);
	return ret;
}

/**
 * @brief Wrapper function for device-specific startstream functions
 *
 * @param dev Pointer to device structure
 *
 * @returns 0
 *
 * This will execute a function determined by the array #cameras and
 * the function find_camera().
 *
 */
int dev_microdia_start_stream(struct usb_microdia *dev)
{
	int ret = -ENODEV;
	if (dev && dev->start_stream)
		ret = dev->start_stream(dev);

	return ret;
}

extern int jpeg;

/**
 * @brief From start.htm
 *
 * @param dev
 *
 * @returns 0
 *
 * @author GWater
 *
 * For SN9C201 with MI1310.
 */
int microdia_6242_start_stream(struct usb_microdia *dev)
{
	int i = -1;
	__u8 buf[48];

	__u8 urb10[24] = {
		0x01, 0x18, 0x00, 0x30, 0x00, 0x09, 0x00, 0xed,
		0x0f, 0xda, 0x0f, 0x3a, 0x00, 0x3a, 0x00, 0xd0,
		0x0f, 0xf7, 0x0f, 0x00, 0x00, 0x00, 0x60, 0x0a};

	__u8 urb13[11] = {
		0x02, 0x00, 0x02, 0x00, 0x50, 0x78, 0x00, 0x00,
		0x07};

	__u8 urb14[22] = {
		0x00, 0x20, 0x20, 0x20, 0x20, 0x00, 0x24, 0x3b,
		0x4f, 0x61, 0x71, 0x80, 0x8f, 0x9d, 0xab, 0xb8,
		0xc4, 0xd1, 0xdd, 0xe9, 0xf4, 0xff};

	__u8 qtable1[64] = {
		0x0d, 0x08, 0x08, 0x0d, 0x08, 0x08, 0x0d, 0x0d,
		0x0d, 0x0d, 0x11, 0x0d, 0x0d, 0x11, 0x15, 0x21,
		0x15, 0x15, 0x11, 0x11, 0x15, 0x2a, 0x1d, 0x1d,
		0x19, 0x21, 0x32, 0x2a, 0x32, 0x32, 0x2e, 0x2a,
		0x2e, 0x2e, 0x36, 0x3a, 0x4b, 0x43, 0x36, 0x3a,
		0x47, 0x3a, 0x2e, 0x2e, 0x43, 0x5c, 0x43, 0x47,
		0x4f, 0x54, 0x58, 0x58, 0x58, 0x32, 0x3f, 0x60,
		0x64, 0x5c, 0x54, 0x64, 0x4b, 0x54, 0x58, 0x54};

	__u8 qtable2[64] = {
		0x0d, 0x11, 0x11, 0x15, 0x11, 0x15, 0x26, 0x15,
		0x15, 0x26, 0x54, 0x36, 0x2e, 0x36, 0x54, 0x54,
		0x54, 0x54, 0x54, 0x54, 0x54, 0x54, 0x54, 0x54,
		0x54, 0x54, 0x54, 0x54, 0x54, 0x54, 0x54, 0x54,
		0x54, 0x54, 0x54, 0x54, 0x54, 0x54, 0x54, 0x54,
		0x54, 0x54, 0x54, 0x54, 0x54, 0x54, 0x54, 0x54,
		0x54, 0x54, 0x54, 0x54, 0x54, 0x54, 0x54, 0x54,
		0x54, 0x54, 0x54, 0x54, 0x54, 0x54, 0x54, 0x54};

        // Sequence of writes, address, followed by data (MSB, LSB)
	__u8 first10c0[][3] = {
          // Start at page 0.  (Datasheet p. 36)
          {0x0d, 0x00, 0x08},  // reset
          {0x0d, 0x00, 0x09},  // reset
          {0x0d, 0x00, 0x08},  // reset
          // Change to page 1
          {0xf0, 0x00, 0x01},  // page=1
          {0x3a, 0x73, 0x00},  // R58:1, output format = 0x7300 (RGB565)
          //{0x9b, 0x73, 0x00},
          // R6:1 was 0x701c in Dave Neuer's code
          //{0x06, 0x30, 0x8c},  // R6:1 = 0x308c  auto exposure off
          // R6:1
          // bit 15: enable manual white balance
          // bit 14: enable auto exposure
          // bit 11: load white balance
          // bit 7: flicker detection
          // bit 4: bypass color correction
          // bits 3:2: Back light compensation
          // bit 1: enable AWB

          // Old values
          {0x06, 0x30, 0x8c},  // R6:1 = 0x308c  auto exposure off, auto white balance
          //{0x06, 0x70, 0x8c},  // auto exposure, auto white balance
          
          //{0x25, 0x00, 0x2a},
          // Change back to page 0
          {0xf0, 0x00, 0x00}
        };

	__u8 second10c0a[2][5] = {
          {0x01, 0x00, 0x0e, 0x00, 0x14}, // row start = 0x000e (14);  col start = 0x0014 (20)
          {0x03, 0x03, 0xc4, 0x05, 0x14}  // window height = 0x03c4 (964);  window width = 0x0514 (1300)
        };

	__u8 second10c0b[3] =
          {0xc8, 0x00, 0x03}; // R200:0 0x0003 set horiz, vertical blanking to context B

	__u8 third10c0[4][3] = {
          {0x0a, 0x00, 0x01}, // R10:0  0x0001.  pixel clock speed
          {0x06, 0x00, 0x29}, // R6:0   0x0029.  vertical blanking
          {0x05, 0x00, 0x72}, // R5:0   0x0072.  horizontal blanking
          {0x20, 0x00, 0x00}  // R32:0  0x0000.  read mode
        };
        
	__u8 fourth10c0a[3][3] = {
          {0x20, 0x00, 0x00}, // R32:0  0x0000.  read mode
          {0x09, 0x01, 0x90}, // R9:0   0x0190.  shutter width
          {0x0d, 0x80, 0x08}  // R13:0  0x8008.  reset.  MSB means hold off updating image settings
        };

	__u8 fourth10c0b[2][5] = {
          {0x2b, 0x01, 0x88, 0x01, 0x88}, // R43:0  0x0188.  green1 gain (default is 0x0020)
                                          // R44:0  0x0188.  blue gain   (default is 0x0020)
          {0x2d, 0x01, 0x88, 0x01, 0x88}  // R45:0  0x0188.  red gain    (default is 0x0020)
                                          // R46:0  0x0188.  green2 gain (default is 0x0020)
		};

	__u8 fifth10c0[4][3] = {
          {0x0a, 0x00, 0x01}, // R10:0  0x0001.  pixel clock speed
          {0x06, 0x00, 0x29}, // R6:0   0x0029.  vertical blanking
          {0x05, 0x00, 0x72}  // R5:0   0x0072.  horizontal blanking
		};

	/* interrupt down */

	buf[0] = 0x00;
	usb_microdia_control_write(dev, 0x1066, buf, 1);				/*  URB 1 */

	/* interrupt up */
	/* interrupt down */

	buf[0] = 0x60;
	usb_microdia_control_write(dev, 0x1007, buf, 1);

	buf[0] = 0x00;
	usb_microdia_control_write(dev, 0x1006, buf, 1);

	buf[0] = 0x78;
	usb_microdia_control_write(dev, 0x1000, buf, 1);

	buf[0] = 0xc7;
	usb_microdia_control_write(dev, 0x1001, buf, 1);

	buf[0] = 0x1c;
	usb_microdia_control_write(dev, 0x1002, buf, 1);

	buf[0] = 0x80;
	usb_microdia_control_write(dev, 0x1020, buf, 1);

	buf[0] = 0x40; buf[1] = 0x30; buf[2] = 0x20;
	buf[3] = 0x10; buf[4] = 0x08;
	usb_microdia_control_write(dev, 0x1067, buf, 5);

	buf[0] = 0xb0; buf[1] = dev->sensor_slave_address; buf[2] = 0x0d;
	buf[3] = 0x00; buf[4] = 0x00; buf[5] = 0x00;
	buf[6] = 0x00; buf[7] = 0x10; buf[8] = 0x03;
	usb_microdia_control_write(dev, 0x10c0, buf, 9);

	usb_microdia_control_write(dev, 0x10e0, urb10, 24);				/* URB 10 */

	buf[0] = 0x00; buf[1] = 0x00; buf[3] = 0x00;
	usb_microdia_control_write(dev, 0x10f8, buf, 3);

	usb_microdia_control_write(dev, 0x1180, urb13, 9);

	buf[0] = 0x0a;
	usb_microdia_control_write(dev, 0x118a, buf, 1);

	usb_microdia_control_write(dev, 0x118b, urb14, 22);

	buf[0] = 0x00; buf[1] = 0x00;
	usb_microdia_control_write(dev, 0x11a1, buf, 2);

	buf[0] = 0x60; buf[1] = 0x20; buf[2] = 0x00;
	buf[3] = 0x08; buf[4] = 0x00; buf[5] = 0x00;
	buf[6] = 0x00; buf[7] = 0x00; buf[8] = 0x00;
	usb_microdia_control_write(dev, 0x11b7, buf, 9);

	buf[0] = 0x30;
	usb_microdia_control_write(dev, 0x11b8, buf, 1);

	buf[0] = 0x78;
	usb_microdia_control_write(dev, 0x1000, buf, 1);

	for (i = 0; i < sizeof(first10c0)/sizeof(first10c0[0]); i++)
		sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 2,
			first10c0[i][0], dev->sensor_flags, &first10c0[i][1]);		/* URB 9-48 */

	buf[0] = 0x60;
	usb_microdia_control_write(dev, 0x1007, buf, 1);

	buf[0] = 0x00;
	usb_microdia_control_write(dev, 0x1006, buf, 1);

	buf[0] = 0x03;
	usb_microdia_control_write(dev, 0x10e0, buf, 1);				/* URB 50 */

	buf[0] = 0x03;
	usb_microdia_control_write(dev, 0x10e0, buf, 1);

	buf[0] = 0xc6;
	usb_microdia_control_write(dev, 0x1001, buf, 1);

	buf[0] = 0xc4;
	usb_microdia_control_write(dev, 0x1001, buf, 1);

    if(jpeg)
        buf[0] = 0x40;
    else
        buf[0] = 0x44;
	usb_microdia_control_write(dev, 0x1001, buf, 1);

	buf[0] = 0x00;
	usb_microdia_control_write(dev, 0x11bc, buf, 1);

	buf[0] = 0x00;
	usb_microdia_control_write(dev, 0x11bd, buf, 1);

	buf[0] = 0x00;
	usb_microdia_control_write(dev, 0x11be, buf, 1);

	buf[0] = 0x00;
	usb_microdia_control_write(dev, 0x11bf, buf, 1);

	sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 4,
		second10c0a[0][0], dev->sensor_flags, &second10c0a[0][1]);

	sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 4,
		second10c0a[1][0], dev->sensor_flags, &second10c0a[1][1]);

	sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 0,
		0x09, dev->sensor_flags, NULL);

	sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 2,
		second10c0b[0], dev->sensor_flags, &second10c0b[1]);

	buf[0] = 0x06; buf[1] = 0x00; buf[2] = 0x02;
	buf[3] = 0x00; buf[4] = 0x28; buf[5] = 0x3c;
	usb_microdia_control_write(dev, 0x1180, buf, 6);

	buf[0] = 0x00; buf[1] = 0x00; buf[2] = 0x6a; buf[3] = 0x50;
	usb_microdia_control_write(dev, 0x11a1, buf, 4);

	buf[0] = 0x00; buf[1] = 0x00; buf[2] = 0x50; buf[3] = 0x3c;
	usb_microdia_control_write(dev, 0x11ab, buf, 4);

	buf[0] = 0x78;
	usb_microdia_control_write(dev, 0x1000, buf, 1);

	buf[0] = 0x18;
	usb_microdia_control_write(dev, 0x1002, buf, 1);

	buf[0] = 0x18;
	usb_microdia_control_write(dev, 0x1002, buf, 1);

	buf[0] = 0x38;
	usb_microdia_control_write(dev, 0x11b8, buf, 1);

	buf[0] = 0x02;
	usb_microdia_control_write(dev, 0x118a, buf, 1);

	buf[0] = 0x02;
	usb_microdia_control_write(dev, 0x0395, buf, 1);

	buf[0] = 0x78;
	usb_microdia_control_write(dev, 0x11b8, buf, 1);				/* URB 80 */
	usb_microdia_control_read(dev, 0x11b8, buf, 1);

	buf[0] = 0xf9;
	usb_microdia_control_write(dev, 0x11b8, buf, 1);
	usb_microdia_control_read(dev, 0x11b8, buf, 1);

	buf[0] = 0xfa;
	usb_microdia_control_write(dev, 0x11b8, buf, 1);
	usb_microdia_control_read(dev, 0x11b8, buf, 1);

	buf[0] = 0x7b;
	usb_microdia_control_write(dev, 0x11b8, buf, 1);
	usb_microdia_control_read(dev, 0x11b8, buf, 1);

	buf[0] = 0x7c;
	usb_microdia_control_write(dev, 0x11b8, buf, 1);
	usb_microdia_control_read(dev, 0x11b8, buf, 1);

	buf[0] = 0x7b;
	usb_microdia_control_write(dev, 0x11b8, buf, 1);				/* URB 90 */

	for (i = 0; i < 4; i++)
		sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 2,
			third10c0[i][0], dev->sensor_flags, &third10c0[i][1]);		/* URB 97-106 */

	buf[0] = 0x06;
	usb_microdia_control_write(dev, 0x1180, buf, 1);

	for (i = 0; i < 3; i++)
		sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 2,
			fourth10c0a[i][0], dev->sensor_flags, &fourth10c0a[i][1]);

	for (i = 0; i < 2; i++)
		sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 4,
			fourth10c0b[i][0], dev->sensor_flags, &fourth10c0b[i][1]);

	sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 0,
		0x33, dev->sensor_flags, NULL);

	sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 2,
		first10c0[0][0], dev->sensor_flags, &first10c0[0][1]);

	buf[0] = 0x0a;
	usb_microdia_control_write(dev, 0x11ba, buf, 1);

	buf[0] = 0x10;
	usb_microdia_control_write(dev, 0x118b, buf, 1);

	buf[0] = 0x05;
	usb_microdia_control_write(dev, 0x10f7, buf, 1);

	buf[0] = 0x14;
	usb_microdia_control_write(dev, 0x10f8, buf, 1);

	buf[0] = 0xff;
	usb_microdia_control_write(dev, 0x10fa, buf, 1);

	buf[0] = 0x00;
	usb_microdia_control_write(dev, 0x10f9, buf, 1);

	buf[0] = 0x00;
	usb_microdia_control_write(dev, 0x10f9, buf, 1);

	buf[0] = 0x0a;
	usb_microdia_control_write(dev, 0x11ba, buf, 1);

	buf[0] = 0x00; buf[1] = 0x00; buf[2] = 0x00; buf[3] = 0x00;
	usb_microdia_control_write(dev, 0x11bc, buf, 4);

	/* Set whole array buf to 0x00 */
	memset(buf, 0x00, 48);
	usb_microdia_control_write(dev, 0x11c0, buf, 48);

	buf[0] = 0x2d; buf[1] = 0x2d; buf[2] = 0x3a;
	buf[3] = 0x05; buf[4] = 0x04; buf[5] = 0x3f;
	usb_microdia_control_write(dev, 0x11a5, buf, 6);

	buf[0] = 0x14; buf[1] = 0xec; buf[2] = 0x0a; buf[3] = 0xf6;
	usb_microdia_control_write(dev, 0x11af, buf, 4);

	buf[0] = 0x32; buf[1] = 0xdd; buf[2] = 0x2d; buf[3] = 0xdd;
	usb_microdia_control_write(dev, 0x11b3, buf, 4);

	buf[0] = 0x07;
	usb_microdia_control_write(dev, 0x10e0, buf, 1);

	buf[0] = 0x27;
	usb_microdia_control_write(dev, 0x10e0, buf, 1);

	usb_microdia_control_write(dev, 0x1100, qtable1, 64);

	usb_microdia_control_write(dev, 0x1140, qtable2, 64);				/* URB 150 */

	buf[0] = 0x07;
	usb_microdia_control_write(dev, 0x10e0, buf, 1);

	buf[0] = 0x0b;
    if(jpeg)
        buf[0] &= 0xfc;
	usb_microdia_control_write(dev, 0x10e0, buf, 1);

	buf[0] = 0x00;
	usb_microdia_control_write(dev, 0x11bc, buf, 1);

	buf[0] = 0x00;
	usb_microdia_control_write(dev, 0x11bd, buf, 1);

	buf[0] = 0x00;
	usb_microdia_control_write(dev, 0x11be, buf, 1);

	buf[0] = 0x00;
	usb_microdia_control_write(dev, 0x11bf, buf, 1);

	sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 4,
		second10c0a[0][0], dev->sensor_flags, &second10c0a[0][1]);

	sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 4,
		second10c0a[1][0], dev->sensor_flags, &second10c0a[1][1]);

	sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 0,
		0x09, dev->sensor_flags, NULL);

	sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 2,
		second10c0b[0], dev->sensor_flags, &second10c0b[1]);

	buf[0] = 0x06; buf[1] = 0x00; buf[2] = 0x02;
	buf[3] = 0x00; buf[4] = 0x28; buf[5] = 0x3c;
	usb_microdia_control_write(dev, 0x1180, buf, 6);

	buf[0] = 0x00; buf[1] = 0x00; buf[2] = 0x6a; buf[3] = 0x50;
	usb_microdia_control_write(dev, 0x11a1, buf, 4);				/* URB 170 */

	buf[0] = 0x00; buf[1] = 0x00; buf[2] = 0x50; buf[3] = 0x3c;
	usb_microdia_control_write(dev, 0x11ab, buf, 4);

	buf[0] = 0x0a;
	usb_microdia_control_write(dev, 0x11ba, buf, 1);

	buf[0] = 0x00;
	usb_microdia_control_write(dev, 0x11b9, buf, 1);

	buf[0] = 0x0b;
	usb_microdia_control_write(dev, 0x11ba, buf, 1);

	buf[0] = 0x78;
	usb_microdia_control_write(dev, 0x1000, buf, 1);

	buf[0] = 0x18;
	usb_microdia_control_write(dev, 0x1002, buf, 1);

	buf[0] = 0x18;
	usb_microdia_control_write(dev, 0x1002, buf, 1);

	buf[0] = 0x7b;
	usb_microdia_control_write(dev, 0x11b8, buf, 1);				/* URB 180 */

	buf[0] = 0x02;
	usb_microdia_control_write(dev, 0x118a, buf, 1);

	buf[0] = 0x02;
	usb_microdia_control_write(dev, 0x0395, buf, 1);

	buf[0] = 0x78;
	usb_microdia_control_write(dev, 0x11b8, buf, 1);
	usb_microdia_control_read(dev, 0x11b8, buf, 1);

	buf[0] = 0xf9;
	usb_microdia_control_write(dev, 0x11b8, buf, 1);
	usb_microdia_control_read(dev, 0x11b8, buf, 1);

	buf[0] = 0xfa;
	usb_microdia_control_write(dev, 0x11b8, buf, 1);
	usb_microdia_control_read(dev, 0x11b8, buf, 1);

	buf[0] = 0x7b;
	usb_microdia_control_write(dev, 0x11b8, buf, 1);
	usb_microdia_control_read(dev, 0x11b8, buf, 1);					/* URB 190 */

	buf[0] = 0x7c;
	usb_microdia_control_write(dev, 0x11b8, buf, 1);
	usb_microdia_control_read(dev, 0x11b8, buf, 1);

	buf[0] = 0x7b;
	usb_microdia_control_write(dev, 0x11b8, buf, 1);

	for (i = 0; i < 3; i++)
		sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 2,
			fifth10c0[i][0], dev->sensor_flags, &fifth10c0[i][1]);		/* URB 200-205 */

	sn9c20x_read_i2c_data(dev, dev->sensor_slave_address, 2,
		0x00, dev->sensor_flags, buf);
	/* __u8 result[2] = {buf[3], buf[4]}; */

	usb_microdia_control_read(dev, 0x1061, buf, 1);
	buf[0] |= 0x02;
	usb_microdia_control_write(dev, 0x1061, buf, 1);

	return 0;
}


/**
 * @brief From startstream.log
 *
 * @param dev
 *
 * @returns 0
 *
 * @author GWater
 *
 * For SN9C201 with SOI968.
 * This is function triggers a working stream of image data and a glowing LED.
 */
int microdia_624e_start_stream(struct usb_microdia *dev)
{
	int ret, i;
	__u8 unknown1[9] = { dev->sensor_flags, dev->sensor_slave_address,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03 };

	__u16 reg;
	__u8 value;

	__u16 regs[][2] = {
		{0x1066, 0x00},
		{0x1000, 0x7c},
		{0x1001, 0x44},
		{0x1002, 0x1c},
		{0x1006, 0x80},
		{0x1007, 0xa0},
		{0x1061, 0x03},
		{0x1020, 0x80},
		{0x1067, 0x40},
		{0x1068, 0x30},
		{0x1069, 0x20},
		{0x106a, 0x10},
		{0x106b, 0x08},
		{0x10e0, 0x0b},
		{0x10f6, 0x60},
		{0x10f7, 0x07},
		{0x10f8, 0x14},
		{0x10f9, 0x00},
		{0x10fa, 0xff},
		{0x1188, 0x8a},
		{0x118b, 0x0a},
		{0x118c, 0x20},
		{0x118d, 0x20},
		{0x118e, 0x20},
		{0x118f, 0x20},
		{0x1190, 0x00},
		{0x1191, 0x24},
		{0x1192, 0x3b},
		{0x1193, 0x4f},
		{0x1194, 0x61},
		{0x1195, 0x71},
		{0x1196, 0x80},
		{0x1197, 0x8f},
		{0x1198, 0x9d},
		{0x1199, 0xab},
		{0x119a, 0xb8},
		{0x119b, 0xc4},
		{0x119c, 0xd1},
		{0x119d, 0xdd},
		{0x119e, 0xe9},
		{0x119f, 0xf4},
		{0x11a0, 0xff},
		{0x11a1, 0x00},
		{0x11a2, 0x00},
		{0x11b7, 0x60},
		{0x11b8, 0x7a},
		{0x11b9, 0x00},
		{0x11ba, 0x0f},
		{0x11bb, 0x00},
		{0x11bc, 0x00},
		{0x11bd, 0x00},
		{0x11be, 0x00},
		{0x11bf, 0x00},
		{0x1180, 0x3c},
		{0x1181, 0x00},
		{0x1182, 0x0b},
		{0x1183, 0x00},
		{0x1184, 0x28},
		{0x1185, 0x3c},
		{0x11a1, 0x00},
		{0x11a2, 0x00},
		{0x11a3, 0x6a},
		{0x11a4, 0x50},
		{0x11ab, 0x00},
		{0x11ac, 0x00},
		{0x11ad, 0x50},
		{0x11ae, 0x3c},
		{0x118a, 0x05},
		{0x0395, 0x05},
		{0x11a5, 0x2d},
		{0x11a6, 0x2d},
		{0x11a7, 0x3a},
		{0x11a8, 0x05},
		{0x11a9, 0x04},
		{0x11aa, 0x3f},
		{0x11af, 0x28},
		{0x11b0, 0xd8},
		{0x11b1, 0x14},
		{0x11b2, 0xec},
		{0x11b3, 0x32},
		{0x11b4, 0xdd},
		{0x11b5, 0x2d},
		{0x11b6, 0xdd},
	};

	__u8 reg_11c0[48];

	__u8 qtable1[64] = {
		0x0d, 0x08, 0x08, 0x0d, 0x08, 0x08, 0x0d, 0x0d,
		0x0d, 0x0d, 0x11, 0x0d, 0x0d, 0x11, 0x15, 0x21,
		0x15, 0x15, 0x11, 0x11, 0x15, 0x2a, 0x1d, 0x1d,
		0x19, 0x21, 0x32, 0x2a, 0x32, 0x32, 0x2e, 0x2a,
		0x2e, 0x2e, 0x36, 0x3a, 0x4b, 0x43, 0x36, 0x3a,
		0x47, 0x3a, 0x2e, 0x2e, 0x43, 0x5c, 0x43, 0x47,
		0x4f, 0x53, 0x58, 0x58, 0x58, 0x32, 0x3f, 0x60,
		0x64, 0x5c, 0x53, 0x64, 0x4b, 0x53, 0x58, 0x53
	};

	__u8 qtable2[64] = {
		0x0d, 0x11, 0x11, 0x15, 0x11, 0x15, 0x26, 0x15,
		0x15, 0x26, 0x53, 0x36, 0x2e, 0x36, 0x53, 0x53,
		0x53, 0x53, 0x53, 0x53, 0x53, 0x53, 0x53, 0x53,
		0x53, 0x53, 0x53, 0x53, 0x53, 0x53, 0x53, 0x53,
		0x53, 0x53, 0x53, 0x53, 0x53, 0x53, 0x53, 0x53,
		0x53, 0x53, 0x53, 0x53, 0x53, 0x53, 0x53, 0x53,
		0x53, 0x53, 0x53, 0x53, 0x53, 0x53, 0x53, 0x53,
		0x53, 0x53, 0x53, 0x53, 0x53, 0x53, 0x53, 0x53
	};


	for (i = 0; i < ARRAY_SIZE(regs); i++) {
		reg = regs[i][0];
		value = regs[i][1];
		ret = usb_microdia_control_write(dev, reg, &value, 1);
		if (ret < 0) {
			UDIA_INFO("Bridge Init Error (%d). line %d\n", ret, i);
			goto err;
		}
	}

	memset(reg_11c0, 0x00, 48);
	ret = usb_microdia_control_write(dev, 0x11c0, reg_11c0, 48);
	if (ret < 0)
		goto err;

	ret = usb_microdia_control_write(dev, 0x1100, qtable1, 64);
	if (ret < 0)
		goto err;

	ret = usb_microdia_control_write(dev, 0x1140, qtable2, 64);
	if (ret < 0)
		goto err;

	/* this is the same register as the I2C write, not sure why
	 * we're writing 9 bytes -- the SN9C102 docs say all writes
	 * must be 8 bytes, but we don't have docs for SN9C20x */
	ret = usb_microdia_control_write(dev, 0x10c0, unknown1, 9);
	if (ret < 0)
		goto err;

	if (dev->sensor_init)
		dev->sensor_init(dev);

	return 0;

err:
	UDIA_ERROR("start stream: failed (%d)!\n", ret);
	return ret;
}

int microdia_624f_start_stream(struct usb_microdia *dev)
{
	int ret, i;
	__u8 unknown1[9] = { dev->sensor_flags, dev->sensor_slave_address,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03 };

	__u16 reg;
	__u8 value;

	__u16 regs[][2] = {
		{0x1007, 0x20},
		{0x1006, 0x00},
		{0x1000, 0x78},
		{0x1001, 0x40},
		{0x1002, 0x1c},
		{0x1061, 0x01},
		{0x1020, 0x80},
		{0x1067, 0x40},
		{0x1068, 0x30},
		{0x1069, 0x20},
		{0x106a, 0x10},
		{0x106b, 0x08},
		{0x1188, 0x87},
		{0x11a1, 0x00},
		{0x11a2, 0x00},
		{0x11a3, 0x6a},
		{0x11a4, 0x50},
		{0x11ab, 0x00},
		{0x11ac, 0x00},
		{0x11ad, 0x50},
		{0x11ae, 0x3c},
		{0x118a, 0x04},
		{0x0395, 0x04},
		{0x11b8, 0x3a},
		{0x118b, 0x0e},
		{0x10f7, 0x05},
		{0x10f8, 0x14},
		{0x10fa, 0xff},
		{0x10f9, 0x00},
		{0x11ba, 0x0e},
		{0x11a5, 0x2d},
		{0x11a6, 0x2d},
		{0x11a7, 0x3a},
		{0x11a8, 0x05},
		{0x11a9, 0x04},
		{0x11aa, 0x3f},
		{0x11af, 0x28},
		{0x11b0, 0xd8},
		{0x11b1, 0x14},
		{0x11b2, 0xec},
		{0x11b3, 0x32},
		{0x11b4, 0xdd},
		{0x11b5, 0x32},
		{0x11b6, 0xdd},
		{0x10e0, 0x2c},
		{0x11bc, 0x40},
		{0x11bd, 0x01},
		{0x11be, 0xf0},
		{0x11bf, 0x00},
		{0x1180, 0x02},
		{0x1181, 0x00},
		{0x1182, 0x07},
		{0x1183, 0x00},
		{0x1184, 0x28},
		{0x1185, 0x3c},
		{0x1061, 0x03},
	};

	__u8 reg_11c0[48] = {
		0x10, 0x21, 0x34, 0x40, 0x47, 0x4f, 0x57, 0x5f,
		0x64, 0x68, 0x6d, 0x73, 0x79, 0x80, 0x89, 0x97,
		0x0d, 0x1c, 0x2a, 0x33, 0x38, 0x3d, 0x44, 0x4a,
		0x4e, 0x52, 0x56, 0x5b, 0x61, 0x68, 0x6f, 0x7a,
		0x0d, 0x1a, 0x2a, 0x31, 0x36, 0x3b, 0x41, 0x47,
		0x4a, 0x4e, 0x53, 0x58, 0x5d, 0x64, 0x6b, 0x76
	};

	__u8 qtable1[64] = {
		0x0d, 0x08, 0x08, 0x0d, 0x08, 0x08, 0x0d, 0x0d,
		0x0d, 0x0d, 0x11, 0x0d, 0x0d, 0x11, 0x15, 0x21,
		0x15, 0x15, 0x11, 0x11, 0x15, 0x2a, 0x1d, 0x1d,
		0x19, 0x21, 0x32, 0x2a, 0x32, 0x32, 0x2e, 0x2a,
		0x2e, 0x2e, 0x36, 0x3a, 0x4b, 0x43, 0x36, 0x3a,
		0x47, 0x3a, 0x2e, 0x2e, 0x43, 0x5c, 0x43, 0x47,
		0x4f, 0x54, 0x58, 0x58, 0x58, 0x32, 0x3f, 0x60,
		0x64, 0x5c, 0x54, 0x64, 0x4b, 0x54, 0x58, 0x54
	};

	__u8 qtable2[64] = {
		0x0d, 0x11, 0x11, 0x15, 0x11, 0x15, 0x26, 0x15,
		0x15, 0x26, 0x54, 0x36, 0x2e, 0x36, 0x54, 0x54,
		0x54, 0x54, 0x54, 0x54, 0x54, 0x54, 0x54, 0x54,
		0x54, 0x54, 0x54, 0x54, 0x54, 0x54, 0x54, 0x54,
		0x54, 0x54, 0x54, 0x54, 0x54, 0x54, 0x54, 0x54,
		0x54, 0x54, 0x54, 0x54, 0x54, 0x54, 0x54, 0x54,
		0x54, 0x54, 0x54, 0x54, 0x54, 0x54, 0x54, 0x54,
		0x54, 0x54, 0x54, 0x54, 0x54, 0x54, 0x54, 0x54
	};

	for (i = 0; i < ARRAY_SIZE(regs); i++) {
		reg = regs[i][0];
		value = regs[i][1];
		ret = usb_microdia_control_write(dev, reg, &value, 1);
		if (ret < 0) {
			UDIA_INFO("Bridge Init Error (%d). line %d\n", ret, i);
			goto err;
		}
	}

	ret = usb_microdia_control_write(dev, 0x11c0, reg_11c0, 48);
	if (ret < 0)
		goto err;

	ret = usb_microdia_control_write(dev, 0x1100, qtable1, 64);
	if (ret < 0)
		goto err;

	ret = usb_microdia_control_write(dev, 0x1140, qtable2, 64);
	if (ret < 0)
		goto err;

	/* this is the same register as the I2C write, not sure why
	 * we're writing 9 bytes -- the SN9C102 docs say all writes
	 * must be 8 bytes, but we don't have docs for SN9C20x */
	ret = usb_microdia_control_write(dev, 0x10c0, unknown1, 9);
	if (ret < 0)
		goto err;

	if (dev->sensor_init)
		dev->sensor_init(dev);

	if (jpeg) {
		value = 0x40;
		ret = usb_microdia_control_write(dev, 0x1001, &value, 1);
		value = 0x2c;
		ret = usb_microdia_control_write(dev, 0x10e0, &value, 1);
	}

	return 0;

err:
	UDIA_ERROR("start stream: failed (%d)!\n", ret);
	return ret;
}

/**
 * @brief Start stream from 6260 devices
 *
 * @param dev
 *
 * @returns 0 or negative error code
 *
 */
int microdia_6260_start_stream(struct usb_microdia *dev)
{
	int ret = 0;
	int i;
	__u16 reg;
	__u8  value[1];
	__u8 buf[32];

	static __u16 bridge6260_start[][2] = {
	{0x1045, 0x1f},
	{0x1066, 0x00},
	{0x1007, 0x20},
	{0x1006, 0x00},
	{0x1000, 0x78},
	{0x1001, 0xc7},
	{0x1002, 0x18},
	{0x1061, 0x01},
	{0x1020, 0x80},
	{0x1067, 0x40},
	{0x1068, 0x30},
	{0x1069, 0x20},
	{0x106a, 0x10},
	{0x106b, 0x08},
	{0x10e0, 0x45},
	{0x10e1, 0x18},
	{0x10e2, 0x00},
	{0x10e3, 0x30},
	{0x10e4, 0x00},
	{0x10e5, 0x09},
	{0x10e6, 0x00},
	{0x10e7, 0xed},
	{0x10e8, 0x0f},
	{0x10e9, 0xda},
	{0x10ea, 0x0f},
	{0x10eb, 0x3a},
	{0x10ec, 0x00},
	{0x10ed, 0x3a},
	{0x10ee, 0x00},
	{0x10ef, 0xd0},
	{0x10d0, 0x0f},
	{0x10d1, 0xf7},
	{0x10d2, 0x0f},
	{0x10d3, 0x00},
	{0x10d4, 0x00},
	{0x10d5, 0x00},
	{0x10d6, 0x60},
	{0x10d7, 0x0a},
	{0x10f8, 0x00},
	{0x10f9, 0x00},
	{0x10fa, 0x00},
	{0x1188, 0x8a},
	{0x118a, 0x08},
	{0x118b, 0x00},
	{0x118c, 0x20},
	{0x118d, 0x20},
	{0x118e, 0x20},
	{0x118f, 0x20},
	{0x1190, 0x00},
	{0x1191, 0x24},
	{0x1192, 0x3b},
	{0x1193, 0x4f},
	{0x1194, 0x61},
	{0x1195, 0x71},
	{0x1196, 0x80},
	{0x1197, 0x8f},
	{0x1198, 0x9d},
	{0x1199, 0xab},
	{0x119a, 0xb8},
	{0x119b, 0xc4},
	{0x119c, 0xd1},
	{0x119d, 0xdd},
	{0x119e, 0xe9},
	{0x119f, 0xf4},
	{0x1200, 0xff},
	{0x11a1, 0x00},
	{0x11a2, 0x00},
	{0x11b7, 0x60},
	{0x11b8, 0x28},
	{0x11b9, 0x00},
	{0x11ba, 0x00},
	{0x11bb, 0x00},
	{0x11bc, 0x40},
	{0x11bd, 0x01},
	{0x11be, 0xf0},
	{0x11bf, 0x00},
	{0x1000, 0x78},
	{0x11b8, 0x38},
	{0x10e0, 0x45},
	{0x1001, 0xc6},
	{0x1001, 0xc4},
	{0x1180, 0x00},
	{0x1181, 0x00},
	{0x1182, 0x01},
	{0x1183, 0x00},
	{0x1184, 0x50},
	{0x1185, 0x3c},
	{0x11a1, 0x00},
	{0x11a2, 0x00},
	{0x11a3, 0xd5},
	{0x11a4, 0x50},
	{0x11ab, 0x00},
	{0x11ac, 0x00},
	{0x11ad, 0xa0},
	{0x11ae, 0x3c},
	{0x1000, 0x78},
	{0x1002, 0x10},
	{0x1002, 0x00},
	{0x11b8, 0x38},
	{0x118a, 0x04},
	{0x0395, 0x04},
	{0x11b8, 0x78},
	{0x11b8, 0xf9},
	{0x11b8, 0xfa},
	{0x11b8, 0x7b},
	{0x11b8, 0x7c},
	{0x11b8, 0x7d},
	{0x11b8, 0x7b},
	{0x1180, 0x00},
	{0x1181, 0x00},
	{0x1182, 0x01},
	{0x1183, 0x00},
	{0x1184, 0x50},
	{0x1185, 0x3c},
	{0x11a1, 0x00},
	{0x11a2, 0x00},
	{0x11a3, 0xd5},
	{0x11a4, 0x50},
	{0x11ab, 0x00},
	{0x11ab, 0x00},
	{0x11ab, 0xa0},
	{0x11ab, 0x3c},
	{0x1061, 0x03},
	{0x1061, 0x01},
	{0x1000, 0x78},
	{0x1002, 0x00},
	{0x1002, 0x00},
	{0x11b8, 0x7b},
	{0x118a, 0x04},
	{0x0395, 0x04},
	{0x11b8, 0x78},
	{0x11b8, 0x79},
	{0x11b8, 0x7a},
	{0x11b8, 0x7b},
	{0x11b8, 0x7c},
	{0x11b8, 0x7d},
	{0x11b8, 0xfa},
	{0x1061, 0x03},
	};

	UDIA_INFO("Starting camera 0c45:6260...\n");

	for (i = 0; i < ARRAY_SIZE(bridge6260_start); i++) {
		reg = bridge6260_start[i][0];
		value[0] = bridge6260_start[i][1];
		ret = usb_microdia_control_write(dev, reg, value, 1);
		if (ret < 0) {
			UDIA_INFO("camera 0c45:6260 Bridge: Start Sequence Error (%d) on array line %d\n", ret, i);
			break;
		}
	}

	buf[0] = 0x80;	buf[1] = 0x21;	buf[2] = 0x00;	buf[3] = 0x00;
	buf[4] = 0x00;	buf[5] = 0x00;	buf[6] = 0x00;	buf[7] = 0x00;
	buf[8] = 0x03;
	ret = usb_microdia_control_write(dev, 0x10c0, buf, 9);

	ret = ov7670_initialise(dev);
	if (ret < 0)
		return ret;

	ret = sn9c20x_read_i2c_data(dev, dev->sensor_slave_address, 1, 0x3b, dev->sensor_flags, buf);
	ret = usb_microdia_control_read(dev, 0x10c1, buf, 1);
	buf[0] = 0x50;
	ret = usb_microdia_control_write(dev, 0x10c1, buf, 1);

	/* TRY TO READ FROM EEPROM: */
	ret = sn9c20x_read_i2c_data(dev, 0x50, 1, 0x00, dev->sensor_flags, buf);
	if (ret < 0)
		UDIA_INFO("No EEPROM found\n");
	else
		UDIA_INFO("Read from EEPROM successful\n");

	buf[0] = 0x21;
	ret = usb_microdia_control_write(dev, 0x10c1, buf, 1);

	return ret;
}

/**
 * @brief From 7 start-stop-logs (made by Alex Carlos, Boris Borisov, bohzan and Comer352l)
 *
 * @param dev
 *
 * @returns 0 (ok) or -1 (error)
 *
 * @author Comer352l
 *
 * Windows driver versions: 5.7.23.000
 * Windows versions: 2000 and XP
 * USB-Logger: SniffUSB 1.8 and 2.0, USBTrace 2.0, SnoopyPro
 * All logs were made using AMCAP with 640x480, RGB24
 */
int microdia_6270_start_stream(struct usb_microdia *dev)
{
	__u8 buf[64];
	__u8 last_11b8[1];
	int k;
	int retI2C;

	/*  Check if sensor slave address is valid: */
	if ((dev->sensor_slave_address != MT9V111_I2C_SLAVE_ADDRESS) && (dev->sensor_slave_address != MT9V011_I2C_SLAVE_ADDRESS)) {
		UDIA_INFO("microdia_6270_start_stream(): Invalid sensor slave address !\n");
		return -1;
	}

	/*  <= INTERRUPT COMING BACK (URB 1953) */
	/*  SWITCH TO ALTERNATE SETTINGS 8 (URB 1954)	// NOT IN ALEX'S LOG */
	/*  => INTERRUPT GOING DOWN (URB 1955) */

	buf[0] = 0x00;
	usb_microdia_control_write(dev, 0x1066, buf, 1);		/*  URB 1956 */

	/*  <= INTERRUPT COMING BACK (URB 1955) */
	/*  => INTERRUPT GOING DOWN (URB 1957) */

	buf[0] = 0x78;
	usb_microdia_control_write(dev, 0x1000, buf, 1);		/*  URB 1958 */
	buf[0] = 0xc7;
	usb_microdia_control_write(dev, 0x1001, buf, 1);		/*  URB 1959 */
	buf[0] = 0x18;
	usb_microdia_control_write(dev, 0x1002, buf, 1);		/*  URB 1960 */
	buf[0] = 0x01;
	usb_microdia_control_write(dev, 0x1061, buf, 1);		/*  URB 1961 */
	buf[0] = 0x80;
	usb_microdia_control_write(dev, 0x1020, buf, 1);		/*  URB 1962 */
	buf[0] = 0x40; buf[1] = 0x30; buf[2] = 0x20; buf[3] = 0x10; buf[4] = 0x08;
	usb_microdia_control_write(dev, 0x1067, buf, 5);		/*  URB 1963 */

	/*  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
	/*  STRANGE I2C MESSAGE: */
	/*  - 9 bytes */
	/*  - byte 8 = 0x00 at first start + first cycle */
	/*  - procedure not complete: no wait/check for ack/error, no readout of 0x10c2 */
	/*  - dummy writes at first cycle and read at second cycle, but there are many I2C messages in the meantime, so this should not work */
	/*  */
	/*  AT FIRST START + FIRST CYCLE: */
	buf[0] = 0x80; buf[1] = dev->sensor_slave_address; buf[2] = 0x00; buf[3] = 0x00;
	buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x00; buf[8] = 0x03;
	/*  AT ALL FURTHER STARTS + FIRST CYCLE: */
	/* MT9V111: I2C MESSAGE => IFP Register 0x33: Auto exposure */
	/* buf[0] = 0x90; buf[1] = dev->sensor_slave_address; buf[2] = 0x33; buf[3] = 0x00; */
	/* buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10; buf[8] = 0x03; */
	/* MT9V011: ????????????????????????? */
	/* buf[0] = 0x90; buf[1] = dev->sensor_slave_address; buf[2] = 0x07; buf[3] = 0x00; */
	/* buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10; buf[8] = 0x03; */
	/*  ALWAYS AT 2nd AND FURTHER CYCLES: */
	/* MT9V111: */
	/* buf[0] = 0xa2; buf[1] = 0x5c; buf[2] = 0x00; buf[3] = 0x36; */
	/* buf[4] = 0x00; buf[5] = 0x82; buf[6] = 0x3a; buf[7] = 0x10; buf[8] = 0x03; */
	/*  => buf[3] to buf[6] simply contain bytes read with URB 2215 */
	/* MT9V011: */
	/* buf[0] = 0xb0; buf[1] = 0x5d; buf[2] = 0x07; buf[3] = 0x00; */
	/* buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10; buf[8] = 0x03; */
	usb_microdia_control_write(dev, 0x10c0, buf, 9);		/*  URB 1964 */
	/*  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

	buf[0] = 0x45; buf[1] = 0x18; buf[2] = 0x00; buf[3] = 0x30;
	buf[4] = 0x00; buf[5] = 0x09; buf[6] = 0x00; buf[7] = 0xed;
	buf[8] = 0x0f; buf[9] = 0xda; buf[10] = 0x0f; buf[11] = 0x3a;
	buf[12] = 0x00; buf[13] = 0x3a; buf[14] = 0x00; buf[15] = 0xd0;
	buf[16] = 0x0f; buf[17] = 0xf7; buf[18] = 0x0f; buf[19] = 0x00;
	buf[20] = 0x00; buf[21] = 0x00; buf[22] = 0x60; buf[23] = 0x0a;
	usb_microdia_control_write(dev, 0x10e0, buf, 24);		/*  URB 1965 */
	buf[0] = 0x00; buf[1] = 0x00; buf[2] = 0x00;
	usb_microdia_control_write(dev, 0x10f8, buf, 3);		/*  URB 1966 */
	buf[0] = 0x06;
	usb_microdia_control_write(dev, 0x1188, buf, 1);		/*  URB 1968 */
	buf[0] = 0x08;
	usb_microdia_control_write(dev, 0x118a, buf, 1);
	buf[0] = 0x00; buf[1] = 0x20; buf[2] = 0x20; buf[3] = 0x20;
	buf[4] = 0x20; buf[5] = 0x00; buf[6] = 0x24; buf[7] = 0x3b;
	buf[8] = 0x4f; buf[9] = 0x61; buf[10] = 0x71; buf[11] = 0x80;
	buf[12] = 0x8f; buf[13] = 0x9d; buf[14] = 0xab; buf[15] = 0xb8;
	buf[16] = 0xc4; buf[17] = 0xd1; buf[18] = 0xdd; buf[19] = 0xe9;
	buf[20] = 0xf4; buf[21] = 0xff;
	usb_microdia_control_write(dev, 0x118b, buf, 22);		/*  URB 1969 */
	buf[0] = 0x00; buf[1] = 0x00;
	usb_microdia_control_write(dev, 0x11a1, buf, 2);		/*  URB 1970 */
	buf[0] = 0x60; buf[1] = 0x28; buf[2] = 0x00; buf[3] = 0x08;
	buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x00;
	buf[8] = 0x00;
	usb_microdia_control_write(dev, 0x11b7, buf, 9);		/*  URB 1971 */
	buf[0] = 0x38;
	usb_microdia_control_write(dev, 0x11b8, buf, 1);		/*  URB 1972 */
	buf[0] = 0x78;
	usb_microdia_control_write(dev, 0x1000, buf, 1);		/*  URB 1973 */

	/*  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
	/*  I2C MESSAGES */
	retI2C = 0;
	if (dev->sensor_slave_address == MT9V111_I2C_SLAVE_ADDRESS) {
		/*  Write to IFP register	0x0d: Color Correction Register 8 */
		buf[0] = 0x00; buf[1] = 0x01;
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 2, 0x0d, dev->sensor_flags, buf);	/*  URBs 1974-1976 */
		/*  Write to IFP register	0x0d: Color Correction Register 8 */
		buf[0] = 0x00; buf[1] = 0x00;	/*  ??? */
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 2, 0x0d, dev->sensor_flags, buf);	/*  URBs 1977-1979 */
		/*  Writes to IFP registers	0x01: Register Address Space Selection */
		/* 				0x02: Color Correction Register 1 */
		buf[0] = 0x00; buf[1] = 0x01;	/*  select IFP address space */
		buf[2] = 0x00; buf[3] = 0x16;	/*  ???	// AT FIRST START + FIRST CYCLE: 0x00 0x00 */
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 4, 0x01, dev->sensor_flags, buf);	/*  URBs 1980-1982 */
		/*  Writes to IFP registers	0x03: Color Correction Register 3 */
		/* 				0x04: Color Correction Register 4 */
		buf[0] = 0x01; buf[1] = 0xe1;	/*  ???	// AT FIRST START + FIRST CYCLE:  0x00 0x00 */
		buf[2] = 0x02; buf[3] = 0x81;	/*  ???	// AT FIRST START + FIRST CYCLE:  0x00 0x00 */
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 4, 0x03, dev->sensor_flags, buf);	/*  URBs 1983-1985 */
		/*  Writes to IFP registers	0x05: Aperture Correction (Sharpening) */
		/* 				0x06: Operating Mode Control */
		buf[0] = 0x00; buf[1] = 0x04;	/*  100% sharpening, no automatic sharpness reduction at low light	// AT FIRST START + FIRST CYCLE: 0x00 0x00: no sharpening */
		buf[2] = 0x00; buf[3] = 0x00;	/*  stop AWB at the current values, no on-the-fly defect correction, no auto exposure, + RESREVED option */
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 4, 0x05, dev->sensor_flags, buf);	/*  URBs 1986-1988 */
		/*  Writes to IFP registers	0x07: Image Flow Processor Soft Reset */
		/* 				0x08: Output Format Control */
		buf[0] = 0x30; buf[1] = 0x02;	/*  reset	// AT FIRST START + FIRST CYCLE: 0x00 0x00 */
		buf[2] = 0x04; buf[3] = 0x80;	/*  bypass entire image processing, raw 8+2 Bayer data output directly */
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 4, 0x07, dev->sensor_flags, buf);	/*  URBs 1989-1991 */
		/*  "Dummy" write to IFP Register 0x11: Color Correction Register 12 */
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 0, 0x11, dev->sensor_flags, NULL);	/*  URBs 1992-1993 */
		/*  Write to register		0x01: Register address space selection */
		/*  Write to sensor register	0x02: Column Start */
		buf[0] = 0x00; buf[1] = 0x04;	/*  select sensor address space */
		buf[2] = 0x00; buf[3] = 0x16;	/*  start with column 22 */
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 4, 0x01, dev->sensor_flags, buf);	/*  URBs 1994-1996 */
		/*  Writes to sensor registers	0x03: Window Height */
		/* 				0x04: Window Width */
		buf[0] = 0x01; buf[1] = 0xe6;	/*  486 */
		buf[2] = 0x02; buf[3] = 0x86;	/*  646 */
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 4, 0x03, dev->sensor_flags, buf);	/*  URBs 1997-1999 */
		/*  Writes to sensor registers	0x05: Horizontal Blanking */
		/* 				0x06: Vertical Blanking */
		buf[0] = 0x00; buf[1] = 0x04;	/*  4 columns (pixel clocks)	// AT FIRST START + FIRST CYCLE: 0x00 0x00 */
		buf[2] = 0x00; buf[3] = 0x00;	/*  0 rows */
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 4, 0x05, dev->sensor_flags, buf);	/*  URBs 2000-2002 */
		/*  Writes to sensor registers	0x07: Output Control */
		/* 				0x08: Row Start */
		buf[0] = 0x30; buf[1] = 0x02;	/*  normal operation + chip enable + RESERVED options */
		buf[2] = 0x00; buf[3] = 0x08;	/*  row 8 */
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 4, 0x07, dev->sensor_flags, buf);	/*  URBs 2003-2005 */
		/*  "Dummy" write to sensor Register 0x11: UNDOCUMENTED */
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 0, 0x11, dev->sensor_flags, NULL);	/*  URBs 2006-2007 */
		/*  Writes to sensor registers	0x0c: Shutter Delay */
		/* 				0x0d: Reset (Soft) (from MT9V011 datasheet) */
		buf[0] = 0x00; buf[1] = 0x00;	/*  0 master clocks */
		buf[2] = 0x00; buf[3] = 0x00;	/*  return to normal operation */
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 4, 0x0c, dev->sensor_flags, buf);	/*  URBs 2008-2010 */
		/*  Writes to sensor registers	0x0e: UNDOCUMENTED */
		/* 				0x0f: UNDOCUMENTED */
		buf[0] = 0x00; buf[1] = 0x00;
		buf[2] = 0x00; buf[3] = 0x00;
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 4, 0x0e, dev->sensor_flags, buf);	/*  URBs 2011-2013 */
		/*  Writes to sensor registers	0x10: UNDOCUMENTED */
		/* 				0x11: UNDOCUMENTED */
		buf[0] = 0x00; buf[1] = 0x00;
		buf[2] = 0x00; buf[3] = 0x00;
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 4, 0x10, dev->sensor_flags, buf);	/*  URBs 2014-2016 */
		/*  Writes to sensor registers	0x12: 2X Zoom Col Start		=> 0x1e bit 0 must be set to activate zoom */
		/* 				0x13: 2X Zoom Row Start		=> 0x1e bit 0 must be set to activate zoom */
		buf[0] = 0x00; buf[1] = 0xb0;	/*  column 176 */
		buf[2] = 0x00; buf[3] = 0x7c;	/*  row 124 */
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 4, 0x12, dev->sensor_flags, buf);	/*  URBs 2017-2018 */
		/*  Writes to sensor registers	0x14: UNDOCUMENTED */
		/* 				0x15: UNDOCUMENTED */
		buf[0] = 0x00; buf[1] = 0x00;
		buf[2] = 0x00; buf[3] = 0x00;
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 4, 0x14, dev->sensor_flags, buf);	/*  URBs 2019-2021 */
		/*  Writes to sensor registers	0x16: UNDOCUMENTED */
		/* 				0x17: UNDOCUMENTED */
		buf[0] = 0x00; buf[1] = 0x00;
		buf[2] = 0x00; buf[3] = 0x00;
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 4, 0x16, dev->sensor_flags, buf);	/*  URBs 2022-2024 */
		/*  Writes to sensor registers	0x18: UNDOCUMENTED */
		/* 				0x19: UNDOCUMENTED */
		buf[0] = 0x00; buf[1] = 0x00;
		buf[2] = 0x00; buf[3] = 0x00;
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 4, 0x18, dev->sensor_flags, buf);	/*  URBs 2025-2027 */
		/*  Writes to sensor registers	0x1a: UNDOCUMENTED */
		/* 				0x1b: UNDOCUMENTED */
		buf[0] = 0x00; buf[1] = 0x00;
		buf[2] = 0x00; buf[3] = 0x00;
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 4, 0x1a, dev->sensor_flags, buf);	/*  URBs 2028-2030 */
		/*  Writes to sensor registers	0x1c: UNDOCUMENTED */
		/* 				0x1d: UNDOCUMENTED */
		buf[0] = 0x00; buf[1] = 0x00;
		buf[2] = 0x00; buf[3] = 0x00;
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 4, 0x1c, dev->sensor_flags, buf);	/*  URBs 2031-2033 */
		/*  Write to sensor register	0x30: RESERVED */
		buf[0] = 0x00; buf[1] = 0x00;
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 2, 0x30, dev->sensor_flags, buf);	/*  URBs 2034-2036 */
		/*  Write to sensor register	0x20: Read Mode */
		buf[0] = 0x00; buf[1] = 0x00;	/*  normal readout */
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 2, 0x20, dev->sensor_flags, buf);	/*  URBs 2037-2039 */
		/*  Writes to sensor registers	0x30: RESERVED */
		/* 				0x31: RESERVED */
		buf[0] = 0x00; buf[1] = 0x05;
		buf[2] = 0x00; buf[3] = 0x00;
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 4, 0x30, dev->sensor_flags, buf);	/*  URBs 2040-2042 */
		/*  "Dummy" write to sensor Register 0x34: RESERVED */
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 0, 0x34, dev->sensor_flags, NULL);	/*  URBs 2043-2044 */
	} else if (dev->sensor_slave_address == MT9V011_I2C_SLAVE_ADDRESS) {
		/*  Write to sensor register	0x07: Output Control */
		buf[0] = 0x00; buf[1] = 0x02;	/*  chip enable, normal operation */
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 2, 0x07, dev->sensor_flags, buf);
		/*  Write to sensor register	0x0d: Soft Reset */
		buf[0] = 0x00; buf[1] = 0x01;	/*  reset */
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 2, 0x0d, dev->sensor_flags, buf);
		/*  Write to sensor register	0x0d: Soft Reset */
		buf[0] = 0x00; buf[1] = 0x00;	/*  resume operation */
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 2, 0x0d, dev->sensor_flags, buf);
		/*  Writes to sensor registers	0x01: Row start */
		/* 				0x02: Column Start */
		buf[0] = 0x00; buf[1] = 0x08;	/*  start with row 8 */
		buf[2] = 0x00; buf[3] = 0x16;	/*  start with column 22 */
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 4, 0x01, dev->sensor_flags, buf);
		/*  Writes to sensor registers	0x03: Window Height */
		/* 				0x04: Window Width */
		buf[0] = 0x01; buf[1] = 0xe1;	/*  481 */
		buf[2] = 0x02; buf[3] = 0x81;	/*  641 */
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 4, 0x03, dev->sensor_flags, buf);
		/*  Writes to sensor registers	0x05: Horizontal Blanking */
		/* 				0x06: Vertical Blanking */
		buf[0] = 0x00; buf[1] = 0x83;	/*  131 columns (pixel clocks) */
		buf[2] = 0x00; buf[3] = 0x06;	/*  6 rows */
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 4, 0x05, dev->sensor_flags, buf);
		/*  Write to sensor register	0x0d: Soft Reset */
		buf[0] = 0x00; buf[1] = 0x02;	/*  UNKNOWN */
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 2, 0x0d, dev->sensor_flags, buf);
		/*  Writes to sensor registers	0x0a: Pixel Clock Speed */
		/* 				0x0b: Frame Restart */
		buf[0] = 0x00; buf[1] = 0x00;	/*  default */
		buf[2] = 0x00; buf[3] = 0x00;	/*  (has no effect/no restart) */
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 4, 0x0a, dev->sensor_flags, buf);
		/*  Writes to sensor registers	0x0c: Shutter Delay */
		/* 				0x0d: Soft Reset */
		buf[0] = 0x00; buf[1] = 0x00;	/*  0 master clocks */
		buf[2] = 0x00; buf[3] = 0x00;	/*  resume operation */
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 4, 0x0c, dev->sensor_flags, buf);
		/*  Writes to sensor registers	0x0e: UNDOCUMENTED */
		/* 				0x0f: UNDOCUMENTED  */
		buf[0] = 0x00; buf[1] = 0x00;
		buf[2] = 0x00; buf[3] = 0x00;
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 4, 0x0e, dev->sensor_flags, buf);
		/*  Writes to sensor registers	0x10: UNDOCUMENTED */
		/* 				0x11: UNDOCUMENTED */
		buf[0] = 0x00; buf[1] = 0x00;
		buf[2] = 0x00; buf[3] = 0x00;
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 4, 0x10, dev->sensor_flags, buf);
		/*  Writes to sensor registers	0x12: 2X Zoom Column Start (from MT9V111 datasheet) */
		/* 				0x13: 2X Zoom Row Start (from MT9V111 datasheet) */
		buf[0] = 0x00; buf[1] = 0x00;	/*  column 0 => bit0 of reg 0x1e must be set to activate zoom */
		buf[2] = 0x00; buf[3] = 0x00;	/*  row 0    => bit0 of reg 0x1e must be set to activate zoom */
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 4, 0x12, dev->sensor_flags, buf);
		/*  Writes to sensor registers	0x14: UNDOCUMENTED */
		/* 				0x15: UNDOCUMENTED */
		buf[0] = 0x00; buf[1] = 0x00;
		buf[2] = 0x00; buf[3] = 0x00;
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 4, 0x14, dev->sensor_flags, buf);
		/*  Writes to sensor registers	0x16: UNDOCUMENTED */
		/* 				0x17: UNDOCUMENTED */
		buf[0] = 0x00; buf[1] = 0x00;
		buf[2] = 0x00; buf[3] = 0x00;
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 4, 0x16, dev->sensor_flags, buf);
		/*  Writes to sensor registers	0x18: UNDOCUMENTED */
		/* 				0x19: UNDOCUMENTED */
		buf[0] = 0x00; buf[1] = 0x00;
		buf[2] = 0x00; buf[3] = 0x00;
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 4, 0x18, dev->sensor_flags, buf);
		/*  Writes to sensor registers	0x1a: UNDOCUMENTED */
		/* 				0x1b: UNDOCUMENTED */
		buf[0] = 0x00; buf[1] = 0x00;
		buf[2] = 0x00; buf[3] = 0x00;
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 4, 0x1a, dev->sensor_flags, buf);
		/*  Writes to sensor registers	0x1c: UNDOCUMENTED */
		/* 				0x1d: UNDOCUMENTED */
		buf[0] = 0x00; buf[1] = 0x00;
		buf[2] = 0x00; buf[3] = 0x00;
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 4, 0x1c, dev->sensor_flags, buf);
		/*  Write to sensor register	0x32: RESERVED */
		buf[0] = 0x00; buf[1] = 0x00;
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 2, 0x32, dev->sensor_flags, buf);
		/*  Writes to sensor registers	0x20: Read Mode */
		/* 				0x21: RESERVED */
		buf[0] = 0x11; buf[1] = 0x01;	/*  output all frames (including bad frames) + UNDOCUMENTED */
		buf[2] = 0x00; buf[3] = 0x00;
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 4, 0x20, dev->sensor_flags, buf);
		/*  Writes to sensor registers	0x22: RESERVED */
		/* 				0x23: UNDOCUMENTED */
		buf[0] = 0x00; buf[1] = 0x00;
		buf[2] = 0x00; buf[3] = 0x00;
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 4, 0x22, dev->sensor_flags, buf);
		/*  Writes to sensor registers	0x24: UNDOCUMENTED */
		/* 				0x25: UNDOCUMENTED */
		buf[0] = 0x00; buf[1] = 0x00;
		buf[2] = 0x00; buf[3] = 0x00;
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 4, 0x24, dev->sensor_flags, buf);
		/*  Writes to sensor registers	0x26: UNDOCUMENTED */
		/* 				0x27: RESERVED */
		buf[0] = 0x00; buf[1] = 0x00;
		buf[2] = 0x00; buf[3] = 0x24;
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 4, 0x26, dev->sensor_flags, buf);
		/*  "Dummy" write to sensor Register 0x30: RESERVED */
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 0, 0x30, dev->sensor_flags, NULL);
		/*  Writes to sensor registers	0x2f: RESERVED */
		/* 				0x30: RESERVED */
		buf[0] = 0xf7; buf[1] = 0xb0;
		buf[2] = 0x00; buf[3] = 0x05;
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 4, 0x2f, dev->sensor_flags, buf);
		/*  Writes to sensor registers	0x31: RESERVED */
		/* 				0x32: RESERVED */
		buf[0] = 0x00; buf[1] = 0x00;
		buf[2] = 0x00; buf[3] = 0x00;
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 4, 0x31, dev->sensor_flags, buf);
		/*  Writes to sensor registers	0x33: RESERVED */
		/* 				0x34: RESERVED */
		buf[0] = 0x00; buf[1] = 0x00;
		buf[2] = 0x01; buf[3] = 0x00;
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 4, 0x33, dev->sensor_flags, buf);
		/*  "Dummy" write to sensor Register 0x3b: RESERVED */
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 0, 0x3b, dev->sensor_flags, NULL);
		/*  Write to sensor register	0x3d: RESERVED */
		buf[0] = 0x06; buf[1] = 0x8f;
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 2, 0x3d, dev->sensor_flags, buf);
		/*  Writes to sensor registers	0x40: RESERVED */
		/* 				0x41: RESERVED */
		buf[0] = 0x01; buf[1] = 0xe0;
		buf[2] = 0x00; buf[3] = 0xd1;
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 4, 0x40, dev->sensor_flags, buf);
		/*  Write to sensor register	0x44: UNDOCUMENTED */
		buf[0] = 0x00; buf[1] = 0x82;
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 2, 0x44, dev->sensor_flags, buf);
		/*  Writes to sensor registers	0x5a: RESERVED */
		/* 				0x5b: RESERVED */
		buf[0] = 0x00; buf[1] = 0x00;
		buf[2] = 0x00; buf[3] = 0x00;
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 4, 0x5a, dev->sensor_flags, buf);
		/*  Writes to sensor registers	0x5c: RESERVED */
		/* 				0x5d: RESERVED */
		buf[0] = 0x00; buf[1] = 0x00;
		buf[2] = 0x00; buf[3] = 0x00;
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 4, 0x5c, dev->sensor_flags, buf);
		/*  Writes to sensor registers	0x5e: RESERVED */
		/* 				0x5f: RESERVED */
		buf[0] = 0x00; buf[1] = 0x00;
		buf[2] = 0xa3; buf[3] = 0x1d;
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 4, 0x5e, dev->sensor_flags, buf);
		/*  "Dummy" write to sensor Register 0x68: UNDOCUMENTED */
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 0, 0x68, dev->sensor_flags, NULL);
		/*  Write to sensor register	0x62: RESERVED */
		buf[0] = 0x06; buf[1] = 0x11;
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 2, 0x62, dev->sensor_flags, buf);
	}
	if (retI2C < 0) {
		UDIA_INFO("1 or more errors occured in I2C-message-block 1 !\n");
		return -1;
	}
	/*  END OF I2C MESSAGES */
	/*  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

	if (dev->sensor_slave_address == MT9V011_I2C_SLAVE_ADDRESS) {
		buf[0] = 0x40;
		usb_microdia_control_write(dev, 0x1007, buf, 8);
		buf[0] = 0x40;
		usb_microdia_control_write(dev, 0x1006, buf, 8);
	}


	/*  *** TEMPORARY SWITCH TO SLAVE ADDRESS 0x50, I2C READ ATTEMPT: */
	/*  - WE CURRENTLY KNOW NO 6270 CAM THAT IS SUCCESSFULLY READ HERE */
	/*  - IF READ FAILS, THIS IS NEVER DONE AGAIN (IN NEXT CYCLE OR FURTHER STARTS) */
	usb_microdia_control_read(dev, 0x10c1, buf, 1);		/*  returns 0x5c */
	buf[0] = 0x50;
	usb_microdia_control_write(dev, 0x10c1, buf, 1);
	retI2C = sn9c20x_read_i2c_data(dev, 0x50, 1, 0x00, SN9C20X_I2C_2WIRE, buf);
	if (retI2C >= 0)
		UDIA_INFO("Read of address 0x00 from slave 0x50 successful:\n=> Please report this to microdia@googlegroups.com (http:/* groups.google.com/group/microdia) !\n");
	buf[0] = 0x5c;
	usb_microdia_control_write(dev, 0x10c1, buf, 1);


	buf[0] = 0x47;		/*  ALEX: 0x44 */
	usb_microdia_control_write(dev, 0x10e0, buf, 1);	/*  URB 2045 */
	buf[0] = 0x47;		/*  ALEX: 0x04 */
	usb_microdia_control_write(dev, 0x10e0, buf, 1);	/*  URB 2046 */
	buf[0] = 0xc6;
	usb_microdia_control_write(dev, 0x1001, buf, 1);	/*  URB 2047 */
	buf[0] = 0xc4;
	usb_microdia_control_write(dev, 0x1001, buf, 1);	/*  URB 2048 */
	buf[0] = 0x84;		/*  ALEX: 0xc0 */
	usb_microdia_control_write(dev, 0x1001, buf, 1);	/*  URB 2049 */
	/* ALEX:
	buf[0] = 0x40;
	usb_microdia_control_write(dev, 0x1001, buf, 1);
	*/

	if (dev->sensor_slave_address == MT9V111_I2C_SLAVE_ADDRESS) {
		/*  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
		/*  I2C MESSAGES */
		retI2C = 0;
		/*  Writes to sensor registers	0x02: Column Start */
		/* 				0x03: Window Height */
		buf[0] = 0x00; buf[1] = 0x16;	/*  coulmn 22 */
		buf[2] = 0x01; buf[3] = 0xe1;	/*  481 */
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 4, 0x02, dev->sensor_flags, buf);	/*  URBs 2050-2052 */
		/*  Writes to sensor registers	0x04: Window Width */
		/* 				0x05: Horizontal Blanking */
		buf[0] = 0x02; buf[1] = 0x81;	/*  641 */
		buf[2] = 0x00; buf[3] = 0x04;	/*  4 columns (pixel clocks)	// AT FIRST START + FIRST CYCLE: 0x00 0x00 */
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 4, 0x04, dev->sensor_flags, buf);	/*  URBs 2053-2055 */
		/*  Writes to sensor registers	0x06: Vertical Blanking */
		/* 				0x07: Output Control */
		buf[0] = 0x00; buf[1] = 0x00;	/*  0 rows */
		buf[2] = 0x30; buf[3] = 0x02;	/*  normal operation + chip enable + RESERVED options */
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 4, 0x06, dev->sensor_flags, buf);	/*  URBs 2056-2058 */
	/*  Write to sensor register	0x0e: UNDOCUMENTED */
		buf[0] = 0x00; buf[1] = 0x08;
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 2, 0x0e, dev->sensor_flags, buf);	/*  URBs 2059-2061 */
		if (retI2C < 0) {
			UDIA_INFO("1 or more errors occured in I2C-message-block 2 !\n");
			return -1;
		}
		/*  END OF I2C MESSAGES */
		/*  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
	}

	buf[0] = 0x02; buf[1] = 0x00; buf[2] = 0x02; buf[3] = 0x00;
	buf[4] = 0x28; buf[5] = 0x3c;
	usb_microdia_control_write(dev, 0x1180, buf, 6);	/*  URB 2062 */
	buf[0] = 0x00; buf[1] = 0x00; buf[2] = 0x6a; buf[3] = 0x50;
	usb_microdia_control_write(dev, 0x11a1, buf, 4);	/*  URB 2065 */
	buf[0] = 0x00; buf[1] = 0x00; buf[2] = 0x50; buf[3] = 0x3c;
	usb_microdia_control_write(dev, 0x11ab, buf, 4);	/*  URB 2066 */
	buf[0] = 0x78;		/*  ALEX: 0x7c */
	usb_microdia_control_write(dev, 0x1000, buf, 1);	/*  URB 2067 */
	buf[0] = 0x18;		/*  ALEX: 0x1c */
	usb_microdia_control_write(dev, 0x1002, buf, 1);	/*  URB 2068 */
	buf[0] = 0x18;		/*  ALEX: 0x1c */
	usb_microdia_control_write(dev, 0x1002, buf, 1);	/*  URB 2069 */
	buf[0] = 0x38;
	usb_microdia_control_write(dev, 0x11b8, buf, 1);	/*  URB 2070 */
	buf[0] = 0x04;
	usb_microdia_control_write(dev, 0x118a, buf, 1);	/*  URB 2071 */
	buf[0] = 0x04;
	usb_microdia_control_write(dev, 0x0395, buf, 1);	/*  URB 2072 */

	buf[0] = 0x78;
	usb_microdia_control_write(dev, 0x11b8, buf, 1);	/*  URB 2073 */
	usb_microdia_control_read(dev, 0x11b8, buf, 1);		/*  URB 2074	// 0x38 */
	if (buf[0] == 0x38)
		buf[0] = 0x79;
	else if (buf[0] == 0xb8)
		buf[0] = 0xf9;
	usb_microdia_control_write(dev, 0x11b8, buf, 1);	/*  URB 2075 */
	usb_microdia_control_read(dev, 0x11b8, buf, 1);		/*  URB 2076	// 0xb9 */
	if (buf[0] == 0x39)
		buf[0] = 0x7a;
	else if (buf[0] == 0xb9)
		buf[0] = 0xfa;
	usb_microdia_control_write(dev, 0x11b8, buf, 1);	/*  URB 2077 */
	usb_microdia_control_read(dev, 0x11b8, buf, 1);		/*  URB 2078 */
	if (buf[0] == 0x3a)
		buf[0] = 0x7b;
	else if (buf[0] == 0xba)
		buf[0] = 0xfb;
	usb_microdia_control_write(dev, 0x11b8, buf, 1);	/*  URB 2079 */
	usb_microdia_control_read(dev, 0x11b8, buf, 1);		/*  URB 2080	// 0x3b */
	if (buf[0] == 0x3b)
		buf[0] = 0x7c;
	else if (buf[0] == 0xbb)
		buf[0] = 0xfc;
	usb_microdia_control_write(dev, 0x11b8, buf, 1);	/*  URB 2081 */
	usb_microdia_control_read(dev, 0x11b8, buf, 1);		/*  URB 2082	// 0x3c */
	if (buf[0] == 0x3c)
		buf[0] = 0x7d;
	else if (buf[0] == 0xbc)
		buf[0] = 0xfd;
	usb_microdia_control_write(dev, 0x11b8, buf, 1);	/*  URB 2083 */
	usb_microdia_control_read(dev, 0x11b8, buf, 1);		/*  URB 2084	// 0x3d */
	last_11b8[0] = buf[0];
	if (dev->sensor_slave_address == MT9V111_I2C_SLAVE_ADDRESS) {
		if (buf[0] == 0x3d)
			buf[0] = 0x7e;
		else if (buf[0] == 0xbd)
			buf[0] = 0xfe;
		usb_microdia_control_write(dev, 0x11b8, buf, 1);	/*  URB 2085 */
		usb_microdia_control_read(dev, 0x11b8, buf, 1);		/*  URB 2086 */
		if (buf[0] == 0x3e)
			buf[0] = 0x7f;
		else if (buf[0] == 0xbe)
			buf[0] = 0xff;
		usb_microdia_control_write(dev, 0x11b8, buf, 1);	/*  URB 2087 */
		usb_microdia_control_read(dev, 0x11b8, last_11b8, 1);	/*  URB 2088 */
	}
	buf[0] = last_11b8[0] & 0xf0;	/*  0xf0 or 0x70 */
	buf[0] = buf[0] | 0x0d;	/*  0x0c => THIS VALUE SEEMS TO DEPEND ON WHAT WAS READ/WRITTEN IN THE LAST 8 READS/WRITES */
	usb_microdia_control_write(dev, 0x11b8, buf, 1);	/*  URB 2089 */


	/*  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
	/*  I2C MESSAGES */
	retI2C = 0;
	if (dev->sensor_slave_address == MT9V111_I2C_SLAVE_ADDRESS) {
		/*  Write to sensor register	0x06: Vertical Blanking */
		buf[0] = 0x00; buf[1] = 0x2d;	/*  45 rows */
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 2, 0x06, dev->sensor_flags, buf);	/*  URBs 2090-2092 */
		/*  Write to sensor register	0x05: Horizontal Blanking */
		buf[0] = 0x00; buf[1] = 0x04;	/*  4 columns (pixel clocks) */
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 2, 0x05, dev->sensor_flags, buf);	/*  URBs 2093-2095 */
		/*  Write to sensor register	0x20: Read Mode */
		buf[0] = 0x00; buf[1] = 0x00;	/*  normal readout */
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 2, 0x20, dev->sensor_flags, buf);	/*  URBs 2096-2098 */
	} else if (dev->sensor_slave_address == MT9V011_I2C_SLAVE_ADDRESS) {
		/*  Write to sensor register	0x0a: Pixel Clock Speed */
		buf[0] = 0x00; buf[1] = 0x00;	/*  default */
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 2, 0x0a, dev->sensor_flags, buf);
		/*  Write to sensor register	0x06: Vertical Blanking */
		buf[0] = 0x00; buf[1] = 0x29;	/*  41 rows */
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 2, 0x06, dev->sensor_flags, buf);
		/*  Write to sensor register	0x05: Horizontal Blanking */
		buf[0] = 0x00; buf[1] = 0x09;	/*  9 columns (pixel clocks) */
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 2, 0x05, dev->sensor_flags, buf);
		/*  Write to sensor register	0x20: Read Mode */
		buf[0] = 0x11; buf[1] = 0x01;	/*  output all frames (including bad ones) + UNKNOWN */
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 2, 0x20, dev->sensor_flags, buf);
	}
	if (retI2C < 0) {
		UDIA_INFO("1 or more errors occured in I2C-message-block 3 !\n");
		return -1;
	}
	/*  END OF I2C MESSAGES */
	/*  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */


	buf[0] = 0x02;
	usb_microdia_control_write(dev, 0x1180, buf, 1);	/*  URB 2099 */

	/*  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
	/*  I2C MESSAGE */
	retI2C = 0;
	if (dev->sensor_slave_address == MT9V111_I2C_SLAVE_ADDRESS) {
		/*  Write to sensor register	0x20: Read Mode */
		buf[0] = 0x00; buf[1] = 0x00;	/*  normal readout */
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 2, 0x20, dev->sensor_flags, buf);	/*  URBs 2100-2102 */
	} else if (dev->sensor_slave_address == MT9V011_I2C_SLAVE_ADDRESS) {
		/*  Write to sensor register	0x20: Read Mode */
		buf[0] = 0x11; buf[1] = 0x01;	/*  output all frames (including bad ones) + UNKNOWN */
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 2, 0x20, dev->sensor_flags, buf);
	}
	if (retI2C < 0) {
		UDIA_INFO("1 or more errors occured in I2C-message-block 4 !\n");
		return -1;
	}
	/*  END OF I2C MESSAGES */
	/*  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */


	buf[0] = 0x02;
	usb_microdia_control_write(dev, 0x1182, buf, 1);	/*  URB 2103 */


	/*  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
	/*  I2C MESSAGES */
	retI2C = 0;
	if (dev->sensor_slave_address == MT9V111_I2C_SLAVE_ADDRESS) {
		/*  Write to sensor register	0x09: Shutter Width */
		buf[0] = 0x00; buf[1] = 0x64;	/*  integration of 100 rows */
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 2, 0x09, dev->sensor_flags, buf);	/*  URBs 2104-2106 */
		/*  Write to sensor register	0x2b: Green 1 Gain */
		/* 				0x2c: Blue Gain */
		buf[0] = 0x00; buf[1] = 0xa0;	/*  32*0.03125*2 = 2 */
		buf[2] = 0x00; buf[3] = 0xa0;	/*  32*0.03125*2 = 2 */
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 4, 0x2b, dev->sensor_flags, buf);	/*  URBs 2107-2109 */
		/*  Write to sensor register	0x2d: Red Gain */
		/* 				0x2e: Green 2 Gain */
		buf[0] = 0x00; buf[1] = 0xa0;	/*  32*0.03125*2 = 2 */
		buf[2] = 0x00; buf[3] = 0xa0;	/*  32*0.03125*2 = 2 */
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 4, 0x2d, dev->sensor_flags, buf);	/*  URBs 2110-2112 */
		/*  "Dummy" write to sensor Register 0x33: RESERVED */
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 0, 0x33, dev->sensor_flags, NULL);	/*  URBs 2113-2114 */
	} else if (dev->sensor_slave_address == MT9V011_I2C_SLAVE_ADDRESS) {
		/*  Write to sensor register	0x09: Shutter Width */
		buf[0] = 0x00; buf[1] = 0x64;	/*  integration of 100 rows */
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 2, 0x09, dev->sensor_flags, buf);
		/*  Write to sensor register	0x07: Output Control */
		buf[0] = 0x00; buf[1] = 0x03;	/*  dont update changes until bit0=0, chip enable, normal operation */
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 2, 0x07, dev->sensor_flags, buf);
		/*  Write to sensor register	0x2b: Green 1 Gain */
		/* 				0x2c: Blue Gain */
		buf[0] = 0x00; buf[1] = 0x33;	/*  51*0.03125*1 = 1.59375 */
		buf[2] = 0x00; buf[3] = 0xa0;	/*  32*0.03125*2 = 2 */
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 4, 0x2b, dev->sensor_flags, buf);
		/*  Write to sensor register	0x2d: Red Gain */
		/* 				0x2e: Green 2 Gain */
		buf[0] = 0x00; buf[1] = 0xa0;	/*  32*0.03125*2 = 2 */
		buf[2] = 0x00; buf[3] = 0x33;	/*  51*0.03125*1 = 1.59375 */
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 4, 0x2d, dev->sensor_flags, buf);
		/*  "Dummy" write to sensor Register 0x33: RESERVED */
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 0, 0x33, dev->sensor_flags, NULL);
		/*  Write to sensor register	0x07: Output Control */
		buf[0] = 0x00; buf[1] = 0x02;	/*  chip enable, normal operation */
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 2, 0x07, dev->sensor_flags, buf);
	}
	if (retI2C < 0) {
		UDIA_INFO("1 or more errors occured in I2C-message-block 5 !\n");
		return -1;
	}
	/*  END OF I2C MESSAGES */
	/*  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

	buf[0] = 0x0a;
	usb_microdia_control_write(dev, 0x11ba, buf, 1);	/*  URB 2119 */

	if (dev->sensor_slave_address == MT9V111_I2C_SLAVE_ADDRESS)
		buf[0] = 0x14;
	else if (dev->sensor_slave_address == MT9V011_I2C_SLAVE_ADDRESS)
		buf[0] = 0x0a;
	usb_microdia_control_write(dev, 0x118b, buf, 1);	/*  URB 2124 */

	buf[0] = 0x07;
	usb_microdia_control_write(dev, 0x10f7, buf, 1);	/*  URB 2135 */

	if (dev->sensor_slave_address == MT9V111_I2C_SLAVE_ADDRESS)
		buf[0] = 0x01;
	else if (dev->sensor_slave_address == MT9V011_I2C_SLAVE_ADDRESS)
		buf[0] = 0x14;
	usb_microdia_control_write(dev, 0x10f8, buf, 1);	/*  URB 2138 */

	if (dev->sensor_slave_address == MT9V111_I2C_SLAVE_ADDRESS)
		buf[0] = 0x0a;
	else if (dev->sensor_slave_address == MT9V011_I2C_SLAVE_ADDRESS)
		buf[0] = 0xff;
	usb_microdia_control_write(dev, 0x10fa, buf, 1);	/*  URB 2139 */

	buf[0] = 0x00;
	usb_microdia_control_write(dev, 0x10f9, buf, 1);	/*  URB 2140 */
	buf[0] = 0x00;
	usb_microdia_control_write(dev, 0x10f9, buf, 1);	/*  URB 2141 */
	buf[0] = 0x0a;
	usb_microdia_control_write(dev, 0x11ba, buf, 1);	/*  URB 2142 */
	buf[0] = 0x00; buf[1] = 0x00; buf[2] = 0x00; buf[3] = 0x00;
	usb_microdia_control_write(dev, 0x11bc, buf, 4);	/*  URB 2143 */
	for (k = 0; k < 48; k++)
		buf[k] = 0x00;
	usb_microdia_control_write(dev, 0x11c0, buf, 48);	/*  URB 2144 */

	buf[0] = 0x2d; buf[1] = 0x2d; buf[2] = 0x3a; buf[3] = 0x05;
	buf[4] = 0x04; buf[5] = 0x3f;
	usb_microdia_control_write(dev, 0x11a5, buf, 6);	/*  URB 2149 */

	if (dev->sensor_slave_address == MT9V111_I2C_SLAVE_ADDRESS) {
		buf[0] = 0x14; buf[1] = 0xec; buf[2] = 0x0a; buf[3] = 0xf6;
	} else if (dev->sensor_slave_address == MT9V011_I2C_SLAVE_ADDRESS) {
		buf[0] = 0x28; buf[1] = 0xd8; buf[2] = 0x14; buf[3] = 0xec;
	}
	usb_microdia_control_write(dev, 0x11af, buf, 4);	/*  URB 2150 */

	buf[0] = 0x32; buf[1] = 0xdd; buf[2] = 0x2d; buf[3] = 0xdd;
	usb_microdia_control_write(dev, 0x11b3, buf, 4);	/*  URB 2151 */
	buf[0] = 0x47;	/*  ALEX: 0x04 */
	usb_microdia_control_write(dev, 0x10e0, buf, 1);	/*  URB 2152 */
	buf[0] = 0x01;
	usb_microdia_control_write(dev, 0x1061, buf, 1);	/*  URB 2153 */
	buf[0] = 0x67;	/*  ALEX: 0x24 */
	usb_microdia_control_write(dev, 0x10e0, buf, 1);	/*  URB 2154 */
	buf[0] = 0x0d; buf[1] = 0x08; buf[2] = 0x08; buf[3] = 0x0d;
	buf[4] = 0x08; buf[5] = 0x08; buf[6] = 0x0d; buf[7] = 0x0d;
	buf[8] = 0x0d; buf[9] = 0x0d; buf[10] = 0x11; buf[11] = 0x0d;
	buf[12] = 0x0d; buf[13] = 0x11; buf[14] = 0x15; buf[15] = 0x21;
	buf[16] = 0x15; buf[17] = 0x15; buf[18] = 0x11; buf[19] = 0x11;
	buf[20] = 0x15; buf[21] = 0x2a; buf[22] = 0x1d; buf[23] = 0x1d;
	buf[24] = 0x19; buf[25] = 0x21; buf[26] = 0x32; buf[27] = 0x2a;
	buf[28] = 0x32; buf[29] = 0x32; buf[30] = 0x2e; buf[31] = 0x2a;
	buf[32] = 0x2e; buf[33] = 0x2e; buf[34] = 0x36; buf[35] = 0x3a;
	buf[36] = 0x4b; buf[37] = 0x43; buf[38] = 0x36; buf[39] = 0x3a;
	buf[40] = 0x47; buf[41] = 0x3a; buf[42] = 0x2e; buf[43] = 0x2e;
	buf[44] = 0x43; buf[45] = 0x5c; buf[46] = 0x43; buf[47] = 0x47;
	buf[48] = 0x4f; buf[49] = 0x53; buf[50] = 0x58; buf[51] = 0x58;
	buf[52] = 0x58; buf[53] = 0x32; buf[54] = 0x3f; buf[55] = 0x60;
	buf[56] = 0x64; buf[57] = 0x5c; buf[58] = 0x53; buf[59] = 0x64;
	buf[60] = 0x4b; buf[61] = 0x53; buf[62] = 0x58; buf[63] = 0x53;
	usb_microdia_control_write(dev, 0x1100, buf, 64);	/*  URB 2155 */
	buf[0] = 0x0d; buf[1] = 0x11; buf[2] = 0x11; buf[3] = 0x15;
	buf[4] = 0x11; buf[5] = 0x15; buf[6] = 0x26; buf[7] = 0x15;
	buf[8] = 0x15; buf[9] = 0x26; buf[10] = 0x53; buf[11] = 0x36;
	buf[12] = 0x2e; buf[13] = 0x36; buf[14] = 0x53; buf[15] = 0x53;
	buf[16] = 0x53; buf[17] = 0x53; buf[18] = 0x53; buf[19] = 0x53;
	buf[20] = 0x53; buf[21] = 0x53; buf[22] = 0x53; buf[23] = 0x53;
	buf[24] = 0x53; buf[25] = 0x53; buf[26] = 0x53; buf[27] = 0x53;
	buf[28] = 0x53; buf[29] = 0x53; buf[30] = 0x53; buf[31] = 0x53;
	buf[32] = 0x53; buf[33] = 0x53; buf[34] = 0x53; buf[35] = 0x53;
	buf[36] = 0x53; buf[37] = 0x53; buf[38] = 0x53; buf[39] = 0x53;
	buf[40] = 0x53; buf[41] = 0x53; buf[42] = 0x53; buf[43] = 0x53;
	buf[44] = 0x53; buf[45] = 0x53; buf[46] = 0x53; buf[47] = 0x53;
	buf[48] = 0x53; buf[49] = 0x53; buf[50] = 0x53; buf[51] = 0x53;
	buf[52] = 0x53; buf[53] = 0x53; buf[54] = 0x53; buf[55] = 0x53;
	buf[56] = 0x53; buf[57] = 0x53; buf[58] = 0x53; buf[59] = 0x53;
	buf[60] = 0x53; buf[61] = 0x53; buf[62] = 0x53; buf[63] = 0x53;
	usb_microdia_control_write(dev, 0x1140, buf, 64);	/*  URB 2156 */
	buf[0] = 0x47;	/*  ALEX: 0x04 */
	usb_microdia_control_write(dev, 0x10e0, buf, 1);	/*  URB 2157 */
	buf[0] = 0x03;
	usb_microdia_control_write(dev, 0x1061, buf, 1);	/*  URB 2158 */
	buf[0] = 0x4b;	/*  ALEX: 0x08 */
	usb_microdia_control_write(dev, 0x10e0, buf, 1);	/*  URB 2159 */

	if (dev->sensor_slave_address == MT9V111_I2C_SLAVE_ADDRESS) {
		/*  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
		/*  I2C MESSAGES */
		retI2C = 0;
		/*  Writes to sensor registers	0x02: Column Start */
		/* 				0x03: Window Hight */
		buf[0] = 0x00; buf[1] = 0x16;	/*  coulmn 22 */
		buf[2] = 0x01; buf[3] = 0xe1;	/*  481 */
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 4, 0x02, dev->sensor_flags, buf);	/*  URBs 2160-2162 */
		/*  Writes to sensor registers	0x04: Window Width */
		/* 				0x05: Horizontal Blanking */
		buf[0] = 0x02; buf[1] = 0x81;	/*  641 */
		buf[2] = 0x00; buf[3] = 0x04;	/*  4 columns (pixel clocks) */
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 4, 0x04, dev->sensor_flags, buf);	/*  URBs 2163-2165 */
		/*  Writes to sensor registers	0x06: Vertical Blanking */
		/* 				0x07: Output Control */
		buf[0] = 0x00; buf[1] = 0x2d;	/*  45 rows */
		buf[2] = 0x30; buf[3] = 0x02;	/*  RESERVED options */
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 4, 0x06, dev->sensor_flags, buf);	/*  URBs 2166-2167 */
		/*  Writes to sensor register	0x0e: UNDOCUMENTED */
		buf[0] = 0x00; buf[1] = 0x08;
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 2, 0x0e, dev->sensor_flags, buf);	/*  URBs 2168-2170 */
		if (retI2C < 0) {
			UDIA_INFO("1 or more errors occured in I2C-message-block 6 !\n");
			return -1;
		}
		/*  END OF I2C MESSAGES */
		/*  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
	} else if (dev->sensor_slave_address == MT9V011_I2C_SLAVE_ADDRESS) {
		buf[0] = 0x40;
		usb_microdia_control_write(dev, 0x1007, buf, 1);
		buf[0] = 0x40;
		usb_microdia_control_write(dev, 0x1006, buf, 1);
	}


	buf[0] = 0x02; buf[1] = 0x00; buf[2] = 0x02; buf[3] = 0x00;
	buf[4] = 0x28; buf[5] = 0x3c;
	usb_microdia_control_write(dev, 0x1180, buf, 6);	/*  URB 2171 */
	buf[0] = 0x00; buf[1] = 0x00; buf[2] = 0x6a; buf[3] = 0x50;
	usb_microdia_control_write(dev, 0x11a1, buf, 4);	/*  URB 2174 */
	buf[0] = 0x00; buf[1] = 0x00; buf[2] = 0x50; buf[3] = 0x3c;
	usb_microdia_control_write(dev, 0x11ab, buf, 4);	/*  URB 2175 */
	buf[0] = 0x03;
	usb_microdia_control_write(dev, 0x1061, buf, 1);	/*  URB 2176 */
	buf[0] = 0x0a;
	usb_microdia_control_write(dev, 0x11ba, buf, 1);	/*  URB 2177 */
	buf[0] = 0x00;
	usb_microdia_control_write(dev, 0x11b9, buf, 1);	/*  URB 2178 */
	buf[0] = 0x0b;
	usb_microdia_control_write(dev, 0x11ba, buf, 1);	/*  URB 2179 */
	buf[0] = 0x01;
	usb_microdia_control_write(dev, 0x1061, buf, 1);	/*  URB 2180 */
	buf[0] = 0x78;	/*  ALEX: 0x7c */
	usb_microdia_control_write(dev, 0x1000, buf, 1);	/*  URB 2181 */
	buf[0] = 0x18;	/*  ALEX: 0x1c */
	usb_microdia_control_write(dev, 0x1002, buf, 1);	/*  URB 2182 */
	buf[0] = 0x18;	/*  ALEX: 0x1c */
	usb_microdia_control_write(dev, 0x1002, buf, 1);	/*  URB 2183 */
	buf[0] = 0x7d;	/*  ALEX: 0xfc */
	usb_microdia_control_write(dev, 0x11b8, buf, 1);	/*  URB 2184 */
	buf[0] = 0x04;
	usb_microdia_control_write(dev, 0x118a, buf, 1);	/*  URB 2185 */
	buf[0] = 0x04;
	usb_microdia_control_write(dev, 0x0395, buf, 1);	/*  URB 2186 */

	if ((last_11b8[0] == 0x3d) || (last_11b8[0] == 0x3f))
		buf[0] = 0x78;
	else if ((last_11b8[0] == 0xbd) || (last_11b8[0] == 0xbf))
		buf[0] = 0xf8;
	usb_microdia_control_write(dev, 0x11b8, buf, 1);	/*  URB 2187 */
	usb_microdia_control_read(dev, 0x11b8, buf, 1);		/*  URB 2188	// 0x38 */
	if (buf[0] == 0x38)
		buf[0] = 0x79;
	else if (buf[0] == 0xb8)
		buf[0] = 0xf9;
	usb_microdia_control_write(dev, 0x11b8, buf, 1);	/*  URB 2189 */
	usb_microdia_control_read(dev, 0x11b8, buf, 1);		/*  URB 2190	// 0xb9 */
	if (buf[0] == 0x39)
		buf[0] = 0x7a;
	else if (buf[0] == 0xb9)
		buf[0] = 0xfa;
	usb_microdia_control_write(dev, 0x11b8, buf, 1);	/*  URB 2191 */
	usb_microdia_control_read(dev, 0x11b8, buf, 1);		/*  URB 2192	// 0xba */
	if (buf[0] == 0x3a)
		buf[0] = 0x7b;
	else if (buf[0] == 0xba)
		buf[0] = 0xfb;
	usb_microdia_control_write(dev, 0x11b8, buf, 1);	/*  URB 2193 */
	usb_microdia_control_read(dev, 0x11b8, buf, 1);		/*  URB 2194	// 0x3b */
	if (buf[0] == 0x3b)
		buf[0] = 0x7c;
	else if (buf[0] == 0xbb)
		buf[0] = 0xfc;
	usb_microdia_control_write(dev, 0x11b8, buf, 1);	/*  URB 2195 */
	usb_microdia_control_read(dev, 0x11b8, buf, 1);		/*  URB 2196	// 0x3c */
	if (buf[0] == 0x3c)
		buf[0] = 0x7d;
	else if (buf[0] == 0xbc)
		buf[0] = 0xfd;
	usb_microdia_control_write(dev, 0x11b8, buf, 1);	/*  URB 2197 */
	usb_microdia_control_read(dev, 0x11b8, buf, 1);		/*  URB 2198	// 0x3d */
	if (dev->sensor_slave_address == MT9V111_I2C_SLAVE_ADDRESS) {
		if (buf[0] == 0x3d)
			buf[0] = 0x7e;
		else if (buf[0] == 0xbd)
			buf[0] = 0xfe;
		usb_microdia_control_write(dev, 0x11b8, buf, 1);	/*  URB 2199 */
		usb_microdia_control_read(dev, 0x11b8, buf, 1);		/*  URB 2200 */
		if (buf[0] == 0x3e)
			buf[0] = 0x7f;
		else if (buf[0] == 0xbe)
			buf[0] = 0xff;
		usb_microdia_control_write(dev, 0x11b8, buf, 1);	/*  URB 2201 */
		usb_microdia_control_read(dev, 0x11b8, buf, 1);	/*  URB 2202 */
	}
	buf[0] = buf[0] & 0xf0;	/*  0xf0 or 0x70 */
	buf[0] = buf[0] | 0x0d;	/*  0x0c => THIS VALUE SEEMS TO DEPEND ON WHAT WAS READ/WRITTEN IN THE LAST 8 READS/WRITES */
	usb_microdia_control_write(dev, 0x11b8, buf, 1);	/*  URB 2089 */

	/*  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
	/*  I2C MESSAGES */
	retI2C = 0;
	if (dev->sensor_slave_address == MT9V111_I2C_SLAVE_ADDRESS) {
		/*  Write to sensor register	0x06: Vertical Blanking */
		buf[0] = 0x00; buf[1] = 0x2d;	/*  45 rows */
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 2, 0x06, dev->sensor_flags, buf);	/*  URBs 2204-2206 */
		/*  Write to sensor register	0x05: Horizontal Blanking */
		buf[0] = 0x00; buf[1] = 0x04;	/*  4 columns (pixel clocks) */
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 2, 0x05, dev->sensor_flags, buf);	/*  URBs 2207-2209 */
		/*  Read of sensor register	0x36: Chip Version (mirror of reg0xff) */
		retI2C += sn9c20x_read_i2c_data(dev, dev->sensor_slave_address, 2, 0x36, dev->sensor_flags, buf);	/*  URBs 2211-2215   // 0x82 0x3a */
	} else if (dev->sensor_slave_address == MT9V011_I2C_SLAVE_ADDRESS) {
		/*  Write to sensor register	0x0a: Pixel Clock Speed */
		buf[0] = 0x00; buf[1] = 0x00;	/*  default */
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 2, 0x06, dev->sensor_flags, buf);
		/*  Write to sensor register	0x06: Vertical Blanking */
		buf[0] = 0x00; buf[1] = 0x29;	/*  41 rows */
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 2, 0x06, dev->sensor_flags, buf);
		/*  Write to sensor register	0x05: Horizontal Blanking */
		buf[0] = 0x00; buf[1] = 0x09;	/*  9 columns (pixel clocks) */
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 2, 0x05, dev->sensor_flags, buf);
		/*  Read of sensor register	0x00: Chip Version (=reg0xff) */
		retI2C += sn9c20x_read_i2c_data(dev, dev->sensor_slave_address, 2, 0x00, dev->sensor_flags, buf);	/*  0x82 0x43 */
	}
	if (retI2C < 0) {
		UDIA_INFO("1 or more errors occured in I2C-message-block 7 !\n");
		return -1;
	}
	/*  END OF I2C MESSAGES */
	/*  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */


	buf[0] = 0x03;
	usb_microdia_control_write(dev, 0x1061, buf, 1);	/*  URB 2216 */


	/*** NOW DRIVER DOES STOP-SEQUENCE
		- WITHOUT WRITES TO 0x1007 AND 0x1006 (URBs 3544-3550)   => MT9V111
		- WRITING 0x40 INSTEAD OF 0x60 TO 0x1007 AND 0x1006	 => MT9V011
	 ***/
	/*** 2nd, 3rd, ... CYCLE: ALL MESSAGES (URBs 1953 - URBs 2216) ARE REPEATED ***/
	/*** ----- WE DON'T DO THIS, IT SEEMS TO BE UNNECESSARY ----- ***/

	/*  Setup IFP registers for AE and AWB (new, not in the logs): */
	if (dev->sensor_slave_address == MT9V111_I2C_SLAVE_ADDRESS) {
		mt9v111_setup_autoexposure(dev);
		mt9v111_setup_autowhitebalance(dev);
		mt9v111_set_autocorrections(dev, 1);
	}

	if (dev->sensor_slave_address == MT9V111_I2C_SLAVE_ADDRESS)
		buf[0] = 0x20;
	else if (dev->sensor_slave_address == MT9V011_I2C_SLAVE_ADDRESS)
		buf[0] = 0x60;
	usb_microdia_control_write(dev, 0x1007, buf, 1);	/*  URB 2489 */
	if (dev->sensor_slave_address == MT9V111_I2C_SLAVE_ADDRESS)
		buf[0] = 0x00;
	else if (dev->sensor_slave_address == MT9V011_I2C_SLAVE_ADDRESS)
		buf[0] = 0x40;
	usb_microdia_control_write(dev, 0x1006, buf, 1);	/*  URB 2490 */
	/*  => These two writes seem to cause the cam to start sending isochronus USB messages */

	return 0;
}

/**
 * @brief Start stream from 627b devices
 *
 * @param dev
 *
 * @returns 0 or negative error code
 *
 */
int microdia_627b_start_stream(struct usb_microdia *dev)
{
	int ret;
	/* Initialize the Bridge Controller */
	ret = sn9c20x_initialize(dev);
	/* Initialize the I2C interface in the Bridge */
	ret = sn9c20x_i2c_initialize(dev);
	/* Finally, Initialize the Sensor array */
	if (dev->sensor_init)
		dev->sensor_init(dev);
	return ret;
}

/**
 * @brief From UsbSnoop-plugin-parsed.log
 *
 * @param dev
 *
 * @returns 0 (OK) or <0 (Error)
 *
 * @author Vincent, Kuzja
 */
int microdia_6288_start_stream(struct usb_microdia *dev)
{
	int ret;
	__u16 reg;
	__u8 buf[64];



	reg = 0x1066 ;
	buf[0] = 0x00;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x1000 ;
	buf[0] = 0x78;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x1001 ;
	buf[0] = 0xc7;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x1002 ;
	buf[0] = 0x1c;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x1061 ;
	buf[0] = 0x01;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x1020 ;
	buf[0] = 0x80;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x1067 ;
	buf[0] = 0x40; buf[1] = 0x30; buf[2] = 0x20;
	buf[3] = 0x10; buf[4] = 0x08;
	ret = usb_microdia_control_write(dev, reg, buf, 5);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0x80; buf[1] = 0x30; buf[2] = 0x00; buf[3] = 0x00; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x00; buf[8] = 0x03;
	ret = usb_microdia_control_write(dev, reg, buf, 9);
	if (ret < 0)
		goto err;

	reg = 0x10e0 ;
	buf[0] = 0x44; buf[1] = 0x18; buf[2] = 0x00; buf[3] = 0x30;
	buf[4] = 0x00; buf[5] = 0x09; buf[6] = 0x00; buf[7] = 0xed;
	buf[8] = 0x0f; buf[9] = 0xda; buf[10] = 0x0f; buf[11] = 0x3a;
	buf[12] = 0x00; buf[13] = 0x3a; buf[14] = 0x00; buf[15] = 0xd0;
	buf[16] = 0x0f; buf[17] = 0xf7; buf[18] = 0x0f; buf[19] = 0x00;
	buf[20] = 0x00; buf[21] = 0x00; buf[22] = 0x60; buf[23] = 0x0a;
	ret = usb_microdia_control_write(dev, reg, buf, 24);
	if (ret < 0)
		goto err;

	reg = 0x10f8 ;
	buf[0] = 0x00; buf[1] = 0x00; buf[2] = 0x00;
	ret = usb_microdia_control_write(dev, reg, buf, 3);
	if (ret < 0)
		goto err;

	reg = 0x1188 ;
	buf[0] = 0x86;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x118a ;
	buf[0] = 0x08;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x118b ;
	buf[0] = 0x00; buf[1] = 0x20; buf[2] = 0x20; buf[3] = 0x20;
	buf[4] = 0x20; buf[5] = 0x00; buf[6] = 0x24; buf[7] = 0x3b;
	buf[8] = 0x4f; buf[9] = 0x61; buf[10] = 0x71; buf[11] = 0x80;
	buf[12] = 0x8f; buf[13] = 0x9d; buf[14] = 0xab; buf[15] = 0xb8;
	buf[16] = 0xc4; buf[17] = 0xd1; buf[18] = 0xdd; buf[19] = 0xe9;
	buf[20] = 0xf4; buf[21] = 0xff;
	ret = usb_microdia_control_write(dev, reg, buf, 22);
	if (ret < 0)
		goto err;

	reg = 0x11a1 ;
	buf[0] = 0x00; buf[1] = 0x00;
	ret = usb_microdia_control_write(dev, reg, buf, 2);
	if (ret < 0)
		goto err;

	reg = 0x11b7 ;
	buf[0] = 0x60; buf[1] = 0x20; buf[2] = 0x00; buf[3] = 0x08;
	buf[4] = 0x00; buf[5] = 0x40; buf[6] = 0x01; buf[7] = 0xf0;
	buf[8] = 0x00;
	ret = usb_microdia_control_write(dev, reg, buf, 9);
	if (ret < 0)
		goto err;

	reg = 0x11b8 ;
	buf[0] = 0x30;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x1000 ;
	buf[0] = 0x78;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x12; buf[3] = 0x80; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x12; buf[3] = 0x01; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x0d; buf[3] = 0x00; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x0e; buf[3] = 0x61; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x11; buf[3] = 0x80; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x13; buf[3] = 0xb8; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x14; buf[3] = 0x3e; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x16; buf[3] = 0x24; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x1e; buf[3] = 0x00; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x1e; buf[3] = 0x00; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x1e; buf[3] = 0x04; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x27; buf[3] = 0x08; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x28; buf[3] = 0x08; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x29; buf[3] = 0x15; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x2c; buf[3] = 0x08; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x32; buf[3] = 0x80; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x34; buf[3] = 0x3d; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x35; buf[3] = 0x00; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x36; buf[3] = 0xf8; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x38; buf[3] = 0x12; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x39; buf[3] = 0x57; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x3a; buf[3] = 0x00; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x3b; buf[3] = 0xcc; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x3c; buf[3] = 0x0c; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x3d; buf[3] = 0x19; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x3e; buf[3] = 0x0c; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x3f; buf[3] = 0x01; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x41; buf[3] = 0x40; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x42; buf[3] = 0x80; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x45; buf[3] = 0x46; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x46; buf[3] = 0x62; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x47; buf[3] = 0x2a; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x48; buf[3] = 0x3c; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x4a; buf[3] = 0xf0; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x4b; buf[3] = 0xdc; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x4c; buf[3] = 0xdc; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x4d; buf[3] = 0xdc; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x4e; buf[3] = 0xdc; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x69; buf[3] = 0x02; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x6c; buf[3] = 0x04; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x6f; buf[3] = 0x9e; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x70; buf[3] = 0x05; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x71; buf[3] = 0x78; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x77; buf[3] = 0x02; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x8a; buf[3] = 0x23; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x8c; buf[3] = 0x0d; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x90; buf[3] = 0x7e; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x91; buf[3] = 0x7c; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x9f; buf[3] = 0x6e; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0xa0; buf[3] = 0x6e; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0xa4; buf[3] = 0x50; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0xa5; buf[3] = 0x68; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0xa6; buf[3] = 0x60; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0xa8; buf[3] = 0xc1; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0xa9; buf[3] = 0xfa; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0xaa; buf[3] = 0x92; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0xab; buf[3] = 0x04; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0xac; buf[3] = 0x80; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0xad; buf[3] = 0x80; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0xae; buf[3] = 0x80; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0xaf; buf[3] = 0x80; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0xb2; buf[3] = 0xf2; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0xb3; buf[3] = 0x20; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0xb4; buf[3] = 0x20; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0xb5; buf[3] = 0x00; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0xb6; buf[3] = 0xaf; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0xbb; buf[3] = 0xae; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0xbc; buf[3] = 0x44; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0xbd; buf[3] = 0x44; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0xbe; buf[3] = 0x3b; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0xbf; buf[3] = 0x3a; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0xc0; buf[3] = 0xe2; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0xc1; buf[3] = 0xc8; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0xc2; buf[3] = 0x01; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0xc4; buf[3] = 0x00; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0xc6; buf[3] = 0x85; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0xc7; buf[3] = 0x81; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0xc9; buf[3] = 0xe0; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0xca; buf[3] = 0xe8; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0xcc; buf[3] = 0xd8; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0xcd; buf[3] = 0x93; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c1 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c1 ;
	buf[0] = 0x50;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0x90; buf[1] = 0x50; buf[2] = 0x00; buf[3] = 0x00; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0x90; buf[1] = 0x50; buf[2] = 0x00; buf[3] = 0x00; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0x90; buf[1] = 0x50; buf[2] = 0x00; buf[3] = 0x00; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0x90; buf[1] = 0x50; buf[2] = 0x00; buf[3] = 0x00; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0x90; buf[1] = 0x50; buf[2] = 0x00; buf[3] = 0x00; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0x90; buf[1] = 0x50; buf[2] = 0x00; buf[3] = 0x00; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0x90; buf[1] = 0x50; buf[2] = 0x00; buf[3] = 0x00; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0x90; buf[1] = 0x50; buf[2] = 0x00; buf[3] = 0x00; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0x90; buf[1] = 0x50; buf[2] = 0x00; buf[3] = 0x00; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0x92; buf[1] = 0x50; buf[2] = 0x00; buf[3] = 0x00; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0x92; buf[1] = 0x50; buf[2] = 0x00; buf[3] = 0x00; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0x92; buf[1] = 0x50; buf[2] = 0x00; buf[3] = 0x00; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0x92; buf[1] = 0x50; buf[2] = 0x00; buf[3] = 0x00; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0x92; buf[1] = 0x50; buf[2] = 0x00; buf[3] = 0x00; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0x92; buf[1] = 0x50; buf[2] = 0x00; buf[3] = 0x00; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0x92; buf[1] = 0x50; buf[2] = 0x00; buf[3] = 0x00; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0x92; buf[1] = 0x50; buf[2] = 0x00; buf[3] = 0x00; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0x92; buf[1] = 0x50; buf[2] = 0x00; buf[3] = 0x00; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c2 ;
	ret = usb_microdia_control_read(dev, reg, buf, 5);
	if (ret < 0)
		goto err;

	reg = 0x10c1 ;
	buf[0] = 0x30;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10e0 ;
	buf[0] = 0x47;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10e0 ;
	buf[0] = 0x47;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x1001 ;
	buf[0] = 0xc6;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x1001 ;
	buf[0] = 0xc4;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x1001 ;
	buf[0] = 0x84;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x11bc ;
	buf[0] = 0x00;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x11bd ;
	buf[0] = 0x00;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x11be ;
	buf[0] = 0x00;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x11bf ;
	buf[0] = 0x00;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x03; buf[3] = 0x14; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xd0; buf[1] = 0x30; buf[2] = 0x17; buf[3] = 0x1b; buf[4] = 0xbc; buf[5] = 0x01; buf[6] = 0x7a; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0x90; buf[1] = 0x30; buf[2] = 0x1b; buf[3] = 0x00; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x32; buf[3] = 0x82; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x1180 ;
	buf[0] = 0x05; buf[1] = 0x00; buf[2] = 0x01; buf[3] = 0x00; buf[4] = 0x50; buf[5] = 0x78;
	ret = usb_microdia_control_write(dev, reg, buf, 6);
	if (ret < 0)
		goto err;

	reg = 0x11a1 ;
	buf[0] = 0x00; buf[1] = 0x00; buf[2] = 0xd5; buf[3] = 0xa0;
	ret = usb_microdia_control_write(dev, reg, buf, 4);
	if (ret < 0)
		goto err;

	reg = 0x11ab ;
	buf[0] = 0x00; buf[1] = 0x00; buf[2] = 0xa0; buf[3] = 0x78;
	ret = usb_microdia_control_write(dev, reg, buf, 4);
	if (ret < 0)
		goto err;

	reg = 0x1000 ;
	buf[0] = 0x78;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x1002 ;
	buf[0] = 0x18;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x1002 ;
	buf[0] = 0x08;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x11b8 ;
	buf[0] = 0x38;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x118a ;
	buf[0] = 0x02;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x0395 ;
	buf[0] = 0x02;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x11b8 ;
	buf[0] = 0x78;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x11b8 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x11b8 ;
	buf[0] = 0xf9;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x11b8 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x11b8 ;
	buf[0] = 0x7a;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x11b8 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x11b8 ;
	buf[0] = 0x7b;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x11b8 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x11b8 ;
	buf[0] = 0x7c;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x11b8 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x11b8 ;
	buf[0] = 0xfd;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x11b8 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x11b8 ;
	buf[0] = 0xfa;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x11; buf[3] = 0x80; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x2a; buf[3] = 0x00; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x2b; buf[3] = 0x06; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x92; buf[3] = 0x00; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x93; buf[3] = 0x00; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x1e; buf[3] = 0x04; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x1e; buf[3] = 0x04; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x1182 ;
	buf[0] = 0x01;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x10; buf[3] = 0x7d; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x04; buf[3] = 0x00; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0xa1; buf[3] = 0x00; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x2d; buf[3] = 0x00; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x2e; buf[3] = 0x00; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x00; buf[3] = 0x00; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x01; buf[3] = 0x80; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x02; buf[3] = 0x80; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x11ba ;
	buf[0] = 0x0a;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x118b ;
	buf[0] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10f7;
	buf[0] = 0x05;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10f8 ;
	buf[0] = 0x14;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10fa ;
	buf[0] = 0xff;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10f9 ;
	buf[0] = 0x00;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10f9 ;
	buf[0] = 0x00;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x11ba ;
	buf[0] = 0x0a;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x11bc ;
	buf[0] = 0x00; buf[1] = 0x00; buf[2] = 0x00; buf[3] = 0x00;
	ret = usb_microdia_control_write(dev, reg, buf, 4);
	if (ret < 0)
		goto err;

	reg = 0x11c0;
	buf[0] = 0x00; buf[1] = 0x00; buf[2] = 0x00; buf[3] = 0x00;
	buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x00;
	buf[8] = 0x00; buf[9] = 0x00; buf[10] = 0x00; buf[11] = 0x00;
	buf[12] = 0x00; buf[13] = 0x00; buf[14] = 0x00; buf[15] = 0x00;
	buf[16] = 0x00; buf[17] = 0x00; buf[18] = 0x00; buf[19] = 0x00;
	buf[20] = 0x00; buf[21] = 0x00; buf[22] = 0x00; buf[23] = 0x00;
	buf[24] = 0x00; buf[25] = 0x00; buf[26] = 0x00; buf[27] = 0x00;
	buf[28] = 0x00; buf[29] = 0x00; buf[30] = 0x00; buf[31] = 0x00;
	buf[32] = 0x00; buf[33] = 0x00; buf[34] = 0x00; buf[35] = 0x00;
	buf[36] = 0x00; buf[37] = 0x00; buf[38] = 0x00; buf[39] = 0x00;
	buf[40] = 0x00; buf[41] = 0x00; buf[42] = 0x00; buf[43] = 0x00;
	buf[44] = 0x00; buf[45] = 0x00; buf[46] = 0x00; buf[47] = 0x00;
	ret = usb_microdia_control_write(dev, reg, buf, 48);
	if (ret < 0)
		goto err;

	reg = 0x11a5 ;
	buf[0] = 0x2d; buf[1] = 0x2d; buf[2] = 0x3a; buf[3] = 0x05;
	buf[4] = 0x04; buf[5] = 0x3f;
	ret = usb_microdia_control_write(dev, reg, buf, 6);
	if (ret < 0)
		goto err;

	reg = 0x11af ;
	buf[0] = 0x28; buf[1] = 0xd8; buf[2] = 0x14; buf[3] = 0xec;
	ret = usb_microdia_control_write(dev, reg, buf, 4);
	if (ret < 0)
		goto err;

	reg = 0x11b3 ;
	buf[0] = 0x32; buf[1] = 0xdd; buf[2] = 0x32; buf[3] = 0xdd;
	ret = usb_microdia_control_write(dev, reg, buf, 4);
	if (ret < 0)
		goto err;

	reg = 0x10e0 ;
	buf[0] = 0x47;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x1061 ;
	buf[0] = 0x01;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10e0 ;
	buf[0] = 0x67;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x1100 ;
	buf[0] = 0x0d; buf[1] = 0x08; buf[2] = 0x08; buf[3] = 0x0d;
	buf[4] = 0x08; buf[5] = 0x08; buf[6] = 0x0d; buf[7] = 0x0d;
	buf[8] = 0x0d; buf[9] = 0x0d; buf[10] = 0x11; buf[11] = 0x0d;
	buf[12] = 0x0d; buf[13] = 0x11; buf[14] = 0x15; buf[15] = 0x21;
	buf[16] = 0x15; buf[17] = 0x15; buf[18] = 0x11; buf[19] = 0x11;
	buf[20] = 0x15; buf[21] = 0x2a; buf[22] = 0x1d; buf[23] = 0x1d;
	buf[24] = 0x19; buf[25] = 0x21; buf[26] = 0x32; buf[27] = 0x2a;
	buf[28] = 0x32; buf[29] = 0x32; buf[30] = 0x2e; buf[31] = 0x2a;
	buf[32] = 0x2e; buf[33] = 0x2e; buf[34] = 0x36; buf[35] = 0x3a;
	buf[36] = 0x4b; buf[37] = 0x43; buf[38] = 0x36; buf[39] = 0x3a;
	buf[40] = 0x47; buf[41] = 0x3a; buf[42] = 0x2e; buf[43] = 0x2e;
	buf[44] = 0x43; buf[45] = 0x5c; buf[46] = 0x43; buf[47] = 0x47;
	buf[48] = 0x4f; buf[49] = 0x53; buf[50] = 0x58; buf[51] = 0x58;
	buf[52] = 0x58; buf[53] = 0x32; buf[54] = 0x3f; buf[55] = 0x60;
	buf[56] = 0x64; buf[57] = 0x5c; buf[58] = 0x53; buf[59] = 0x64;
	buf[60] = 0x4b; buf[61] = 0x53; buf[62] = 0x58; buf[63] = 0x53;
	ret = usb_microdia_control_write(dev, reg, buf, 64);
	if (ret < 0)
		goto err;

	reg = 0x1140 ;
	buf[0] = 0x0d; buf[1] = 0x11; buf[2] = 0x11; buf[3] = 0x15;
	buf[4] = 0x11; buf[5] = 0x15; buf[6] = 0x26; buf[7] = 0x15;
	buf[8] = 0x15; buf[9] = 0x26; buf[10] = 0x53; buf[11] = 0x36;
	buf[12] = 0x2e; buf[13] = 0x36; buf[14] = 0x53; buf[15] = 0x53;
	buf[16] = 0x53; buf[17] = 0x53; buf[18] = 0x53; buf[19] = 0x53;
	buf[20] = 0x53; buf[21] = 0x53; buf[22] = 0x53; buf[23] = 0x53;
	buf[24] = 0x53; buf[25] = 0x53; buf[26] = 0x53; buf[27] = 0x53;
	buf[28] = 0x53; buf[29] = 0x53; buf[30] = 0x53; buf[31] = 0x53;
	buf[32] = 0x53; buf[33] = 0x53; buf[34] = 0x53; buf[35] = 0x53;
	buf[36] = 0x53; buf[37] = 0x53; buf[38] = 0x53; buf[39] = 0x53;
	buf[40] = 0x53; buf[41] = 0x53; buf[42] = 0x53; buf[43] = 0x53;
	buf[44] = 0x53; buf[45] = 0x53; buf[46] = 0x53; buf[47] = 0x53;
	buf[48] = 0x53; buf[49] = 0x53; buf[50] = 0x53; buf[51] = 0x53;
	buf[52] = 0x53; buf[53] = 0x53; buf[54] = 0x53; buf[55] = 0x53;
	buf[56] = 0x53; buf[57] = 0x53; buf[58] = 0x53; buf[59] = 0x53;
	buf[60] = 0x53; buf[61] = 0x53; buf[62] = 0x53; buf[63] = 0x53;
	ret = usb_microdia_control_write(dev, reg, buf, 64);
	if (ret < 0)
		goto err;

	reg = 0x10e0 ;
	buf[0] = 0x47;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x1061 ;
	buf[0] = 0x03;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10e0 ;
	buf[0] = 0x4b;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x11bc ;
	buf[0] = 0x00;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x11bd ;
	buf[0] = 0x00;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x11be ;
	buf[0] = 0x00;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x11bf ;
	buf[0] = 0x00;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x03; buf[3] = 0x14; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xd0; buf[1] = 0x30; buf[2] = 0x17; buf[3] = 0x1b; buf[4] = 0xbc; buf[5] = 0x01; buf[6] = 0x7a; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0x90; buf[1] = 0x30; buf[2] = 0x1b; buf[3] = 0x00; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x32; buf[3] = 0x82; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x1180 ;
	buf[0] = 0x05; buf[1] = 0x00; buf[2] = 0x01; buf[3] = 0x00;
	buf[4] = 0x50; buf[5] = 0x78;
	ret = usb_microdia_control_write(dev, reg, buf, 6);
	if (ret < 0)
		goto err;

	reg = 0x11a1 ;
	buf[0] = 0x00; buf[1] = 0x00; buf[2] = 0xd5; buf[3] = 0xa0;
	ret = usb_microdia_control_write(dev, reg, buf, 4);
	if (ret < 0)
		goto err;

	reg = 0x11ab ;
	buf[0] = 0x00; buf[1] = 0x00; buf[2] = 0xa0; buf[3] = 0x78;
	ret = usb_microdia_control_write(dev, reg, buf, 4);
	if (ret < 0)
		goto err;

	reg = 0x1061 ;
	buf[0] = 0x03;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x11ba ;
	buf[0] = 0x0a;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x11b9 ;
	buf[0] = 0x00;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x11ba ;
	buf[0] = 0x0b;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x1061 ;
	buf[0] = 0x01;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x1000 ;
	buf[0] = 0x78;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x1002 ;
	buf[0] = 0x08;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x1002 ;
	buf[0] = 0x08;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x11b8 ;
	buf[0] = 0xfa;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x118a ;
	buf[0] = 0x02;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x0395 ;
	buf[0] = 0x02;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x11b8 ;
	buf[0] = 0xf8;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x11b8 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x11b8 ;
	buf[0] = 0xf9;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x11b8 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x11b8 ;
	buf[0] = 0x7a;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x11b8 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x11b8 ;
	buf[0] = 0x7b;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x11b8 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x11b8 ;
	buf[0] = 0x7c;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x11b8 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x11b8 ;
	buf[0] = 0xfd;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x11b8 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x11b8 ;
	buf[0] = 0xfa;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x11; buf[3] = 0x80; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x2a; buf[3] = 0x00; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x2b; buf[3] = 0x70; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x92; buf[3] = 0x00; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x93; buf[3] = 0x00; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0x90; buf[1] = 0x30; buf[2] = 0x0a; buf[3] = 0x00; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0x92; buf[1] = 0x30; buf[2] = 0x00; buf[3] = 0x00; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c2 ;
	ret = usb_microdia_control_read(dev, reg, buf, 5);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0x90; buf[1] = 0x30; buf[2] = 0x0b; buf[3] = 0x00; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0x92; buf[1] = 0x30; buf[2] = 0x00; buf[3] = 0x00; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c2 ;
	ret = usb_microdia_control_read(dev, reg, buf, 5);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0x90; buf[1] = 0x30; buf[2] = 0x1c; buf[3] = 0x00; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0x92; buf[1] = 0x30; buf[2] = 0x00; buf[3] = 0x00; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c2 ;
	ret = usb_microdia_control_read(dev, reg, buf, 5);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0x90; buf[1] = 0x30; buf[2] = 0x1d; buf[3] = 0x00; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0x92; buf[1] = 0x30; buf[2] = 0x00; buf[3] = 0x00; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c2 ;
	ret = usb_microdia_control_read(dev, reg, buf, 5);
	if (ret < 0)
		goto err;

	reg = 0x1061 ;
	buf[0] = 0x03;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x1007 ;
	buf[0] = 0x20;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x1006 ;
	buf[0] = 0x00;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0xa1; buf[3] = 0x01; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x10; buf[3] = 0x06; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x04; buf[3] = 0x00; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x2d; buf[3] = 0x36; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x2e; buf[3] = 0x00; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x00; buf[3] = 0x01; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x15;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x00; buf[3] = 0x06; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x15;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x01; buf[3] = 0x94; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x02; buf[3] = 0x8d; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x00; buf[3] = 0x09; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x15;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x01; buf[3] = 0x95; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x02; buf[3] = 0x85; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x00; buf[3] = 0x0c; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x15;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x01; buf[3] = 0x96; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x02; buf[3] = 0x80; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	buf[0] = 0xa0; buf[1] = 0x30; buf[2] = 0x02; buf[3] = 0x7c; buf[4] = 0x00; buf[5] = 0x00; buf[6] = 0x00; buf[7] = 0x10;
	ret = usb_microdia_control_write(dev, reg, buf, 8);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x10c0 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x1007 ;
	buf[0] = 0x20;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x1006 ;
	buf[0] = 0x20;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;




	/* END OF MICRODIA 6288 START STREAM */

	return 0;
err:
	UDIA_ERROR("r/w in Register %x failed (%d)!\n", reg, ret);
	return ret;
}


/**
 * @brief Wrapper function for device-specific stopstream functions
 *
 * @param dev Pointer to device structure
 *
 * @returns 0
 *
 * This will execute a function determined by the array #cameras and
 * the function find_camera().
 *
 */
int dev_microdia_stop_stream(struct usb_microdia *dev)
{
	int ret = -ENODEV;
	if (dev && dev->stop_stream)
		ret = dev->stop_stream(dev);

	return ret;
}

/**
 * @brief From stop.htm
 *
 * @param dev
 *
 * @returns 0
 *
 * @author GWater
 *
 * For SN9C201 with MI1310.
 * This function has not been tested yet.
 */
int microdia_6242_stop_stream(struct usb_microdia *dev)
{
	__u8 buf[3];

	__u8 only10c0[3] = {0x0d, 0x00, 0x00};

	usb_microdia_control_read(dev, 0x1061, buf, 1);
	buf[0] &= ~0x02;
	usb_microdia_control_write(dev, 0x1061, buf, 1);

	buf[0] = 0x60;
	usb_microdia_control_write(dev, 0x1007, buf, 1);

	buf[0] = 0x00;
	usb_microdia_control_write(dev, 0x1006, buf, 1);

	sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 2,
		only10c0[0], dev->sensor_flags, &only10c0[1]);

	buf[0] = 0x38;
	usb_microdia_control_write(dev, 0x1000, buf, 1);

	buf[0] = 0x6b;
	usb_microdia_control_write(dev, 0x11b8, buf, 1);

	buf[0] = 0x60;
	usb_microdia_control_write(dev, 0x1007, buf, 1);

	buf[0] = 0x20;
	usb_microdia_control_write(dev, 0x1006, buf, 1);


	buf[0] = 0x98; buf[1] = 0xe7; buf[2] = 0x11;
	usb_microdia_control_write(dev, 0x1000, buf, 3);

	/**
	 * After the select alternate setting 0 is actually another transfer:
	 *	buf[0] = 0x00;
	 *	usb_microdia_control_write(dev, 0x1066, buf, 1);
	 */

	return 0;
}


/**
 * @brief From stopstream.log
 *
 * @param dev
 *
 * @returns 0
 *
 * @author GWater
 *
 * For SN9C201 with SOI968.
 * I don't whether the function really stops the stream.
 * Nevertheless the LED on the webcam now stops glowing.
 * The function is very similar to "microdia_624f_stop_stream".
 */
int microdia_624e_stop_stream(struct usb_microdia *dev)
{
	__u8 buf[3];

	buf[0] = 0x01;
	usb_microdia_control_write(dev, 0x1061, buf, 1);

	/* Next two writes added because it works better.
	 * The LED stops glowing and I think the stream does too.
	 * 0x20 = 0b00100000 which is the minimal of what the
	 * other stopstreams have in common.
	 */
	buf[0] = 0x20;
	usb_microdia_control_write(dev, 0x1007, buf, 1);

	buf[0] = 0x20;
	usb_microdia_control_write(dev, 0x1006, buf, 1);

	buf[0] = 0x3c;
	usb_microdia_control_write(dev, 0x1000, buf, 1);

	buf[0] = 0x6a;
	usb_microdia_control_write(dev, 0x11b8, buf, 1);

	buf[0] = 0x38; buf[1] = 0xe7; buf[2] = 0x11;
	usb_microdia_control_write(dev, 0x1000, buf, 3);

	usb_microdia_control_read(dev, 0x1066, buf, 1);

	return 0;
}


/**
 * @brief Stop device 624f from streaming
 *
 * @param dev Pointer to device structure
 *
 * @returns 0 or negative error code
 *
 */
int microdia_624f_stop_stream(struct usb_microdia *dev)
{
	int ret;
	__u16 command;
	__u8 buf[3];

	buf[0] = 0x02;
	command = 0x1066;
	ret = usb_microdia_control_write(dev, command, buf, 1);
	if (ret < 0)
		goto err;

	buf[0] = 0x01;
	command = 0x1061;
	ret = usb_microdia_control_write(dev, command, buf, 1);
	if (ret < 0)
		goto err;

	buf[0] = 0x38;
	command = 0x1000;
	ret = usb_microdia_control_write(dev, command, buf, 1);
	if (ret < 0)
		goto err;

	buf[0] = 0x6a;
	command = 0x11b8;
	ret = usb_microdia_control_write(dev, command, buf, 1);
	if (ret < 0)
		goto err;

	buf[0] = 0x20;
	command = 0x1007;
	ret = usb_microdia_control_write(dev, command, buf, 1);
	if (ret < 0)
		goto err;

	buf[0] = 0x20;
	command = 0x1006;
	ret = usb_microdia_control_write(dev, command, buf, 1);
	if (ret < 0)
		goto err;

	buf[0] = 0x98;
	buf[1] = 0xe7;
	buf[2] = 0x11;
	command = 0x1000;
	ret = usb_microdia_control_write(dev, command, buf, 3);
	if (ret < 0)
		goto err;

	return ret;

err:
	UDIA_ERROR("command %x failed (%d)!\n", command, ret);
	return ret;
}

/**
 * @brief Stop device 6260 from streaming
 *
 * @param dev Pointer to device structure
 *
 * @returns 0 or negative error code
 *
 */
int microdia_6260_stop_stream(struct usb_microdia *dev)
{
	int ret = 0;
	int i;
	__u16 reg;
	__u8  value[1];

	static __u16 bridge6260_stop[][2] = {
	{0x1061, 0x01},
	{0x1000, 0x38},
	{0x11b8, 0x6b},
	{0x1007, 0x20},
	{0x1006, 0x20},
	{0x1000, 0x98},
	{0x1001, 0xe7},
	{0x1002, 0x11},
	{0x1066, 0x00},
	{0x1045, 0x0f},
	{0x10c8, 0x00},
	{0x1020, 0x80},
	};

	UDIA_INFO("Stopping camera...\n");

	for (i = 0; i < ARRAY_SIZE(bridge6260_stop); i++) {
		reg = bridge6260_stop[i][0];
		value[0] = bridge6260_stop[i][1];
		ret = usb_microdia_control_write(dev, reg, value, 1);
		if (ret < 0) {
			UDIA_INFO("6260 Bridge: Stop Sequence Error (%d) on array line %d\n", ret, i);
			break;
		}
	}

	return 0;
}

/**
 * @brief From 7 start-stop-logs (made by Alex Carlos, Boris Borisov, bohzan and Comer352l)
 *
 * @param dev
 *
 * @returns 0 (ok) or -1 (error)
 *
 * @author Comer352l
 *
 * Windows driver version: 5.7.23.000
 * Windows versions: 2000 and XP
 * USB-Logger: SniffUSB 1.8 and 2.0, USBTrace 2.0, SnoopyPro
 * All logs were made using AMCAP with 640x480, RGB24
 */
int microdia_6270_stop_stream(struct usb_microdia *dev)
{
	int ret = 0;
	int retI2C = 0;
	__u8 buf[3];

	if (dev->sensor_slave_address == MT9V111_I2C_SLAVE_ADDRESS) {
		buf[0] = 0x20;
		ret += usb_microdia_control_write(dev, 0x1007, buf, 1);		/*  URB 3541 */
		buf[0] = 0x20;
		ret += usb_microdia_control_write(dev, 0x1006, buf, 1);		/*  URB 3542 */
		/*  => These two writes seem to cause the cam to stop sending isochronus USB messages */
	}

	ret += usb_microdia_control_read(dev, 0x1061, buf, 1);
	buf[0] &= ~0x02;

	ret += usb_microdia_control_write(dev, 0x1061, buf, 1);		/*  URB 3544 */
	if (dev->sensor_slave_address == MT9V011_I2C_SLAVE_ADDRESS) {
		buf[0] = 0x60;
		ret += usb_microdia_control_write(dev, 0x1007, buf, 1);		/*  URB 3541 */
		buf[0] = 0x60;
		ret += usb_microdia_control_write(dev, 0x1006, buf, 1);		/*  URB 3542 */
	}
	if (dev->sensor_slave_address == MT9V011_I2C_SLAVE_ADDRESS) {
		/*  Write to sensor register	0x07: Output Control */
		buf[0] = 0x00; buf[1] = 0x00;	/*  stop sensor readout, normal operation */
		retI2C += sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 2, 0x07, dev->sensor_flags, buf);
	}
	buf[0] = 0x38;		/*  ALEX: 0x3c */
	ret += usb_microdia_control_write(dev, 0x1000, buf, 1);		/*  URB 3545 */
	buf[0] = 0x6d;		/*  ALEX: 0xec; FRANK: also 0x6c */
	ret += usb_microdia_control_write(dev, 0x11b8, buf, 1);		/*  URB 3546 */
	buf[0] = 0x38; buf[1] = 0xe7; buf[2] = 0x11;
	ret += usb_microdia_control_write(dev, 0x1000, buf, 3);		/*  URB 3547 */

	/*  INTERRUPT COMMING BACK (URB 2227) */
	/*  SELECT ALTERNATE SETTINGS (URB 3548) */
	/*  INTERRUPT GOING DOWN (URB 3549) */

	buf[0] = 0x00;
	ret += usb_microdia_control_write(dev, 0x1066, buf, 1);		/*  URB 3550 */

	if (ret < 9) {
		UDIA_INFO("One ore more errors occured while stopping stream !\n");
		return -1;
	} else {
		return 0;
	}
}

/**
 * @brief Stop device 627b from streaming
 *
 * @param dev Pointer to device structure
 *
 * @returns 0 or negative error code
 *
 */
int microdia_627b_stop_stream(struct usb_microdia *dev)
{
	int ret = 0;
	__u16 reg;
	__u8 buf[3];

	reg = 0x1007;
	buf[0] = 0x60;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x1006;
	buf[0] = 0x20;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x1061;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	buf[0] &= ~0x02;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x1000;
	buf[0] = 0x38;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x11b8;
	buf[0] = 0x6b;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x1000;
	buf[0] = 0x38; buf[1] = 0xe7; buf[2] = 0x11;
	ret = usb_microdia_control_write(dev, reg, buf, 3);
	if (ret < 0)
		goto err;

	reg = 0x1066;
	buf[0] = 0x00;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	return ret;

err:
	UDIA_ERROR("r/w in Register %x failed (%d)!\n", reg, ret);
	return ret;
}

/**
 * @brief From UsbSnoop-plugin-parsed.log
 *
 * @param dev
 *
 * @returns 0 (OK) or <0 (Error)
 *
 * @author Vincent, Kuzja
 */
int microdia_6288_stop_stream(struct usb_microdia *dev)
{
	int ret = 0;
	__u16 reg;
	__u8 buf[8];

	reg = 0x1061 ;
	ret = usb_microdia_control_read(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	buf[0] &= ~0x02;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x1000 ;
	buf[0] = 0x38;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x11b8 ;
	buf[0] = 0xe9;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x1007 ;
	buf[0] = 0x20;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x1006 ;
	buf[0] = 0x20;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	reg = 0x1000 ;
	buf[0] = 0x98; buf[1] = 0xe7; buf[2] = 0x11;
	ret = usb_microdia_control_write(dev, reg, buf, 3);
	if (ret < 0)
		goto err;

	reg = 0x1066 ;
	buf[0] = 0x00;
	ret = usb_microdia_control_write(dev, reg, buf, 1);
	if (ret < 0)
		goto err;

	return 0;
err:
	UDIA_ERROR("r/w in Register %x failed (%d)!\n", reg, ret);
	return ret;
}

/**
 * @brief Detect whether the image for 624f has to be flipped
 *
 * @param dev Pointer to device structure
 *
 * @returns 0 or negative error code
 *
 */
int microdia_624f_flip_detect(struct usb_microdia *dev)
{
	int ret = 0;
	__u8 val;

	ret = usb_microdia_control_read(dev, 0x1009, &val, 1);
	if (ret < 0)
		return -EAGAIN;
	if (val & 0x01)
		dev->vsettings.vflip = 1;
	else
		dev->vsettings.vflip = 0;
	return ret;
}

/**
 * @brief Detect whether the image for 6260 has to be flipped
 *
 * @param dev Pointer to device structure
 *
 * @returns 0 or negative error code
 *
 */
int microdia_6260_flip_detect(struct usb_microdia *dev)
{
	const __u8 flip_bit = 0x01;
	int ret = 0;
	__u8 val;
	static __u8 flip_reg = flip_bit;
	__u8 vflip;

	ret = usb_microdia_control_read(dev, 0x1009, &val, 1);
	if (ret < 0)
		return -EAGAIN;
	if (flip_reg != (val & flip_bit)) {
		if (val & flip_bit)
			vflip = 0;
		else
			vflip = 1;
		ret = ov7670_auto_flip(dev, vflip);
		flip_reg = (val & flip_bit);
	}

	return ret;
}



int microdia_6242_set_autofeature(struct usb_microdia *dev)
{
    int i=0;
    __u8 buf[][3] = {
        {0xf0, 0x00, 0x01},     // page=1
        {0x06, 0x30, 0x82},     // default auto exp. off
        {0xf0, 0x00, 0x00}      // switch back to page=0
    };

    // make sure on page=1
    sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 2,
            buf[0][0], dev->sensor_flags, &buf[0][1]);

    // read bytes from 0x06 address
    sn9c20x_read_i2c_data(dev, dev->sensor_slave_address, 2,
            buf[1][0], dev->sensor_flags, &buf[1][1]);

    // set bit 14 of 0x06 address enable auto exp.
    if(dev->vsettings.auto_exposure) buf[1][1] |= 0x40;         // enable auto exposure
    else buf[1][1] &= ~0x40;                                    // disable auto exposure

    if(dev->vsettings.auto_whitebalance) buf[1][2] |= 0x02;    // enable auto white balance
    else buf[1][2] &= ~0x02;                                  // disable auto white balance

    for(i=1;i<3;i++){
        sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 2,
                buf[i][0], dev->sensor_flags, &buf[i][1]);
    }

    return 0;
}

int microdia_6242_set_exposure(struct usb_microdia *dev)
{
    __u8 buf[3];
    unsigned int exposure = dev->vsettings.exposure;

    buf[0]=0xf0; buf[1]=0x00; buf[2]=0x00;

    sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 2,
            buf[0], dev->sensor_flags, &buf[1]);

    buf[0]=0x09;
    buf[1]=(__u8)(exposure >> 7);
    buf[2]=(__u8)(exposure & 0x00ff);

    sn9c20x_write_i2c_data(dev, dev->sensor_slave_address, 2,
            buf[0], dev->sensor_flags, &buf[1]);

    return 0;
}
