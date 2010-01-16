#include "bob.h"
#include <sensors/sensors.h>
#include <usb/usb.h>
#include <timer/timer.h>
#include <motors/motors.h>
#include <servos/servos.h>
#include <accel/accel.h>
#include <uart/uart.h>

#include <utility/trace.h>
#include <pio/pio_it.h>

void BobInit()
{
	TRACE_CONFIGURE(DBGU_STANDARD, 115200, MCK);
	PIO_InitializeInterrupts(0);
	//UsbInit();
	UartInit();
	PitInit(MC_PID_PERIOD);
	SensorsInit();
	MotorsInit();
	ServosInit();
	AccelInit();
}

void BobTest()
{
	int i,j;
	int status = 1;
	int testvals[8];
	
	TRACE_CONFIGURE(DBGU_STANDARD, 115200, MCK);
	PIO_InitializeInterrupts(0);
	//UsbInit();
	UartInit();
	printf("\n\n\n\n\n\n\r");
	printf("Starting BoB test\n\r");
	
	printf("Periodic Interval Timer");
	PitInit(MC_PID_PERIOD);
	usleep(50000);
	printf("\t\t- PASSED\n\r");
	
	////////////////////////////////////////////////////////////////////////////////
	// check Digital input
	////////////////////////////////////////////////////////////////////////////////
	SensorsTestInit();
	printf("Press BLACK button ");
	while(!BlackButton()){};
	printf("\t\t- PASSED\n\r");
	
	// set analog ports to 0
	for(i=0;i<8;i++) ADigitalOut(i,0);
	// read in digital values and compare to 0 print out bad port
	for(i=0;i<8;i++){
		if(Digital(i) != 0){
			printf("Digital LOW %d is HIGH\n\r",i);
			status = 0;
		}
	}
	// set analogs to 1
	for(i=0;i<8;i++) ADigitalOut(i,1);
	// read in digital values and compare to 1 print out bad port
	for(i=0;i<8;i++){
		if(Digital(i) != 1){
			printf("Digital HIGH %d is LOW\n\r",i);
			status = 0;
		}
	}
	if(status) printf("Digital Inputs\t\t\t- PASSED\n\r");
	else {
		printf("\nDigital Inputs - FAILED\n\r");
		printf("Is LOOPBACK connector plugged in correctly?\n\r");
		printf("Check for short circuit on U1 left side\n\r");
		printf("Check U5\n\r");
		while(1){};
	}
	
	////////////////////////////////////////////////////////////////////////////////
	// check Digital output and Analog input
	////////////////////////////////////////////////////////////////////////////////
	// reinit sensors
	SensorsInit();
	printf("Sensors Configured\t\t- PASSED\n\r");
	// PLUG IN LOOPBACK CONNECTOR ///////////////////////////////////////////////////////////////////////
	// check Analog ports -------------------------------------------------------------------------------
	for(i=0;i<8;i++){
		// configure all analogs with pullups disabled
		AnalogPullup(i,0);
		// configure all digitals to outputs
		DigitalConfig(i,1);
		// set all digital outs to 0
		DigitalOut(i,0);
	}
	// wait for configuration to take place
	usleep(30000);
	// read in analog values and compare to 0 print out bad port
	for(i=0;i<8;i++){
		if(Analog(i) > 0){
			printf("Analog LOW %d: %d\n\r",i,Analog(i));
			status = 0;
		}
	}
	// set all digital outs to 1
	for(i=0;i<8;i++) DigitalOut(i,1);
	// wait for configuration to take place
	usleep(30000);
	// read in analog values and compare to 4096 print out bad port
	for(i=0;i<8;i++){
		if(Analog(i) < 1022){
			printf("Analog HIGH %d: %d\n\r",i,Analog(i));
			status = 0;
		}
	}
	if(status) printf("Analog Inputs & Digital Outputs\t- PASSED\n\r");
	else {
		printf("\nAnalog Inputs - FAILED\n\r");
		printf("Is LOOPBACK connector plugged in correctly?\n\r");
		printf("if LOW values near 4 check chip U5\n\n\r");
		while(1){};
	}
	
	// check Battery voltage switch ---------------------------------------------------------------------
	printf("Voltage %d ", BattVoltage());
	if(BattVoltage() > 6000) printf("Battery Switch\t- PASSED\n\r");
	else {
		printf("Battery Switch - FAILED\n\r");
		printf("Is battery more than 6 volts charged?\n\r");
		printf("if Voltage is near zero check U6");
		while(1){};
	}
	
	// check VCC power rail -----------------------------------------------------------------------------
	printf("\nNote GREEN Light on LOOPBACK then press BLACK button\n\r");
	usleep(2000000);
	while (!BlackButton()){};
	printf("Check U8 if not blinking\n\n\r");
	for(i=0;i<5;i++){
		if(i%2) SensorPowerOff();
		else SensorPowerOn();
		usleep(1000000);
	}
	
	
	// UNPLUG LOOPBACK CONNECTOR ///////////////////////////////////////////////////////////////////////
	printf("Unplug LOOPBACK connector then press BLACK button\n\n\r");
	// check analog pullups ----------------------------------------------------------------------------
	while(!BlackButton()){};
	
	// read in analog values without pullup disabled
	for(i=0;i<8;i++){
		testvals[i] = Analog(i);
	}
	// configure all analogs with pullups enabled
	for(i=0;i<8;i++) AnalogPullup(i,1);
	usleep(300000);
	
	// read in analog values and compare to pullups disabled
	for(i=0;i<8;i++){
		// if the change is less than threshold the pullups are not working
		if((Analog(i) - testvals[i]) < 5){
			printf("Analog PULLUP %d: %d\n\r",i,Analog(i));
			status = 0;
		}
	}
	
	if(status) printf("Analog Pullups\t\t\t- PASSED\n\r");
	else {
		printf("Analog Inputs - FAILED\n\r");
		printf("if PULLUP values are lower than 1020 check chip U5");
		while(1){};
	}
	
	
	//////////////////////////////////////////////////////////////////////////////
	// check Accelerometer
	//////////////////////////////////////////////////////////////////////////////
	// set bob on flat surface
	printf("Initalizing Accelerometer\n\r");
	AccelInit();
	for(i=0;i<5;i++){
		AccelCalibration();
		if(!status) {
			status = 1;
			printf("\t\t\tre-Calibrating\n\r");
		}
		// read axis data and check that X and Y are near zero and Z is down
		for(j=0;j<10;j++){
			usleep(100000);
			if(Accel_X() > 5 || Accel_X() < -5){
				status = 0;
				printf("\r X axis:%d",Accel_X());
			}
			if(Accel_Y() > 5 || Accel_Y() < -5){
				status = 0;
				printf("\r Y axis:%d",Accel_Y());
			}
			if(Accel_Z() > 70 || Accel_Z() < 60){
				status = 0;
				printf("\r Z axis:%d",Accel_Z());
			}
		}
		if(status) break; // passed calibration
	}
	if(status) printf("Accelerometer Calibration\t- PASSED\n\r");
	else {
		printf("Accelerometer Calibration\t- FAILED\n\r");
		printf("Is the board LEVEL?\n\r");
		printf("check U7 or R91\n\r");
		while(1){};
	}
	
	i=j=1;
	
	printf("\nPlug in SERVOS then press BLACK button\r\n\n");
	
	ServosInit();
	MotorsInit();
	
	/* once all other test have been run */
	while(1){
		////////////////////////////////////////////////////////////////////////////
		// check Servos
		////////////////////////////////////////////////////////////////////////////
		
		if(BlackButton()){
			
			EnableServos();
			printf("\n\rServos Enabled\n\r");
			j=5;
			i=j;
			printf("Right  ");
			while(i > 0){
				SetServoPosition(0,i);
				SetServoPosition(1,i);
				SetServoPosition(2,i);
				SetServoPosition(3,i);
				i += j;
				if(i>2048){
					j=-j;
					printf("Left  ");
				}
				usleep(10000);
			}
			// servos recenter print servos centering
			printf("Center \n\r");
			SetServoPosition(0,1024);
			SetServoPosition(1,1024);
			SetServoPosition(2,1024);
			SetServoPosition(3,1024);
			usleep(1000000);
			DisableServos();
			printf("Servos Disabled\n\r");
			j=1;
			i=j;
		}
		
		////////////////////////////////////////////////////////////////////////////
		// check Motors
		////////////////////////////////////////////////////////////////////////////
		
		Motor(0,i);
		Motor(1,i);
		Motor(2,i);
		Motor(3,i);
		if(i>0) printf("BLUE\r");
		else if(i<0) printf("RED  \r");
		if(i==99 || i== -99)j=-j;
		i+=j;
		usleep(10000);
	}
	
}
