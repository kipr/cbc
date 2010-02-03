#include "cbob_accel.h"
#include "cbob_spi.h"
#include "cbob_cmd.h"

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <asm/semaphore.h>
#include <asm/uaccess.h>

static int cbob_accel_major = CBOB_ACCEL_MAJOR;

/* File Ops */

static ssize_t cbob_accel_read(struct file *file, char *buf, size_t count, loff_t *ppos);
static int     cbob_accel_ioctl(struct inode *inode, struct file *file, unsigned int ioctl_num, unsigned long ioctl_param);
static int     cbob_accel_open(struct inode *inode, struct file *file);
static int     cbob_accel_release(struct inode *inode, struct file *file);

static struct file_operations cbob_accel_fops = {
	owner:   THIS_MODULE,
	open:    cbob_accel_open,
	release: cbob_accel_release,
	read:    cbob_accel_read,
	ioctl:   cbob_accel_ioctl
};

static int cbob_accel_open(struct inode *inode, struct file *file)
{
  struct accel_axis *accel;
  
  accel = kmalloc(sizeof(struct accel_axis), GFP_KERNEL);
  
  if(accel == 0)
    return -ENOMEM;
    
  accel->axis = iminor(inode);
  
  file->private_data = accel;
  
  
  return 0;
}

static int cbob_accel_release(struct inode *inode, struct file *file)
{
  kfree(file->private_data);
  return 0;
}

static ssize_t cbob_accel_read(struct file *file, char *buf, size_t count, loff_t *ppos) 
{
  struct accel_axis *accel = file->private_data;
  short data[3] = {0,0,0};
  int error;
  
  if((error = cbob_spi_message(CBOB_CMD_ACCEL_READ, &(accel->axis), 1, data, 3)) < 0)
    return error;
  
  if(count > 6)
    count = 6;
  
  copy_to_user(buf, (char*)&data, count);
  
  return count;
}

static int cbob_accel_ioctl(struct inode *inode, struct file *file, unsigned int ioctl_num, unsigned long ioctl_param)
{
    short request[4];
    short result[3];
    int error;

    switch(ioctl_num) {
      case CBOB_ACCEL_RECALIBRATE:
        request[0] = 0;
        if((error = cbob_spi_message(CBOB_CMD_ACCEL_CONFIG, request, 1, 0, 0)) < 0)
          return error;
        break;
      case CBOB_ACCEL_GET_CAL:
        request[0] = 1;
        if((error = cbob_spi_message(CBOB_CMD_ACCEL_CONFIG, request, 1, result, 3)) < 0)
          return error;
        copy_to_user((void*)ioctl_param, result, sizeof(short)*3);
        break;
      case CBOB_ACCEL_SET_CAL:
        request[0] = 2;
        copy_from_user(&(request[1]), (void*)ioctl_param, sizeof(short)*3);
        if((error = cbob_spi_message(CBOB_CMD_ACCEL_CONFIG, request, 4, 0, 0)) < 0)
          return error;
        break;
    }
    return 0;
}

/* init and exit */
int cbob_accel_init(void)
{
  int error;
  
  error = register_chrdev(cbob_accel_major, CBOB_ACCEL_NAME, &cbob_accel_fops);
  
  if(error < 0) {
    printk(KERN_ALERT "Failed to register cbob_accel char device with error: %d\n", error);
    return error;
  }
  
  return 0;
}

void cbob_accel_exit(void)
{
  int error;
  
  error = unregister_chrdev(cbob_accel_major, CBOB_ACCEL_NAME);
  if(error < 0) {
    printk(KERN_ALERT "Failed to unregister cbob_accel char device with error: %d\n", error);
  }
}
