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
	struct pid_port *pid = file->private_data;
	short data[4];
	int error;
	
	if(count > 6) count = 6;
	
	copy_from_user(((void*)data)+2, buf, count);
	data[0] = pid->port;
	
	if((error = cbob_spi_message(CBOB_CMD_PID_WRITE, data, (count>>1)+1, 0, 0)) < 0)
		return error;
	
  return 0;
}

static int cbob_pid_ioctl(struct inode *inode, struct file *file, unsigned int ioctl_num, unsigned long ioctl_param)
{
	struct pid_port *pid = file->private_data;
	short request[8];
  short result[8];
	int error, arg;
	
	switch(ioctl_num) {
		case CBOB_PID_CLEAR_COUNTER:
			request[0] = 0;
			request[1] = pid->port;
			if((error = cbob_spi_message(CBOB_CMD_PID_CONFIG, request, 2, 0, 0)) < 0)
				return error;
			break;
		case CBOB_PID_SET_COUNTER:
			/*copy_from_user(&arg, (void*)ioctl_param, sizeof(int));
			request[0] = 1;
			request[1] = pid->port;
			memcpy(((void*)request)+4, &arg, sizeof(int));*/
			// not implemented currently
			break;
		case CBOB_PID_SET_GAINS:
			request[0] = 2;
			request[1] = pid->port;
			copy_from_user(((void*)request)+4, (void*)ioctl_param, sizeof(short)*6);
			if((error = cbob_spi_message(CBOB_CMD_PID_CONFIG, request, 8, 0, 0)) < 0)
				return error;
			break;
		case CBOB_PID_GET_GAINS:
			request[0] = 3;
			request[1] = pid->port;
			if((error = cbob_spi_message(CBOB_CMD_PID_CONFIG, request, 2, result, 6)) < 0)
				return error;
      copy_to_user((void*)ioctl_param, result, 14);
			break;
		case CBOB_PID_GET_DONE:
			request[0] = 4;
      request[1] = pid->port;
      if((error = cbob_spi_message(CBOB_CMD_PID_CONFIG, request, 2, result, 1)) < 0)
				return error;
			arg = result[0];
			copy_to_user((void*)ioctl_param, &arg, sizeof(int));
			break;
		case CBOB_PID_RESET_GAINS:
			request[0] = 5;
			request[1] = pid->port;
			if((error = cbob_spi_message(CBOB_CMD_PID_CONFIG, request, 2, 0, 0)) < 0)
				return error;
			break;
		case CBOB_PID_RECALIBRATE:
		  request[0] = 6;
		  if((error = cbob_spi_message(CBOB_CMD_PID_CONFIG, request, 1, 0, 0)) < 0)
				return error;
			break;
    case CBOB_PID_GET_CAL:
      request[0] = 7;
      if((error = cbob_spi_message(CBOB_CMD_PID_CONFIG, request, 1, result, 4)) < 0)
        return error;
      copy_to_user((void*)ioctl_param, result, sizeof(short)*4);
      break;
    case CBOB_PID_SET_CAL:
      request[0] = 8;
      copy_from_user(&(request[1]), (void*)ioctl_param, sizeof(short)*4);
      if((error = cbob_spi_message(CBOB_CMD_PID_CONFIG, request, 5, 0, 0)) < 0);
        return error;
      break;
	}
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
