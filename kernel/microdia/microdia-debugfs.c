/**
 * @file microdia-debugfs.c
 * @author Brian johnson
 * @date 2008-03-28
 *
 * @brief Debugfs interface for direct device manipulation
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

#include <linux/debugfs.h>
#include <linux/seq_file.h>

#include "microdia.h"
#include "sn9c20x.h"

#undef DEFINE_SIMPLE_ATTRIBUTE
#define DEFINE_SIMPLE_ATTRIBUTE(__fops, __get, __set, __fmt)         \
static void __fops ## _set_wrapper(void *data, __u64 val)            \
{                                                                    \
	__set(data, val);                                            \
}                                                                    \
static __u64 __fops ## _get_wrapper(void *data)                      \
{                                                                    \
	__u64 value;                                                 \
	__get(data, &value);                                         \
	return value;                                                \
}                                                                    \
static int __fops ## _open(struct inode *inode, struct file *file)   \
{                                                                    \
	__simple_attr_check_format(__fmt, 0ull);                     \
	return simple_attr_open(inode, file,                         \
				__fops ## _get_wrapper,              \
				__fops ## _set_wrapper,              \
				__fmt);                              \
}                                                                    \
static struct file_operations __fops = {                             \
	.owner   = THIS_MODULE,                                      \
	.open    = __fops ## _open,                                  \
	.release = simple_attr_close,                                \
	.read    = simple_attr_read,                                 \
	.write   = simple_attr_write,                                \
};

/**
 * @var debug_dir_name
 *   Name of our directory in debugfs
 */
static const char *debug_dir_name = "microdia";

/**
 * @var debug_dir
 *   Dentry for our debug dir (for cleanup)
 */
static struct dentry *debug_dir;

struct kref debug_ref;
/**
 * @var dent_log_level
 *   debugfs entry for dynamically changing the log level
 */
static struct dentry *dent_log_level;

static int debugfs_u16_set(void *data, __u64 val)
{
	*(u16 *)data = val;
	return 0;
}

static int debugfs_u16_get(void *data, __u64 *val)
{
	*val = *(u16 *)data;
	return 0;
}

static int debugfs_u8_set(void *data, __u64 val)
{
	*(u8 *)data = val;
	return 0;
}

static int debugfs_u8_get(void *data, __u64 *val)
{
	*val = *(u8 *)data;
	return 0;
}

static void *bridge_dump_start(struct seq_file *m, loff_t *pos)
{
	loff_t *spos = kmalloc(sizeof(loff_t), GFP_KERNEL);
	if (spos == NULL)
		return ERR_PTR(-ENOMEM);

	if (*pos < 0 || *pos > 31)
		return NULL;

	*spos = *pos;
	return spos;
}

static void *bridge_dump_next(struct seq_file *m, void *v, loff_t *pos)
{
	loff_t *spos = (loff_t *)v;
	*pos = ++(*spos);

	if (*spos > 31)
		return ERR_PTR(-EACCES);

	return spos;
}

static void bridge_dump_stop(struct seq_file *m, void *v)
{
	if (v && !IS_ERR(v))
		kfree(v);
}

/**
 * @brief Print out the values of registers 0x1000-0x1200 of the bridge
 *
 * @param m
 * @param v
 *
 * @return 0
 *
 */
static int bridge_dump_show(struct seq_file *m, void *v)
{
	__u8 values[16];
	loff_t *spos = (loff_t *)v;
	__u16 reg = 0x1000 + (*spos * 16);
	struct usb_microdia *dev = m->private;

	if (usb_microdia_control_read(dev, reg, values, 16) < 0)
		return -EACCES;

	seq_printf(m, "0x%X %02X %02X %02X %02X %02X %02X %02X %02X "
		   "%02X %02X %02X %02X %02X %02X %02X %02X\n", reg,
		   values[0], values[1], values[2], values[3],
		   values[4], values[5], values[6], values[7],
		   values[8], values[9], values[10], values[11],
		   values[12], values[13], values[14], values[15]);

	return 0;
}

static struct seq_operations bridge_dump_seq_ops = {
	.start =	bridge_dump_start,
	.next =		bridge_dump_next,
	.stop =		bridge_dump_stop,
	.show =		bridge_dump_show
};

static int bridge_dump_open(struct inode *inode, struct file *file)
{
	int ret;
	ret = seq_open(file, &bridge_dump_seq_ops);
	if (ret == 0)
		((struct seq_file *)file->private_data)->private =
			inode->u.generic_ip;
	return ret;
}

static struct file_operations bridge_dump_ops = {
	.owner		= THIS_MODULE,
	.open		= bridge_dump_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= seq_release,
};

/**
 * @brief Set the value for a specific register of the bridge
 *
 * @param data
 * @param val
 *
 * @return 0
 *
 */
