#include "cbob_sensors.h"
#include "cbob_spi.h"
#include "cbob_cmd.h"

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <asm/semaphore.h>
#include <asm/uaccess.h>

static int cbob_sensors_major = CBOB_SENSORS_MAJOR;

/* File Ops */

static ssize_t cbob_sensors_read(struct file *file, char *buf, size_t count, loff_t *ppos);
static int     cbob_sensors_open(struct inode *inode, struct file *file);
static int     cbob_sensors_release(struct inode *inode, struct file *file);

static struct file_operations cbob_sensors_fops = {
	owner:   THIS_MODULE,
	open:    cbob_sensors_open,
	release: cbob_sensors_release,
	read:    cbob_sensors_read
};

static int cbob_sensors_open(struct inode *inode, struct file *file)
{
  return 0;
}

static int cbob_sensors_release(struct inode *inode, struct file *file)
{
  return 0;
}

static ssize_t cbob_sensors_read(struct file *file, char *buf, size_t count, loff_t *ppos) 
{
  short data[14] = {0,0,0,0,0,0,0,0,0,0,0,0};
  int error;
  
  if((error = cbob_spi_message(CBOB_CMD_SENSORS_READ, 0, 0, data, 14)) < 0)
    return error;
  
  if(count > 28)
    count = 28;
  
  copy_to_user(buf, data, count);
  
  return 0;
}

/* init and exit */
int cbob_sensors_init(void)
{
  int error;
  
  error = register_chrdev(cbob_sensors_major, CBOB_SENSORS_NAME, &cbob_sensors_fops);
  
  if(error < 0) {
    printk(KERN_ALERT "Failed to register cbob_sensors char device with error: %d\n", error);
    return error;
  }
  
  return 0;
}

void cbob_sensors_exit(void)
{
  int error;
  
  error = unregister_chrdev(cbob_sensors_major, CBOB_SENSORS_NAME);
  if(error < 0) {
    printk(KERN_ALERT "Failed to unregister cbob_sensors char device with error: %d\n", error);
  }
}
