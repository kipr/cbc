#include "cbob_status.h"
#include "cbob_spi.h"
#include "cbob_cmd.h"

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <asm/semaphore.h>
#include <asm/uaccess.h>

static int cbob_status_major = CBOB_STATUS_MAJOR;

/* File Ops */

static ssize_t cbob_status_read(struct file *file, char *buf, size_t count, loff_t *ppos);
static int     cbob_status_open(struct inode *inode, struct file *file);
static int     cbob_status_release(struct inode *inode, struct file *file);

static struct file_operations cbob_status_fops = {
	owner:   THIS_MODULE,
    open:    cbob_status_open,
    release: cbob_status_release,
	read:    cbob_status_read
};

static int cbob_status_open(struct inode *inode, struct file *file)
{
  return 0;
}

static int cbob_status_release(struct inode *inode, struct file *file)
{
  return 0;
}

static ssize_t cbob_status_read(struct file *file, char *buf, size_t count, loff_t *ppos) 
{
  short data;
  char version;
  int error;
  
  if((error = cbob_spi_message(CBOB_CMD_STATUS_READ, 0, 0, &data, 1)) < 0)
    return error;
  
  if(count > 1)
    count = 1;

  version = data;
  
  copy_to_user(buf, &version, count);
  
  return count;
}

/* init and exit */
int cbob_status_init(void)
{
  int error;
  
  error = register_chrdev(cbob_status_major, CBOB_STATUS_NAME, &cbob_status_fops);
  
  if(error < 0) {
    printk(KERN_ALERT "Failed to register cbob_status char device with error: %d\n", error);
    return error;
  }
  
  return 0;
}

void cbob_status_exit(void)
{
  int error;
  
  error = unregister_chrdev(cbob_status_major, CBOB_STATUS_NAME);
  if(error < 0) {
    printk(KERN_ALERT "Failed to unregister cbob_status char device with error: %d\n", error);
  }
}
