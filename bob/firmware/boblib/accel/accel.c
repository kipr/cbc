#include "accel.h"

#include <bob.h>
#include <pio/pio.h>
#include <pmc/pmc.h>
#include <twi/twi.h>
#include <timer/timer.h>

#include <string.h>
#include <stdio.h>

//#define STACCEL 1   // uncomment if ST 16-bit accel is selected

// Accelerometer TWI address
#ifdef STACCEL
	#define ACCELADDR	0X18	// ST ACCEL
#else
	#define ACCELADDR	0X1D	//Freescale ACCEL
#endif

#define TWITIMEOUTMAX 50000
// Pin Setup Arrays
const Pin twiPins[] = {PIN_TWID,PIN_TWICK};
const Pin accIntPins[] = {PIN_ACCINT1};

char g_idle = 0;
int g_AccelX = 0;
int g_AccelY = 0;
int g_AccelZ = 0;
short g_calValues[3] = {-2,19,-23};

static void AccelCallback(void);

unsigned char AccelInit(void)
{	
	PIO_Configure(accIntPins, PIO_LISTSIZE(accIntPins));
	
	if(PMC_IsPeriphEnabled(AT91C_ID_TWI) == 0){
		PIO_Configure(twiPins, PIO_LISTSIZE(twiPins));
		
		PMC_EnablePeripheral(AT91C_ID_TWI);
	
		TWI_ConfigureMaster(AT91C_BASE_TWI,TWCK,MCK);
	}
	usleep(10000);

#ifdef STACCEL
	// ST Accelerometer
	char setup[4];
	setup[0] = (1<<5)|(2<<3)|7; // Normal power, data rate = 400Hz, ZYX axis enabled
	setup[1] = 0; // Do not add filter stuff
	setup[2] = (2<<0); // Data ready interrupt on INT 2 pad
	setup[3] = (0<<4); // do update between reads, and set scale to +/- 2g's
	WriteAccelData(0x20|(1<<7),setup,4);
#else
	// Freescale Accelerometer
	//AccelCalibration();	
	AccelSetScale(2);	
	AccelSetCalibration();
	
#endif
	SetPitCallback(AccelCallback,3);
	
	return 0;
}

void AccelCalibration(void)
{
#ifdef STACCEL	
	return;
#else
	char value[9];
	short avgOffset[3];
	int i,j;
	
	g_idle = 1;	// put the callback funciton on idle so it doesn't interrupt
	// set the Accelerometer to measurement mode
	value[0] = 5;
	WriteAccelData(0x16, value, 1);
	
	// clear out the old calibration values first!!!!!!!!
	g_calValues[0] = 0;
	g_calValues[1] = 0;
	g_calValues[2] = 0;
	value[0] = value[1] = value[2] = value[3] = value[4] = value[5] = 0;
	WriteAccelData(0x10, value, 6);
	usleep(200);
	
	for(j=0;j<10;j++){
		// get a set of readings from the Accelerometer
		while(!PIO_Get(&(accIntPins[0]))){nop();} // wait for data ready pin
		ReadAccelData(0x0,value,9);			// read in 9 bytes for both 10bit and 8bit
	
		for(i=0;i<3;i++){
			avgOffset[i] = value[i*2+1]<<8;
			avgOffset[i] |= value[i*2];
			if(avgOffset[i] > 512) avgOffset[i] -=1024;
			
			g_calValues[i] += avgOffset[i];	// sum up values
		}
		usleep(100000);
	}

	// take the average
	g_calValues[0] /= 10;
	g_calValues[1] /= 10;
	g_calValues[2] /= 10;
	
	// multiply the offsets by 2, stated in the datasheet unsure why has to do with 1/2 LSB
	g_calValues[0] = -g_calValues[0]*2;
	g_calValues[1] = -g_calValues[1]*2;
	g_calValues[2] = (64 - g_calValues[2])*2;
	
	// write data to offset register	
	AccelSetCalibration();
	g_idle = 0;	// turn back on the callback
#endif
}

void GetAccelCal(short *calVal)
{
	calVal[0] = g_calValues[0];
	calVal[1] = g_calValues[1];
	calVal[2] = g_calValues[2];
}

void SetAccelCal(short *calVal)
{
	g_calValues[0] = calVal[0];
	g_calValues[1] = calVal[1];
	g_calValues[2] = calVal[2];
}

void AccelSetCalibration(void)
{	
	WriteAccelData(0x10, (char*)g_calValues, 6);
}

int Accel_X(void)
{
	return g_AccelX;
}

