#include "servos.h"

#include <bob.h>
#include <pio/pio.h>
#include <pmc/pmc.h>
#include <pwmc/pwmc.h>

const Pin servos[] = {SERVOS};

volatile int g_ServoPositions[4] = {-1,-1,-1,-1};	// holds the servo positions

void ServosInit(void)
{
	PIO_Configure(servos, PIO_LISTSIZE(servos));
	
	// enable the PWM peripheral if it has not already been
	if(!PMC_IsPeriphEnabled(AT91C_ID_PWMC)) {
		PMC_EnablePeripheral(AT91C_ID_PWMC);
		PWMC_ConfigureClocks(1000000, 0, MCK);
	}
}

void EnableServoPort(int port)
{
	// set clock to MCK/64 or MCK/(2^6), only works up to 1024
	g_ServoPositions[port] = 1024;
	PWMC_ConfigureChannel(port, 6, 0, 0);
	PWMC_SetPeriod(port, SC_PWM_PERIOD);
	SetServoPosition(port,g_ServoPositions[port]);
	PWMC_EnableChannel(port);
}

void EnableServos(void)
{
	int i;
	for(i=0;i<4;i++) EnableServoPort(i);
}

void DisableServoPort(int port)
{
	PWMC_DisableChannel(port);
	g_ServoPositions[port] = -1;
}

void DisableServos(void)
{
	int i;
	for(i=0;i<4;i++) DisableServoPort(i);
}

// position = 0-2048
void SetServoPosition(int port, int position)
{
	int pulseCount = 0;
	if(port < 0 || port > 3) return;
	
	if(position < 0) {
		DisableServoPort(port);
		return;
	}
	if(position > 2048) position = 2048;
	
	if(g_ServoPositions[port] < 0) EnableServoPort(port);
	
	g_ServoPositions[port] = position;
	
	// PWM clock is running at 48MHz/64 ~ 1.33microSeconds/tick
	// Servos pulses range from 640 - 2540 microSeconds
	// Position inputs are 11bits - 0-2048
	// therfore 640/1.33 = 480 ticks 
	pulseCount = (7*position)/10+480;
	pulseCount = SC_PWM_PERIOD - pulseCount;
	PWMC_SetDutyCycle(port, pulseCount);
}

int GetServoPosition(int port)
{
	return g_ServoPositions[port];
}


