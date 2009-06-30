#include "cbob_uart.h"
#include "cbob_spi.h"
#include "cbob_cmd.h"

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <asm/semaphore.h>
#include <asm/uaccess.h>

static int cbob_uart_major = CBOB_UART_MAJOR;

/* File Ops */

static ssize_t cbob_uart_read(struct file *file, char *buf, size_t count, loff_t *ppos);
static ssize_t cbob_uart_write(struct file *file, const char *buf, size_t count, loff_t *ppos);
static int     cbob_uart_ioctl(struct inode *inode, struct file *file, unsigned int ioctl_num, unsigned long ioctl_param);
static int     cbob_uart_open(struct inode *inode, struct file *file);
static int     cbob_uart_release(struct inode *inode, struct file *file);

static struct file_operations cbob_uart_fops = {
	owner:   THIS_MODULE,
	open:    cbob_uart_open,
	release: cbob_uart_release,
	read:    cbob_uart_read,
	write:   cbob_uart_write,
	ioctl:   cbob_uart_ioctl
};

static int cbob_uart_open(struct inode *inode, struct file *file)
{
  struct uart_port *uart;
  
  uart = kmalloc(sizeof(struct uart_port), GFP_KERNEL);
  
  if(uart == 0)
    return -ENOMEM;
    
  uart->port = iminor(inode);
  
  file->private_data = uart;
  
  
  return 0;
}

static int cbob_uart_release(struct inode *inode, struct file *file)
{
  kfree(file->private_data);
  return 0;
}

static ssize_t cbob_uart_read(struct file *file, char *buf, size_t count, loff_t *ppos) 
{
  struct uart_port *uart = file->private_data;
  short data;
  int error;
  
  if((error = cbob_spi_message(CBOB_CMD_UART_READ, &(uart->port), 1, &data, 1)) < 0)
    return error;
  
  if(count > 2)
    count = 2;
  
  copy_to_user(buf, (char*)&data, count);
  
  return count;

}

static ssize_t cbob_uart_write(struct file *file, const char *buf, size_t count, loff_t *ppos)
{
	struct uart_port *uart = file->private_data;
	char kbuf;
	short data;
	int error;
	
	if(count == 0)
		return 0;
	
	copy_from_user(&kbuf, buf, 1);
	data = kbuf;
	
	if((error = cbob_spi_message(CBOB_CMD_UART_WRITE, &(uart->port), 1, &data, 1)) < 0)
		return error;
	
  return 1;
}

static int cbob_uart_ioctl(struct inode *inode, struct file *file, unsigned int ioctl_num, unsigned long ioctl_param)
{
  return 0;
}

/* init and exit */
int cbob_uart_init(void)
{
  int error;
  
  error = register_chrdev(cbob_uart_major, CBOB_UART_NAME, &cbob_uart_fops);
  
  if(error < 0) {
    printk(KERN_ALERT "Failed to register cbob_uart char device with error: %d\n", error);
    return error;
  }
  
  return 0;
}

void cbob_uart_exit(void)
{
  int error;
  
  error = unregister_chrdev(cbob_uart_major, CBOB_UART_NAME);
  if(error < 0) {
    printk(KERN_ALERT "Failed to unregister cbob_uart char device with error: %d\n", error);
  }
}
