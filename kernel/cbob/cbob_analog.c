#include "cbob_analog.h"
#include "cbob_spi.h"
#include "cbob_cmd.h"

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <asm/semaphore.h>
#include <asm/uaccess.h>

static int cbob_analog_major = CBOB_ANALOG_MAJOR;

/* File Ops */

static ssize_t cbob_analog_read(struct file *file, char *buf, size_t count, loff_t *ppos);
static int     cbob_analog_open(struct inode *inode, struct file *file);
static int     cbob_analog_release(struct inode *inode, struct file *file);
static int     cbob_analog_ioctl(struct inode *inode, struct file *file, unsigned int ioctl_num, unsigned long ioctl_param);

static struct file_operations cbob_analog_fops = {
	owner:   THIS_MODULE,
	open:    cbob_analog_open,
	release: cbob_analog_release,
	read:    cbob_analog_read,
	ioctl:   cbob_analog_ioctl
};

static int cbob_analog_open(struct inode *inode, struct file *file)
{
  struct analog_port *analog;
  
  analog = kmalloc(sizeof(struct analog_port), GFP_KERNEL);
  
  if(analog == 0)
    return -ENOMEM;
    
  analog->port = iminor(inode);
  
  file->private_data = analog;
  
  return 0;
}

static int cbob_analog_release(struct inode *inode, struct file *file)
{
  kfree(file->private_data);
  return 0;
}

static ssize_t cbob_analog_read(struct file *file, char *buf, size_t count, loff_t *ppos) 
{
  struct analog_port *analog = file->private_data;
  short data[9] = {0,0,0,0,0,0,0,0, 0};
  int error;
  
  if((error = cbob_spi_message(CBOB_CMD_ANALOG_READ, &(analog->port), 1, data, 9)) < 0)
    return error;
  
  if(count > 18)
    count = 18; 
  
  copy_to_user(buf, (char*)&data, count);
  
  return count;
}

static int cbob_analog_ioctl(struct inode *inode, struct file *file, unsigned int ioctl_num, unsigned long ioctl_param)
{
	short request[2];
	short retval;
	int arg, error;
	
	copy_from_user(&arg, (void*)ioctl_param, sizeof(int));
	
	switch(ioctl_num) {
		case CBOB_ANALOG_SET_PULLUPS:
			request[0] = 0;
			request[1] = arg;
			if((error = cbob_spi_message(CBOB_CMD_ANALOG_CONFIG, request, 2, 0,0)) < 0)
				return error;
			break;
		case CBOB_ANALOG_GET_PULLUPS:
			request[0] = 1;
			if((error = cbob_spi_message(CBOB_CMD_ANALOG_CONFIG, request, 1, &retval, 1)) < 0)
				return error;
			arg = retval;
			break;
	}
	
	copy_to_user((void*)ioctl_param, &arg, sizeof(int));
	
  return 0;
}

/* init and exit */
int cbob_analog_init(void)
{
  int error;
  
  error = register_chrdev(cbob_analog_major, CBOB_ANALOG_NAME, &cbob_analog_fops);
  
  if(error < 0) {
    printk(KERN_ALERT "Failed to register cbob_analog char device with error: %d\n", error);
    return error;
  }
  
  return 0;
}

void cbob_analog_exit(void)
{
  int error;
  
  error = unregister_chrdev(cbob_analog_major, CBOB_ANALOG_NAME);
  if(error < 0) {
    printk(KERN_ALERT "Failed to unregister cbob_analog char device with error: %d\n", error);
  }
}
