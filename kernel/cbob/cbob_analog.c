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

static struct file_operations cbob_analog_fops = {
	owner:   THIS_MODULE,
	open:    cbob_analog_open,
	release: cbob_analog_release,
	read:    cbob_analog_read
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
  short data[8] = {0,0,0,0,0,0,0,0};
  int error;
  
  if((error = cbob_spi_message(CBOB_CMD_ANALOG_READ, &(analog->port), 1, data, 8)) < 0)
    return error;
  
  if(count > 16)
    count = 16; 
  
  copy_to_user(buf, (char*)&data, count);
  
  return count;
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
