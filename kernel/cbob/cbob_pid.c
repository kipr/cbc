#include "cbob_pid.h"
#include "cbob_spi.h"
#include "cbob_cmd.h"

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <asm/semaphore.h>
#include <asm/uaccess.h>

static int cbob_pid_major = CBOB_PID_MAJOR;

/* File Ops */

static ssize_t cbob_pid_read(struct file *file, char *buf, size_t count, loff_t *ppos);
static ssize_t cbob_pid_write(struct file *file, const char *buf, size_t count, loff_t *ppos);
static int     cbob_pid_ioctl(struct inode *inode, struct file *file, unsigned int ioctl_num, unsigned long ioctl_param);
static int     cbob_pid_open(struct inode *inode, struct file *file);
static int     cbob_pid_release(struct inode *inode, struct file *file);

static struct file_operations cbob_pid_fops = {
	owner:   THIS_MODULE,
	open:    cbob_pid_open,
	release: cbob_pid_release,
	read:    cbob_pid_read,
	write:   cbob_pid_write,
	ioctl:   cbob_pid_ioctl
};

static int cbob_pid_open(struct inode *inode, struct file *file)
{
  struct pid_port *pid;
  
  pid = kmalloc(sizeof(struct pid_port), GFP_KERNEL);
  
  if(pid == 0)
    return -ENOMEM;
    
  pid->port = iminor(inode);
  
  file->private_data = pid;
  
  
  return 0;
}

static int cbob_pid_release(struct inode *inode, struct file *file)
{
  kfree(file->private_data);
  return 0;
}

static ssize_t cbob_pid_read(struct file *file, char *buf, size_t count, loff_t *ppos) 
{
  struct pid_port *pid = file->private_data;
  int data[4] = {0,0,0,0};
  int error;
  
  if((error = cbob_spi_message(CBOB_CMD_PID_READ, &(pid->port), 1, (short*)data, 8)) < 0)
    return error;
  
  if(count > 16)
    count = 16;
  
  copy_to_user(buf, (char*)&data, count);
  
  return count;
}

static ssize_t cbob_pid_write(struct file *file, const char *buf, size_t count, loff_t *ppos)
{
  return 0;
}

static int cbob_pid_ioctl(struct inode *inode, struct file *file, unsigned int ioctl_num, unsigned long ioctl_param)
{
  return 0;
}

/* init and exit */
int cbob_pid_init(void)
{
  int error;
  
  error = register_chrdev(cbob_pid_major, CBOB_PID_NAME, &cbob_pid_fops);
  
  if(error < 0) {
    printk(KERN_ALERT "Failed to register cbob_pid char device with error: %d\n", error);
    return error;
  }
  
  return 0;
}

void cbob_pid_exit(void)
{
  int error;
  
  error = unregister_chrdev(cbob_pid_major, CBOB_PID_NAME);
  if(error < 0) {
    printk(KERN_ALERT "Failed to unregister cbob_pid char device with error: %d\n", error);
  }
}
