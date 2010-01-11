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

#include <usb/device/cdc-serial/CDCDSerialDriver.h>
#include <usb/device/cdc-serial/CDCDSerialDriverDescriptors.h>

int main()
{
	//int i=0;
	//int max = 0,min = 0;
	//int val;
	DigitalInit();
	if(BlackButton()) Bootloader();
	
	BobInit();
	ChumbyInit();
	
	//while(!BlackButton()) nop();
	//usleep(100000);
	
	printf("Boot!!!!\n\r");
	
	ChumbyBend(0);
	ChumbySS1(0);
	
/*	while(1) {
		while(BlackButton()) nop();
		ChumbySS1(0);
		while(!BlackButton()) nop();
		ChumbySS1(1);
	}*/
	
	while(1){
		while(USBD_GetState() == USBD_STATE_CONFIGURED) nop();
		while(USBD_GetState() != USBD_STATE_CONFIGURED) nop();
		UartStartRead();
		/*
		if(BlackButton()){
			//max = min = 0;
			if(i) i=0;
			else i=1;
			DigitalPullup(0,i);
			DigitalPullup(1,i);
			usleep(20000);
		}
		//val = Accel_Y();
		//if(max< val) max = val;
		//if(min> val) min = val;
		//printf("X %d\tY %d\r\n",Accel_X(),Accel_Y());//,max,min);
		//printf("%d D0-%d\t D1-%d \t D6-%d\t D7-%d\r\n",i,Digital(0),Digital(1),Digital(6),Digital(7));
		usleep(100000);
		*/
	}
	
	return 1;
}

