#include "cbob_uart.h"
#include "cbob_spi.h"
#include "cbob_cmd.h"

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <asm/semaphore.h>
#include <asm/uaccess.h>
#include <linux/delay.h>
#include <asm/arch/imx-regs.h>
#include <asm/arch/irqs.h>
#include <asm/arch/hardware.h>
#include <linux/interrupt.h>
#include <linux/tty.h>
#include <linux/tty_driver.h>
#include <linux/tty_flip.h>
#include <linux/workqueue.h>

/* File Ops */


static int cbob_uart_major = CBOB_UART_MAJOR;

struct cbob_uart {
  volatile int port;
  volatile int open_count;
  struct semaphore sem;
};

static int  cbob_uart_open(struct inode *inode, struct file *file);
static int  cbob_uart_close(struct inode *inode, struct file *file);
static int  cbob_uart_write(struct file *file, const char *buf, size_t count, loff_t *ppos);
static int  cbob_uart_read(struct file *file, char *buf, size_t count, loff_t *ppos);
static int  cbob_uart_ioctl(struct inode *inode, struct file *file, unsigned int ioctl_num, unsigned long ioctl_param);

static struct file_operations cbob_uart_fops = {
  open:       cbob_uart_open,
  release:    cbob_uart_close,
  write:      cbob_uart_write,
  read:       cbob_uart_read,
  ioctl:      cbob_uart_ioctl
};

struct cbob_uart cbob_uarts[CBOB_UART_MINORS];

static int cbob_uart_open(struct inode *inode, struct file *file)
{ 
	int port;
	struct cbob_uart *uart = 0;
  
  printk("%s\n", __FUNCTION__);
  
  port = iminor(inode);
  
  if(port > 1) return -ENODEV;
  
  uart = &cbob_uarts[port];
  
  if(down_interruptible(&uart->sem))
  	return -EINTR;
  
  if(uart->open_count) {
  	up(&uart->sem);
  	return -EBUSY;
  }
  
  uart->open_count = 1;
  
  file->private_data = uart;
  
  up(&uart->sem);
  
  return 0;
}


static int cbob_uart_close(struct inode *inode, struct file *file)
{
	struct cbob_uart *uart;
	
	printk("%s...", __FUNCTION__);
	
	uart = file->private_data;
	
	down(&uart->sem);
	
	uart->open_count = 0;
	
	up(&uart->sem);
	
	printk("returning...\n");
	return 0;
}

static ssize_t cbob_uart_write(struct file *file, const char *buf, size_t count, loff_t *ppos)
{
	struct cbob_uart *uart = file->private_data;
	int port = uart->port;
	short data[35], written;
	
	printk("%s\n", __FUNCTION__);
	
	if(count > 64) count = 64;
	
	if(!uart)
		return -ENODEV;
	
	if(down_interruptible(&uart->sem))
		return -EINTR;

	copy_from_user(&(data[2]), buf, count);
	data[0] = port;
	data[1] = count;
	
	cbob_spi_message(CBOB_CMD_UART_WRITE, data, (count>>1)+3, &written, 1);
	
	up(&uart->sem);
	
	return written;
}

static int  cbob_uart_read(struct file *file, char *buf, size_t count, loff_t *ppos)
{
	struct cbob_uart *uart = file->private_data;
	int port = uart->port;
	short data[33], request[2], data_read;
	
	printk("%s\n", __FUNCTION__);
	
	if(count > 64) count = 64;
	
	if(!uart)
		return -ENODEV;
		
	printk("uart is valid\n");
		
	if(down_interruptible(&uart->sem))
		return -EINTR;
		
	printk("got the semaphore\n");
		
	if(!(imx_gpio_read(GPIO_PORTKP)&1)) {
		up(&uart->sem);
		return 0;
	}
	
	printk("passed the io port test\n");

	request[0] = port;
	request[1] = count;
	cbob_spi_message(CBOB_CMD_UART_READ, request, 2, data, 33);
	
	printk("spi request completed\n");
	
	data_read = data[0];
	
	printk("read %d bytes\n", data_read);
	
	copy_to_user(buf, &(data[1]), data_read);
	
	printk("done!\n");
	
	up(&uart->sem);
	
	return data_read;
}


static int cbob_uart_ioctl(struct inode *inode, struct file *file, unsigned int ioctl_num, unsigned long ioctl_param)
{
	struct cbob_uart *uart = file->private_data;
	short request[2];
	short retval;
	int arg, error;
	
	printk("%s\n", __FUNCTION__);
	
	copy_from_user(&arg, (void*)ioctl_param, sizeof(int));
	
	switch(ioctl_num) {
		case CBOB_UART_SET_SIGMASK:
			request[0] = 0;
			request[1] = arg;
			if((error = cbob_spi_message(CBOB_CMD_UART_CONFIG, request, 2, 0,0)) < 0)
				return error;
			break;
		case CBOB_UART_GET_SIGMASK:
			request[0] = 1;
			if((error = cbob_spi_message(CBOB_CMD_UART_CONFIG, request, 1, &retval, 1)) < 0)
				return error;
			arg = retval;
			break;
	}
	
	copy_to_user((void*)ioctl_param, &arg, sizeof(int));
	
  return 0;
}

/* init and exit */
int cbob_uart_init(void)
{
	int error;
	
	printk("%s\n", __FUNCTION__);
	
	error = register_chrdev(cbob_uart_major, CBOB_UART_NAME, &cbob_uart_fops);
	
	if(error < 0) {
    printk(KERN_ALERT "Failed to register cbob_uart char device with error: %d\n", error);
    return error;
  }
  
  sema_init(&(cbob_uarts[0].sem), 1);
  sema_init(&(cbob_uarts[1].sem), 1);
  
  cbob_uarts[1].port = 1;
	
  return error;
}

void cbob_uart_exit(void)
{
  int error;
  
  printk("%s\n", __FUNCTION__);
  
  error = unregister_chrdev(cbob_uart_major, CBOB_UART_NAME);
  
  if(error < 0) {
    printk(KERN_ALERT "Failed to unregister cbob_uart char device with error: %d\n", error);
  }
}

