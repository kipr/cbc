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

struct cbob_uart {
  struct tty_struct *tty;
  int open_count;
  struct semaphore sem;
};

static int  cbob_uart_open(struct tty_struct *tty, struct file *filp);
static void cbob_uart_close(struct tty_struct *tty, struct file *filp);
static int  cbob_uart_write(struct tty_struct *tty, const unsigned char *buf, int count);
static int  cbob_uart_write_room(struct tty_struct *tty);
static void do_close(struct cbob_uart *uart);

irqreturn_t cbob_uart_handler(int irq, void *data, struct pt_regs *regs);
static void cbob_uart_fetch_data(void *arg);

DECLARE_WORK(cbob_uart_fetch, cbob_uart_fetch_data, 0);

static struct tty_driver *cbob_uart_tty_driver;
static struct tty_operations cbob_uart_ops = {
  open:       cbob_uart_open,
  close:      cbob_uart_close,
  write:      cbob_uart_write,
  write_room: cbob_uart_write_room
};

struct cbob_uart *cbob_uarts[CBOB_UART_MINORS];
struct workqueue_struct *cbob_uart_workqueue;
//static unsigned char cbob_uart_packet[2][CBOB_UART_PACKET_SIZE];

static int cbob_uart_open(struct tty_struct *tty, struct file *filp)
{ 
  struct cbob_uart *uart;
  int index;
  //short statusmask;
  
  tty->driver_data = NULL;
  
  index = tty->index;
  uart = cbob_uarts[index];
  if(uart == NULL) {
  	uart = kmalloc(sizeof(struct cbob_uart), GFP_KERNEL);
  	if(!uart)
  		return -ENOMEM;
  	init_MUTEX(&uart->sem);
  	uart->open_count = 0;
  	
  	cbob_uarts[index] = uart;
  }
  
  if(down_interruptible(&uart->sem))
  	return -EINTR;
  
  tty->driver_data = uart;
  uart->tty = tty;
  
  uart->open_count++;
  
  //if(cbob_uarts[0] && cbob_uarts[0]->open_count) statusmask |= 1;
  //if(cbob_uarts[1] && cbob_uarts[1]->open_count) statusmask |= 2;
  
  //cbob_spi_message(CBOB_CMD_UART_CONFIG, &statusmask, 1, 0, 0);
  
  up(&uart->sem);
  return 0;
}

static void cbob_uart_close(struct tty_struct *tty, struct file *filp)
{
  struct cbob_uart *uart = tty->driver_data;
  
  if(uart)
    do_close(uart);
}

static void do_close(struct cbob_uart *uart)
{
	
  down(&uart->sem);
  
  if(!uart->open_count) {
  	up(&uart->sem);
  	return;
  }
  
  --uart->open_count;
  up(&uart->sem);
}

static ssize_t cbob_uart_write(struct tty_struct *tty, const unsigned char *buf, int count)
{
	struct cbob_uart *uart = tty->driver_data;
	int index = tty->index;
	short data[35], written;
	
	if(!uart)
		return -ENODEV;
	
	if(down_interruptible(&uart->sem))
		return -EINTR;
	
	if(!uart->open_count) {
		up(&uart->sem);
		return -EINVAL;
	}
	
	if(count > 64) count = 64;
	
	memcpy(&(data[2]), buf, count);
	data[0] = index;
	data[1] = count;
	
	cbob_spi_message(CBOB_CMD_UART_WRITE, data, (count>>1)+3, &written, 1);
	
	up(&uart->sem);
	
	return written;
}

static int  cbob_uart_write_room(struct tty_struct *tty)
{
	return 64;
}

irqreturn_t cbob_uart_handler(int irq, void *data, struct pt_regs *regs)
{
	
	queue_work(cbob_uart_workqueue, &cbob_uart_fetch);
	
  return IRQ_HANDLED;
}

