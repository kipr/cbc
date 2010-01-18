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

int main()
{
	//int i=0;
	//int max = 0,min = 0;
	//int val;
	DigitalInit();
	if(BlackButton()) Bootloader();
	
	BobInit();
	ChumbyInit();
	
	printf("Boot!!!!\n\r");
	
	ChumbyRun();
	
	return 1;
}

