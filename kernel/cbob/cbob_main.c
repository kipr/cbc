#include <linux/config.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>

#include "cbob_spi.h"
#include "cbob_digital.h"
#include "cbob_analog.h"
#include "cbob_sensors.h"

MODULE_AUTHOR("jorge@kipr.org");
MODULE_LICENSE("GPL");

static int cbob_digital_initted;
static int cbob_analog_initted;

static int __init cbob_init(void) 
{
  cbob_spi_init();
  
  if(cbob_digital_init() != 0) {
    cbob_digital_initted = 0;
    printk(KERN_ALERT "Failed to initialize cbob_digitals.\n");
  }
  else cbob_digital_initted = 1;
  
  if(cbob_analog_init() != 0) {
    cbob_analog_initted = 0;
    printk(KERN_ALERT "Failed to initialize cbob_analog.\n");
  }
  else cbob_analog_initted = 1;
  
  cbob_sensors_init();
  
  return (0);
}

static void __exit cbob_exit(void) {
  if(cbob_digital_initted) cbob_digital_exit();
  if(cbob_analog_initted)  cbob_analog_exit();
  cbob_sensors_exit();
}

// entry and exit mappings
module_init(cbob_init);
module_exit(cbob_exit);