int Accel_Y(void)
{
	return g_AccelY;
}

int Accel_Z(void)
{
	return g_AccelZ;
}


static void AccelCallback(void)
{
#ifdef STACCEL
	char value[6];
	while(!PIO_Get(&(accIntPins[0]))){nop();} // wait for data ready pin
	ReadAccelData(0x28|(1<<7),value,6); // ST chip
	g_AccelX = ((signed char)value[1]<<8)|value[0];
	g_AccelY = ((signed char)value[3]<<8)|value[2];
	g_AccelZ = ((signed char)value[5]<<8)|value[4];
#else
	char value[3];
	if(g_idle) return;
	while(!PIO_Get(&(accIntPins[0]))){nop();} // wait for data ready pin
	ReadAccelData(0x6,value,3);
	g_AccelX = (signed char)value[0];
	g_AccelY = (signed char)value[1];
	g_AccelZ = (signed char)value[2];
#endif
}

#ifdef STACCEL
int Accel_X_ST(void)
{
	char value[2];
	
	while(!PIO_Get(&(accIntPins[0]))){nop();} // wait for data ready pin
	ReadAccelData(0x28|(1<<7), value, 2); // ST chip
	return ((signed char)value[1]<<8)|value[0];
}

int Accel_Y_ST(void)
{
	char value[2];
	
	while(!PIO_Get(&(accIntPins[0]))){nop();} // wait for data ready pin
	ReadAccelData(0x2A|(1<<7), value, 2); // ST chip
	return ((signed char)value[1]<<8)|value[0];
}

int Accel_Z_ST(void)
{
	char value[2];
	
	while(!PIO_Get(&(accIntPins[0]))){nop();} // wait for data ready pin
	ReadAccelData(0x2C|(1<<7), value, 2); // ST chip
	return ((signed char)value[1]<<8)|value[0];
}
#endif


#ifdef STACCEL
// sets the g-scale of the ST chip 2,4,8 g measurment
unsigned char AccelSetScale(int gsel)
{
	char scale;
	
	switch (gsel) {
		case 2:
			scale = 0<<4;
			break;
		case 4:
			scale = 1<<4;
			break;
		case 8:
			scale = 3<<4;
			break;
		default:
			scale = 0<<4;
			break;
	}
	scale |= (1<<7); //no updates between reading
	return WriteAccelData(0x23, &scale, 1);
}
#else
// sets the g-scale of the Freescale chip 2,4,8 g measurment
unsigned char AccelSetScale(int gsel)
{
	char scale;
	
	switch (gsel) {
		case 2:
			scale = 1<<2;
			break;
		case 4:
			scale = 2<<2;
			break;
		case 8:
			scale = 0<<2;
			break;
		default:
			scale = 2<<2;
			break;
	}
	scale |= 1; // set scale and turn on measurement mode
	return WriteAccelData(0x16, &scale, 1);
}
#endif

unsigned char WriteAccelData(unsigned int iaddress, char *bytes, unsigned int num)
{
	unsigned int timeout;
	
	// wait for TWI bus to be ready
	while(!(TWI_TransferComplete(AT91C_BASE_TWI))) nop();
	
	// Start Writing
	TWI_StartWrite(AT91C_BASE_TWI,ACCELADDR,iaddress,1,*bytes++);
	num--;
	
	while(num > 0){
		// Wait before sending the next byte
		timeout = 0;
		while(!TWI_ByteSent(AT91C_BASE_TWI) && (++timeout<TWITIMEOUTMAX)) nop();
		if(timeout == TWITIMEOUTMAX) return 1;
		TWI_WriteByte(AT91C_BASE_TWI, *bytes++);
		num--;
	}
	
	return 0;
}

unsigned char ReadAccelData(unsigned int iaddress, char *bytes, unsigned int num)
{
	unsigned int timeout;
	
	// wait for TWI bus to be ready
	while(!(TWI_TransferComplete(AT91C_BASE_TWI))) nop();
	
	// Start Reading
	TWI_StartRead(AT91C_BASE_TWI, ACCELADDR,iaddress,1);
	
	while (num > 0) {
		// Last byte
		if(num == 1) TWI_Stop(AT91C_BASE_TWI);
		
		// wait for byte then read and store it
		timeout = 0;
		while(!TWI_ByteReceived(AT91C_BASE_TWI) && (++timeout<TWITIMEOUTMAX)) nop();
		if(timeout == TWITIMEOUTMAX) return 2;
		*bytes++ = TWI_ReadByte(AT91C_BASE_TWI);
		num--;
	}
	
	return 0;
}
