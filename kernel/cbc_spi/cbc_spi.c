/*
       cbc_spi.c
       
       Created from linux-2.6.16-chumby-1.6.0/drivers/char/chumby_accel.c

       This file is part of the chumby accelerometer driver in the linux kernel.
       Copyright (c) Chumby Industries, 2007

       The accelerometer driver is free software; you can redistribute it and/or modify
       it under the terms of the GNU General Public License as published by
       the Free Software Foundation; either version 2 of the License, or
       (at your option) any later version.

       The accelerometer driver is distributed in the hope that it will be useful,
       but WITHOUT ANY WARRANTY; without even the implied warranty of
       MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
       GNU General Public License for more details.

       You should have received a copy of the GNU General Public License
       along with the Chumby; if not, write to the Free Software
       Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include <linux/config.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>

#include <linux/kernel.h>       /* printk() */
#include <linux/slab.h>         /* kmalloc() */
#include <linux/fs.h>           /* everything... */
#include <linux/errno.h>        /* error codes */
#include <linux/types.h>        /* size_t */
#include <linux/proc_fs.h>
#include <linux/fcntl.h>        /* O_ACCMODE */
#include <linux/seq_file.h>
#include <linux/cdev.h>

#include <asm/io.h>
#include <asm/system.h>         /* cli(), *_flags */
#include <asm/uaccess.h>        /* copy_*_user */

#include <linux/miscdevice.h>
#include <linux/ioport.h>
#include <linux/poll.h>
#include <linux/sysctl.h>
#include <linux/spinlock.h>
#include <linux/delay.h>
#include <linux/rtc.h>

#include <linux/timer.h>

#include <asm/arch/imx-regs.h>

/*
 * basic parameters
 */

int cbob_major = 0; // dynamic allocation
int cbob_minor = 0;
int cbob_nr_devs = 1;
struct cdev *cbob_cdev = 0;
int cbob_opened = 0;

MODULE_AUTHOR("jorge@kipr.org");
MODULE_LICENSE("GPL");


#define CLK_DIVIDER  ((PCDR1 & PCDR1_PERDIV2_MASK) >> PCDR1_PERDIV2_POS)
#define BASE_FREQ    350
#define TARGET_FREQ  5

#define SPI_CHAN 0

static int cbob_open(struct inode *inode, struct file *file);
static int cbob_release(struct inode *inode, struct file *file);

static ssize_t cbob_read(struct file *file, char *buf, size_t count, loff_t *ppos);
static ssize_t cbob_write(struct file *file, const unsigned char *buf, size_t count, loff_t *ppos );

// map into the generic driver infrastructure
static struct file_operations cbob_fops = {
	owner:   THIS_MODULE,
	read:    cbob_read,
	write:   cbob_write,
	open:    cbob_open,
	release: cbob_release,
};

///////////// code /////////////
/* Taken from chumby_accel.c by chumby */

static int spi_tx_fifo_empty(void)
{ return (SSP_INT_REG(SPI_CHAN) & SSP_INT_TE);}

static int spi_rx_fifo_data_ready(void)
{ return (SSP_INT_REG(SPI_CHAN) & SSP_INT_RR);}

static unsigned int spi_exchange_data(unsigned int dataTx) {
  while(!spi_tx_fifo_empty());

  SSP_TX_REG(SPI_CHAN)   = dataTx;	     // transfer data
  SSP_CTRL_REG(SPI_CHAN) |= SSP_XCH;         // exchange data
    
  while(!spi_rx_fifo_data_ready());

  return SSP_RX_REG(SPI_CHAN);
}

static int cbob_release(struct inode *inode, struct file *file) {
  return 0;
}

static int cbob_open(struct inode *inode, struct file *file) {
  // make sure we're not opened twice
  return(0);
}

/*static int cbob_read_block(char *buf, size_t len)
{
  
}

static int cbob_write_block(char *buf, size_t len)
{

}*/

