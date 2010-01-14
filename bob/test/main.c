#include <bob.h>
#include <stdio.h>
#include <usb/usb.h>
#include <sensors/sensors.h>
#include <timer/timer.h>
#include <bootloader/bootloader.h>
#include <motors/motors.h>		
#include <servos/servos.h>
#include <accel/accel.h>
#include <chumby/chumby.h>
#include <uart/uart.h>
#include <usart/usart.h>
#include <pio/pio.h>
#include <pmc/pmc.h>

#include <usb/device/cdc-serial/CDCDSerialDriver.h>
#include <usb/device/cdc-serial/CDCDSerialDriverDescriptors.h>

int main()
{
	//int i=0;
	//int max = 0,min = 0;
	//int val;
	DigitalInit();
	if(BlackButton()) Bootloader();
	
	BobTest();
	
	return 1;
}

