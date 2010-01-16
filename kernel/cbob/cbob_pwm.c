#include "cbob_pwm.h"
#include "cbob_spi.h"
#include "cbob_cmd.h"

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <asm/semaphore.h>
#include <asm/uaccess.h>

static int cbob_pwm_major = CBOB_PWM_MAJOR;

/* File Ops */

static ssize_t cbob_pwm_write(struct file *file, const char *buf, size_t count, loff_t *ppos);
static ssize_t cbob_pwm_read(struct file *file, char *buf, size_t count, loff_t *ppos);
static int     cbob_pwm_ioctl(struct inode *inode, struct file *file, unsigned int ioctl_num, unsigned long ioctl_param);
static int     cbob_pwm_open(struct inode *inode, struct file *file);
static int     cbob_pwm_release(struct inode *inode, struct file *file);

static struct file_operations cbob_pwm_fops = {
	owner:   THIS_MODULE,
	open:    cbob_pwm_open,
	release: cbob_pwm_release,
	read:    cbob_pwm_read,
	write:   cbob_pwm_write,
	ioctl:   cbob_pwm_ioctl
};

static int cbob_pwm_open(struct inode *inode, struct file *file)
{
  struct motor_pwm *pwm;
  
  pwm = kmalloc(sizeof(struct motor_pwm), GFP_KERNEL);
  
  if(pwm == 0)
    return -ENOMEM;
    
  pwm->port = iminor(inode);
  
  file->private_data = pwm;
  
  return 0;
}

static int cbob_pwm_release(struct inode *inode, struct file *file)
{
  kfree(file->private_data);
  return 0;
}

static ssize_t cbob_pwm_read(struct file *file, char *buf, size_t count, loff_t *ppos) 
{
  struct motor_pwm *pwm = file->private_data;
  short data[4] = {0,0,0,0};
  signed char outdata[4] = {0,0,0,0};
  int i, error;
  
  if((error = cbob_spi_message(CBOB_CMD_PWM_READ, &(pwm->port), 1, data, 4)) < 0)
    return error;
  
  for(i = 0;i < 4;i++)
	  outdata[i] = data[i];

  if(count > 4)
    count = 4;
 
  copy_to_user(buf, (char*)&outdata, count);
  
  return count;
}

static ssize_t cbob_pwm_write(struct file *file, const char *buf, size_t count, loff_t *ppos)
{
  struct motor_pwm *pwm = file->private_data;
  short data[5] = {0,0,0,0,0};
  char user_data[4] = {0,0,0,0};
  int i, error;
  
  if(count == 0) return 0;
  
  copy_from_user(user_data, buf, (count <= 4 ? count : 4));
  
  data[0] = pwm->port;
  
  for(i = 0;i < count;i++)
    data[i+1] = (signed char)user_data[i];
  
  if((error = cbob_spi_message(CBOB_CMD_PWM_WRITE, data, 1 + count, 0, 0)) < 0)
    return error;
  
  return 0;
}

static int cbob_pwm_ioctl(struct inode *inode, struct file *file, unsigned int ioctl_num, unsigned long ioctl_param)
{
  return 0;
}

/* init and exit */
int cbob_pwm_init(void)
{
  int error;
  
  error = register_chrdev(cbob_pwm_major, CBOB_PWM_NAME, &cbob_pwm_fops);
  
  if(error < 0) {
    printk(KERN_ALERT "Failed to register cbob_pwm char device with error: %d\n", error);
    return error;
  }
  
  return 0;
}

void cbob_pwm_exit(void)
{
  int error;
  
  error = unregister_chrdev(cbob_pwm_major, CBOB_PWM_NAME);
  if(error < 0) {
    printk(KERN_ALERT "Failed to unregister cbob_pwm char device with error: %d\n", error);
  }
}