static int __init cbob_init(void) {
  dev_t dev = 0;
  int result, err;

  cbob_cdev = cdev_alloc();

  result = alloc_chrdev_region(&dev, cbob_minor, cbob_nr_devs, "cbob");
  cbob_major = MAJOR(dev);

  printk("Initting CBOB SPI Interface...\n");

  cdev_init(cbob_cdev, &cbob_fops);
  cbob_cdev->owner = THIS_MODULE;
  cbob_cdev->ops = &cbob_fops;
  err = cdev_add (cbob_cdev, dev, 1);
  
  printk("Got CBOB Major=%d\n", cbob_major);

  // hardware init
  // map GPIO ports appropriately
  // CSPI1_SS1    F16    CSPI1_SS1 (GPIO   )     PD27  out 1  ** not controlled by CSPI 
  // CSPI1_SS0    F19    CSPI1_SS0 (GPIO   )     PD28  out 1  ** not controlled by CSPI
  // CSPI1_SCLK   H10    CSPI1_SCLK(primary)     PD29  out 1
  // CSPI1_MISO   F17    CSPI1_MISO(primary)     PD30  in  0
  // CSPI1_MOSI   J12    CSPI1_MOSI(primary)     PD31  out 1
  // ** note that SS0,1 is decoded on the sensor card to 4 individual chipselects
  //    so these values have to be manually controlled by the software
  
  //imx_gpio_mode( GPIO_PORTD | 27 | GPIO_OUT | GPIO_PF );
  imx_gpio_mode( GPIO_PORTD | 28 | GPIO_OUT | GPIO_PF );

  imx_gpio_mode( GPIO_PORTD | 29 | GPIO_OUT | GPIO_PF );
  imx_gpio_mode( GPIO_PORTD | 30 | GPIO_IN  | GPIO_PF );
  imx_gpio_mode( GPIO_PORTD | 31 | GPIO_OUT | GPIO_PF );

  // turn on the clock
  PCCR0 |= PCCR0_CSPI1_EN;

  // setup the SPI interfaces
  // reset the interface
  SSP_RESET_REG(SPI_CHAN) = 0x00000001;
  udelay(200);      //wait

  // use 32.768kHz clock speed, with 0 clock insertion
  SSP_PER_REG(SPI_CHAN) = 0x00008000;

  // init the hardware config register
  // 31-24 00000000
  //    23 0     BURST off
  //    22 0     SDHC off
  //    21 0     SWAP off
  // 20-19 00    SS asserted
  // 18-14 00000 ((BASE_FREQ/CLK_DIVIDER) / (TARGET_FREQ) >> 1)
  // 13-12 00    DRCTL ignore ready
  //    11 1     MODE = master
  //    10 1*    SPIEN enable interface (toggle to reset Tx FIFO)
  //     9 0     XCH  exchange off for now
  //     8 0     SSPOL is active low
  //     7 1     SSCTL CS toggles between bursts
  //     6 1     PHA phase 1 operation (falling edge)
  //     5 1     POL (active 1 idle)
  //   4-0 01111 BITCOUNT 16-bit transfer for Kionix
  SSP_CTRL_REG(SPI_CHAN) = 0;
  SSP_CTRL_REG(SPI_CHAN) |= (SSP_MODE_MASTER | SSP_ENABLE | SSP_SS_PULSE | SSP_PHA1 | SSP_POL1 | SSP_WS(8) );
  SSP_CTRL_REG(SPI_CHAN) |= (((BASE_FREQ/CLK_DIVIDER) / (TARGET_FREQ) >> 1) + 1) << 14;
  SSP_CTRL_REG(SPI_CHAN) &= ~SSP_MODE_MASTER;  // reset fifo
  // printk( "accel clock init: base freq %d, clk_divider %d, target_freq %d, calc div %d\n", BASE_FREQ, CLK_DIVIDER, TARGET_FREQ, ((BASE_FREQ/CLK_DIVIDER) / (TARGET_FREQ) >> 1) + 1 );

  udelay(100);      //wait
  SSP_CTRL_REG(SPI_CHAN) &= 0xFFFFFFE0;
  SSP_CTRL_REG(SPI_CHAN) |= SSP_WS(8);
  SSP_CTRL_REG(SPI_CHAN) |= SSP_MODE_MASTER;  // reset fifo

  return (0);
}

static ssize_t cbob_read(struct file *file, char *buf, size_t count, loff_t *ppos) 
{
  char *data;
  int i;
  
  if((data = kmalloc(count, GFP_KERNEL))==0)
    return -ENOMEM;
  
  for(i = 0;i < count;i++)
    data[i] = spi_exchange_data(0);
    
  copy_to_user(buf, data, count);
  
  return count;
}

static ssize_t cbob_write(struct file *file, const unsigned char *buf, size_t count, loff_t *ppos ) 
{
  char *data;
  int i;
  
  if((data = kmalloc(count, GFP_KERNEL)) == 0)
    return -ENOMEM;
   
  copy_from_user(data, buf, count);
  
  for(i = 0;i < count;i++)
    spi_exchange_data(data[i]);
    
  return count;
}


static void __exit cbob_exit(void) {
  dev_t devno = MKDEV(cbob_major, cbob_minor);
  cdev_del( cbob_cdev );
  unregister_chrdev_region(devno, cbob_nr_devs);  
}

// entry and exit mappings
module_init(cbob_init);
module_exit(cbob_exit);
