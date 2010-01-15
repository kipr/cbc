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
	
	USBD_Connect();
 	UartStartRead();
	
	//MoveAtVelocity(0,100);
	while(1){
		/*while(!BlackButton());
		ChumbySS1(1);
		while(BlackButton());
		ChumbySS1(0);*/
		
		while(USBD_GetState() == USBD_STATE_CONFIGURED) nop();
		while(USBD_GetState() != USBD_STATE_CONFIGURED) nop();
		UartStartRead();
		
	// testing functions
	/*
		if(BlackButton()){
			ClearMotorCounter(0);
			//MoveToPosition(0,100,1000);
		}
		//if(IsMotorDone(0)) MoveAtVelocity(0,0);
		printf("P%d\tTPC%d\tdone-%d %d\r\n",GetMotorCounter(0), GetMotorTPC(0),IsMotorDone(0),GetMotorControlState(0));
		//printf("%d D0-%d\t D1-%d \t D6-%d\t D7-%d\r\n",i,Digital(0),Digital(1),Digital(6),Digital(7));
		usleep(100000);
	*/	
	}
	
	return 1;
}