int bridge_value_set(void *data, __u64 val)
{
	struct usb_microdia *dev = data;
	__u8 value = (__u64)val;
	if (dev != NULL) {
		if (dev->debug.bridge_addr >= 0x1000 &&
		    dev->debug.bridge_addr <= 0x11ff) {
			usb_microdia_control_write(dev,
						   dev->debug.bridge_addr,
						   &value, 1);
		}
	}
	return 0;
}

/**
 * @brief Print out the value of a specific register of the bridge
 *
 * @param data
 * @param val
 *
 * @return 0
 *
 */
int bridge_value_get(void *data, __u64 *val)
{
	struct usb_microdia *dev = data;
	__u8 value;
	if (dev != NULL) {
		if (dev->debug.bridge_addr >= 0x1000 &&
		    dev->debug.bridge_addr <= 0x11ff) {
			if (usb_microdia_control_read(dev,
						      dev->debug.bridge_addr,
						      &value, 1) >= 0) {
				*val = value;
			}
		}
	}
	return 0;
}

/**
 * @brief Set the value (8-bit) for a specific register of the sensor
 *
 * @param data
 * @param val
 *
 * @return 0
 *
 */
int sensor_value8_set(void *data, __u64 val)
{
	struct usb_microdia *dev = data;
	__u8 value = (__u8)val;
	if (dev != NULL) {
		sn9c20x_write_i2c_data(dev, dev->sensor_slave_address,
				       1, dev->debug.sensor_addr,
				       dev->sensor_flags, &value);
	}
	return 0;
}

/**
 * @brief Print out the value (8-bit) of a specific register of the sensor
 *
 * @param data
 * @param val
 *
 * @return 0
 *
 */
int sensor_value8_get(void *data, __u64 *val)
{
	struct usb_microdia *dev = data;
	__u8 value;
	if (dev != NULL) {
		if (sn9c20x_read_i2c_data(dev, dev->sensor_slave_address,
					  1, dev->debug.sensor_addr,
					  dev->sensor_flags, &value) >= 0) {
			*val = value & 0xFF;
		}
	}
	return 0;
}

/**
 * @brief Set the value (16-bit) for a specific register of the sensor
 *
 * @param data
 * @param val
 *
 * @return 0
 *
 */
int sensor_value16_set(void *data, __u64 val)
{
	struct usb_microdia *dev = data;
	__u8 buf[2];
	buf[0] = (val >> 8) & 0xFF;
	buf[1] = val & 0xFF;
	if (dev != NULL) {
		sn9c20x_write_i2c_data(dev, dev->sensor_slave_address,
				       2, dev->debug.sensor_addr,
				       dev->sensor_flags, buf);
	}
	return 0;
}

/**
 * @brief Print out the value (16-bit) of a specific register of the sensor
 *
 * @param data
 * @param val
 *
 * @return 0
 *
 */
int sensor_value16_get(void *data, __u64 *val)
{
	struct usb_microdia *dev = data;
	__u8 buf[2];
	if (dev != NULL) {
		if (sn9c20x_read_i2c_data(dev, dev->sensor_slave_address,
					  2, dev->debug.sensor_addr,
					  dev->sensor_flags, buf) >= 0) {
			*val = ((buf[0] << 8) | buf[1]) & 0xFFFF;
		}
	}
	return 0;
}

/**
 * @brief Set the value (32-bit) for a specific register of the sensor
 *
 * @param data
 * @param val
 *
 * @return 0
 *
 */
int sensor_value32_set(void *data, __u64 val)
{
	struct usb_microdia *dev = data;
	__u8 buf[4];
	buf[0] = (val >> 24) & 0xFF;
	buf[1] = (val >> 16) & 0xFF;
	buf[2] = (val >> 8) & 0xFF;
	buf[3] = val & 0xFF;
	if (dev != NULL) {
		sn9c20x_write_i2c_data(dev, dev->sensor_slave_address,
				       4, dev->debug.sensor_addr,
				       dev->sensor_flags, buf);
	}
	return 0;
}

/**
 * @brief Print out the value (32-bit) of a specific register of the sensor
 *
 * @param data
 * @param val
 *
 * @return 0
 *
 */
int sensor_value32_get(void *data, __u64 *val)
{
	struct usb_microdia *dev = data;
	__u8 buf[4];
	if (dev != NULL) {
		if (sn9c20x_read_i2c_data(dev, dev->sensor_slave_address,
					  4, dev->debug.sensor_addr,
					  dev->sensor_flags, buf) >= 0) {
			*val = ((buf[0] << 24) | (buf[1] << 16)
			       | (buf[2] << 8) | buf[3]) & 0xFFFFFFFF;
		}
	}
	return 0;
}

DEFINE_SIMPLE_ATTRIBUTE(fops_x16,
			debugfs_u16_get,
			debugfs_u16_set,
			"0x%04llx\n");

DEFINE_SIMPLE_ATTRIBUTE(fops_x8,
			debugfs_u8_get,
			debugfs_u8_set,
			"0x%02llx\n");