void cbob_uart_fetch_data(void *arg) {
	struct tty_struct *tty;
	short request[2];
	short data[33];
	

	request[0] = 0;
	request[1] = 64;	
	if(cbob_spi_message(CBOB_CMD_UART_READ, request, 2, data, 33) < 0) return;
	
	if(data[0] && cbob_uarts[0] && cbob_uarts[0]->open_count) {
		tty = cbob_uarts[0]->tty;
		tty_insert_flip_string(tty, (void*)&(data[1]), data[0]);
		tty_flip_buffer_push(tty);
	}
	
	request[0] = 1;
	if(cbob_spi_message(CBOB_CMD_UART_READ, request, 2, data, 33) < 0) return;
	
	if(data[0] && cbob_uarts[1] && cbob_uarts[1]->open_count) {
		tty = cbob_uarts[1]->tty;
		tty_insert_flip_string(tty, (void*)&(data[1]), data[0]);
		tty_flip_buffer_push(tty);
	}
}

/* init and exit */
int cbob_uart_init(void)
{
  int error, i;
  
  cbob_uart_tty_driver = alloc_tty_driver(CBOB_UART_MINORS);
  if(!cbob_uart_tty_driver)
  	return -ENOMEM;
  
  cbob_uart_tty_driver->owner = THIS_MODULE;
  cbob_uart_tty_driver->driver_name = "cbob_uart";
  cbob_uart_tty_driver->name = "uart";
  cbob_uart_tty_driver->devfs_name = "cbc/uart%d";
  cbob_uart_tty_driver->major = CBOB_UART_MAJOR;
  cbob_uart_tty_driver->type = TTY_DRIVER_TYPE_SERIAL,
  cbob_uart_tty_driver->subtype = SERIAL_TYPE_NORMAL;
  cbob_uart_tty_driver->flags = TTY_DRIVER_REAL_RAW | TTY_DRIVER_NO_DEVFS;
  cbob_uart_tty_driver->init_termios = tty_std_termios;
  cbob_uart_tty_driver->init_termios.c_iflag = IGNBRK | IGNPAR;
  cbob_uart_tty_driver->init_termios.c_oflag = 0;
  cbob_uart_tty_driver->init_termios.c_cflag = B115200 | CS8 | CREAD | CLOCAL;
  cbob_uart_tty_driver->init_termios.c_lflag = 0;
  cbob_uart_tty_driver->init_termios.c_cc[VMIN] = 1;
  cbob_uart_tty_driver->init_termios.c_cc[VTIME] = 0;
  tty_set_operations(cbob_uart_tty_driver, &cbob_uart_ops);
  
  error = tty_register_driver(cbob_uart_tty_driver);
  if(error) {
  	printk(KERN_ERR "failed to register cbob uart tty driver");
  	put_tty_driver(cbob_uart_tty_driver);
  	return error;
  }
  
  for(i = 0;i < CBOB_UART_MINORS; i++) {
  	tty_register_device(cbob_uart_tty_driver, i, NULL);
  	cbob_uarts[i] = NULL;
  }
 
 	cbob_uart_workqueue = create_singlethread_workqueue("CBOB UART");
 
 	imx_gpio_mode(GPIO_PORTD | 27 | GPIO_IN | GPIO_GPIO | GPIO_IRQ_RISING);
  request_irq(IRQ_GPIOD(27), cbob_uart_handler, 0, "CBOB", 0);
  
  return error;
}

void cbob_uart_exit(void)
{
	struct cbob_uart *uart;
	int i;
	
	free_irq(IRQ_GPIOD(27), 0);
	
	for(i = 0;i < CBOB_UART_MINORS;i++)
		tty_unregister_device(cbob_uart_tty_driver, i);
	tty_unregister_driver(cbob_uart_tty_driver);
	
	for(i = 0;i < CBOB_UART_MINORS;i++) {
		uart = cbob_uarts[i];
		if(uart) {
			while(uart->open_count)
				do_close(uart);
			kfree(uart);
			cbob_uarts[i] = NULL;
		}
	}
	
	flush_workqueue(cbob_uart_workqueue);
	destroy_workqueue(cbob_uart_workqueue);
}

