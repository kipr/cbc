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

static ssize_t cbob_pwm_write(struct file *filp, const char *buf, size_t count, loff_t *ppos);
static ssize_t cbob_pwm_read(struct file *filp, char *buf, size_t count, loff_t *ppos);
static int     cbob_pwm_open(struct inode *inode, struct file *filp);
static int     cbob_pwm_release(struct inode *inode, struct file *filp);

static struct file_operations cbob_pwm_fops = {
	owner:   THIS_MODULE,
	open:    cbob_pwm_open,
	release: cbob_pwm_release,
	read:    cbob_pwm_read,
	write:   cbob_pwm_write
};

static int cbob_pwm_open(struct inode *inode, struct file *filp)
{
  struct motor_pwm *pwm;
  
  pwm = kmalloc(sizeof(struct motor_pwm), GFP_KERNEL);
  
  if(pwm == 0)
    return -ENOMEM;
    
  pwm->port = iminor(inode);
  
  filp->private_data = pwm;
  
  
  return 0;
}

static int cbob_pwm_release(struct inode *inode, struct file *filp)
{
  kfree(filp->private_data);
  return 0;
}

static ssize_t cbob_pwm_read(struct file *filp, char *buf, size_t count, loff_t *ppos) 
{
  struct motor_pwm *pwm = filp->private_data;
  short data[2];
  int error;
  
  if((error = cbob_spi_message(CBOB_CMD_PWM_READ, &(pwm->port), 1, data, 2)) < 0)
    return error;
  
  if(count > 4)
    count = 4;
  
  copy_to_user(buf, (char*)&data, count);
  
  return count;
}

static ssize_t cbob_pwm_write(struct file *filp, const char *buf, size_t count, loff_t *ppos)
{
  struct motor_pwm *pwm = filp->private_data;
  short data[3];
  char user_data[4] = {0,0,0,0};
  int error;
  
  if(count == 0) return 0;
  
  copy_from_user(user_data, buf, (count <= 4 ? count : 4));
  
  data[0] = pwm->port;
  
  if(pwm->port >= 0 && pwm->port <= 3)
    data[1] = (short)user_data[0];
  else
    memcpy(data+2, user_data, 4);
  
  if((error = cbob_spi_message(CBOB_CMD_PWM_WRITE, data, 3, 0, 0)) < 0)
    return error;
  
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