DEFINE_SIMPLE_ATTRIBUTE(bridge_value_ops,
			bridge_value_get,
			bridge_value_set,
			"0x%02llx\n");

DEFINE_SIMPLE_ATTRIBUTE(sensor_value8_ops,
			sensor_value8_get,
			sensor_value8_set,
			"0x%02llX\n");

DEFINE_SIMPLE_ATTRIBUTE(sensor_value16_ops,
			sensor_value16_get,
			sensor_value16_set,
			"0x%04llX\n");

DEFINE_SIMPLE_ATTRIBUTE(sensor_value32_ops,
			sensor_value32_get,
			sensor_value32_set,
			"0x%08llX\n");

void debugfs_delete(struct kref *ref)
{
	if (debug_dir)
		debugfs_remove(debug_dir);
}

void microdia_init_debugfs()
{
	debug_dir = debugfs_create_dir(debug_dir_name, NULL);
	if (debug_dir)
		dent_log_level = debugfs_create_u8("log_level",
						   S_IRUGO | S_IWUGO,
						   debug_dir,
						   &log_level);
	kref_init(&debug_ref);
}


void microdia_uninit_debugfs()
{
	if (dent_log_level)
		debugfs_remove(dent_log_level);
	kref_put(&debug_ref, debugfs_delete);
}

/**
 * @brief Create the 'debug' entries.
 *
 * This function permits to create all the entries in the 'debug' filesystem.
 *
 * @param dev device structure
 *
 * @returns 0 if all is OK
 */
int microdia_create_debugfs_files(struct usb_microdia *dev)
{
	char device_name[9];
	if (debug_dir) {
		snprintf(device_name, 9, "video%d", dev->vdev->minor);
		dev->debug.dent_device =
			debugfs_create_dir(device_name, debug_dir);
		if (dev->debug.dent_device) {
			dev->debug.bridge_addr = 0x1000;
			dev->debug.sensor_addr = 0x00;
			dev->debug.dent_bridge_addr =
				debugfs_create_file("bridge.address",
						   S_IRUGO | S_IWUGO,
						   dev->debug.dent_device,
						   &dev->debug.bridge_addr,
						   &fops_x16);
			dev->debug.dent_bridge_val =
				debugfs_create_file("bridge.value",
						    S_IRUGO | S_IWUGO,
						    dev->debug.dent_device,
						    dev, &bridge_value_ops);
			dev->debug.dent_bridge_dump =
				debugfs_create_file("bridge.dump",
						    S_IRUGO,
						    dev->debug.dent_device,
						    dev, &bridge_dump_ops);
			dev->debug.dent_sensor_addr =
				debugfs_create_file("sensor.address",
						  S_IRUGO | S_IWUGO,
						  dev->debug.dent_device,
						  &dev->debug.sensor_addr,
						  &fops_x8);
			dev->debug.dent_sensor_val8 =
				debugfs_create_file("sensor.value8",
						    S_IRUGO | S_IWUGO,
						    dev->debug.dent_device,
						    dev, &sensor_value8_ops);
			dev->debug.dent_sensor_val16 =
				debugfs_create_file("sensor.value16",
						    S_IRUGO | S_IWUGO,
						    dev->debug.dent_device,
						    dev, &sensor_value16_ops);
			dev->debug.dent_sensor_val32 =
				debugfs_create_file("sensor.value32",
						    S_IRUGO | S_IWUGO,
						    dev->debug.dent_device,
						    dev, &sensor_value32_ops);
		}
	}
	kref_get(&debug_ref);
	return 0;
}

/**
 * @brief Remove the 'debugfs' entries.
 *
 * This function permits to remove all the entries in the 'debug' filesystem.
 *
 * @param dev device structure
 *
 * @returns 0 if all is OK
 */
int microdia_remove_debugfs_files(struct usb_microdia *dev)
{
	if (dev->debug.dent_bridge_val)
		debugfs_remove(dev->debug.dent_bridge_val);
	if (dev->debug.dent_bridge_addr)
		debugfs_remove(dev->debug.dent_bridge_addr);
	if (dev->debug.dent_bridge_dump)
		debugfs_remove(dev->debug.dent_bridge_dump);
	if (dev->debug.dent_sensor_addr)
		debugfs_remove(dev->debug.dent_sensor_addr);
	if (dev->debug.dent_sensor_val8)
		debugfs_remove(dev->debug.dent_sensor_val8);
	if (dev->debug.dent_sensor_val16)
		debugfs_remove(dev->debug.dent_sensor_val16);
	if (dev->debug.dent_sensor_val32)
		debugfs_remove(dev->debug.dent_sensor_val32);
	if (dev->debug.dent_device)
		debugfs_remove(dev->debug.dent_device);
	kref_put(&debug_ref, debugfs_delete);
	return 0;
}

