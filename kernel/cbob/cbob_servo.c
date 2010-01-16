#include "cbob_servo.h"
#include "cbob_spi.h"
#include "cbob_cmd.h"

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <asm/semaphore.h>
#include <asm/uaccess.h>

static int cbob_servo_major = CBOB_SERVO_MAJOR;

/* File Ops */

static ssize_t cbob_servo_read(struct file *file, char *buf, size_t count, loff_t *ppos);
static ssize_t cbob_servo_write(struct file *file, const char *buf, size_t count, loff_t *ppos);
static int     cbob_servo_ioctl(struct inode *inode, struct file *file, unsigned int ioctl_num, unsigned long ioctl_param);
static int     cbob_servo_open(struct inode *inode, struct file *file);
static int     cbob_servo_release(struct inode *inode, struct file *file);

static struct file_operations cbob_servo_fops = {
	owner:   THIS_MODULE,
	open:    cbob_servo_open,
	release: cbob_servo_release,
	read:    cbob_servo_read,
	write:   cbob_servo_write,
	ioctl:   cbob_servo_ioctl
};

static int cbob_servo_open(struct inode *inode, struct file *file)
{
  struct servo_port *servo;
  
  servo = kmalloc(sizeof(struct servo_port), GFP_KERNEL);
  
  if(servo == 0)
    return -ENOMEM;
    
  servo->port = iminor(inode);
  
  file->private_data = servo;
  
  
  return 0;
}

static int cbob_servo_release(struct inode *inode, struct file *file)
{
  kfree(file->private_data);
  return 0;
}

static ssize_t cbob_servo_read(struct file *file, char *buf, size_t count, loff_t *ppos) 
{
  struct servo_port *servo = file->private_data;
  short data[4] = {0,0,0,0};
  int error;
  
  if((error = cbob_spi_message(CBOB_CMD_SERVO_READ, &(servo->port), 1, data, 4)) < 0)
    return error;
  
  if(count > 8)
    count = 8;
  
  copy_to_user(buf, (char*)&data, count);
  
  return count;
}

static ssize_t cbob_servo_write(struct file *file, const char *buf, size_t count, loff_t *ppos)
{
	struct servo_port *servo = file->private_data;
	int error;
	short outdata[2] = {0,0};
	
	if(count > 2) count = 2;
	
	copy_from_user(((void*)outdata)+2, buf, count);
	outdata[0] = servo->port;
	
	if((error = cbob_spi_message(CBOB_CMD_SERVO_WRITE, outdata, 2, 0, 0)) < 0)
		return error;
		
	return count;
}

static int cbob_servo_ioctl(struct inode *inode, struct file *file, unsigned int ioctl_num, unsigned long ioctl_param)
{
	return 0;
}

/* init and exit */
int cbob_servo_init(void)
{
  int error;
  
  error = register_chrdev(cbob_servo_major, CBOB_SERVO_NAME, &cbob_servo_fops);
  
  if(error < 0) {
    printk(KERN_ALERT "Failed to register cbob_servo char device with error: %d\n", error);
    return error;
  }
  
  return 0;
}

void cbob_servo_exit(void)
{
  int error;
  
  error = unregister_chrdev(cbob_servo_major, CBOB_SERVO_NAME);
  if(error < 0) {
    printk(KERN_ALERT "Failed to unregister cbob_servo char device with error: %d\n", error);
  }
}
