#include "sensors.h"

#include <bob.h>
#include <pio/pio.h>
#include <adc/adc.h>
#include <pmc/pmc.h>
#include <twi/twi.h>
#include <timer/timer.h>

#include <stdio.h>

#define PULLUPADDR 0X27
#define TWITIMEOUTMAX 50000

// Pin Setup Arrays
const Pin aDigitalOuts[] = {ADIGITAL_OUTPUTS};
const Pin analogs[] = {ANALOG_SENSORS};
const Pin digitalIns[] = {DIGITAL_INPUTS};
const Pin digitalOuts[] = {DIGITAL_OUTPUTS};
const Pin sensorsPower[] = {SENS_VEN,SENS_VSTAT,BATTV_SELECT,BLACK_BUTTON,CHARGING};
const Pin twiPortPins[] = {PIN_TWID,PIN_TWICK};

char g_AnalogPullups = 0;
short g_AnalogPullupMask = 0;
char g_DigitalPullups = 0;
volatile int g_AnalogReading[8];
volatile int g_BatteryCounter = 0;
volatile int g_BatteryVoltage[40];
volatile int g_BatteryVoltageIndex = -1;
volatile int g_BatteryAverage;
static void SensorsCallback(void);

void CalculateBatteryPower(int voltage)
{
	int i, sum;
	
	voltage = (9990*voltage)/1023;
	
	if(g_BatteryVoltageIndex == -1) {
		for(i = 0;i < 40;i++)
			g_BatteryVoltage[i] = voltage;
		g_BatteryVoltageIndex = 0;
	}
	else {
		g_BatteryVoltage[g_BatteryVoltageIndex++] = voltage;
		if(g_BatteryVoltageIndex >= 40) g_BatteryVoltageIndex = 0;
	}
	
	for(i = 0;i < 40;i++)
		sum += g_BatteryVoltage[i];
	
	g_BatteryAverage = sum/40;
}

void SensorsTestInit()
{
	char pinDirection = 0;
	int i;
	PIO_Configure(aDigitalOuts, PIO_LISTSIZE(aDigitalOuts));
	PIO_Configure(digitalIns, PIO_LISTSIZE(digitalIns));
	PIO_Configure(sensorsPower, PIO_LISTSIZE(sensorsPower));
	
	PMC_EnablePeripheral(AT91C_ID_PIOB);
	PMC_EnablePeripheral(AT91C_ID_PIOA);
	
	SelectAng0();		// select analog port 0 to be mesured instead of BattVoltage
	SensorPowerOn();	// turn on the Vcc line
	
	if(PMC_IsPeriphEnabled(AT91C_ID_TWI) == 0){
		PIO_Configure(twiPortPins, PIO_LISTSIZE(twiPortPins));
		PMC_EnablePeripheral(AT91C_ID_TWI);
		
		TWI_ConfigureMaster(AT91C_BASE_TWI,TWCK,MCK);
	}
	
	// set all pins on pullup chip to output
	WritePullupData(0x00,&pinDirection,1);
	WritePullupData(0x01,&pinDirection,1);
	
	for(i = 0;i < 8;i++){
		if(i>1) AnalogPullup(i,0);  // disable Analog pullups for output test
		DigitalPullup(i,1);		// enable all digital pullups
	}
}

void SensorsInit()
{
	char pinDirection = 0;
	int i;
	PIO_Configure(analogs, PIO_LISTSIZE(analogs));
	PIO_Configure(digitalIns, PIO_LISTSIZE(digitalIns));
	PIO_Configure(sensorsPower, PIO_LISTSIZE(sensorsPower));
	
	PMC_EnablePeripheral(AT91C_ID_PIOB);
	PMC_EnablePeripheral(AT91C_ID_PIOA);
	
	PMC_EnablePeripheral(AT91C_ID_ADC1);
	ADC_Initialize(AT91C_BASE_ADC1, AT91C_ID_ADC1, AT91C_ADC_TRGEN_DIS, \
				   0, AT91C_ADC_SLEEP_NORMAL_MODE, AT91C_ADC_LOWRES_10_BIT, \
				   MCK, BOARD_ADC_FREQ, 10, 3000);
	for(i = 0;i < 8;i++) ADC_EnableChannel(AT91C_BASE_ADC1, i);

	SetPitCallback(SensorsCallback,1);
	
	SelectAng0();		// select analog port 0 to be mesured instead of BattVoltage
	SensorPowerOn();	// turn on the Vcc line
	
	if(PMC_IsPeriphEnabled(AT91C_ID_TWI) == 0){
		PIO_Configure(twiPortPins, PIO_LISTSIZE(twiPortPins));
		PMC_EnablePeripheral(AT91C_ID_TWI);
		
		TWI_ConfigureMaster(AT91C_BASE_TWI,TWCK,MCK);
	}
	
	// set all pins on pullup chip to output
	WritePullupData(0x00,&pinDirection,1);
	WritePullupData(0x01,&pinDirection,1);
	
	for(i = 0;i < 8;i++){
		if(i>1) AnalogPullup(i,1);  // enable pullups if needed
		DigitalPullup(i,1);		// enable all digital pullups
	}
}

static void SensorsCallback(void)
{
	UpdateAnalogs();
}

void DigitalInit()
{
	PIO_Configure(digitalIns, PIO_LISTSIZE(digitalIns));
}

int BlackButton()
{
  return !PIO_Get(&(sensorsPower[3]));
}

// state = 1 pullup enabled
// state = 0 pullup disabled
void DigitalPullup(int port, int state)
{
	char temp;
	int shifter = 0;
	
	switch (port) {
		case 0:
			shifter = 4;
			break;
		case 1:
			shifter = 3;
			break;
		case 2:
			shifter = 5;
			break;
		case 3:
			shifter = 2;
			break;
		case 4:
			shifter = 6;
			break;
		case 5:
			shifter = 1;
			break;
		case 6:
			shifter = 7;
			break;
		case 7:
			shifter = 0;
			break;
		default:
			return;
	}
	
	if(state){
		// set the pin to be pulled up
		g_DigitalPullups |= (1 << shifter);
	}
	else{
		// set the pin to be floating
		g_DigitalPullups &= (~(1 << shifter));
	}
	
	temp = ~g_DigitalPullups;
	// Write the data out to the chip
	WritePullupData(0x15,&temp,1);
}

int Digital(int port)
{
	if(port < 0 || port > 8)
	  return 0;
	
	return !PIO_Get(&(digitalIns[port]));
}

int AllDigitals()
{
  int i = 0;
  int value = 0;
  
  for(i = 0;i < 8;i++) value |= Digital(i)<<i;
  
  return value;
}

// state = 1 pin is HIGH
// state = 0 pin is LOW
void DigitalOut(int port, int state)
{	
	if(port < 0 || port > 8) 
		return;
	
	if(state)
		PIO_Set(&(digitalOuts[port]));
	else
		PIO_Clear(&(digitalOuts[port]));
}

// state = 1 pin is HIGH
// state = 0 pin is LOW
void ADigitalOut(int port, int state)
{	
	if(port < 0 || port > 8) 
		return;
	
	if(state)
		PIO_Clear(&(aDigitalOuts[port]));
	else
		PIO_Set(&(aDigitalOuts[port]));
}

// dir = 0 is input
// dir = 1 is output
void DigitalConfig(int port, int dir)
{
	if(port < 0 || port > 8) 
		return;
	
	if(dir){
		PIO_Configure(&(digitalOuts[port]),1);
		DigitalPullup(port,0); // disable pullup
	}
	else{
		PIO_Configure(&(digitalIns[port]),1);
		DigitalPullup(port,1); // enable pullup
	}
}

// returns 1 if the port is an output
// returns 0 if the port is an input
// returns -1 if port is out of range
int GetDigitalConfig(int port)
{
	if(port < 0 || port > 8) 
		return -1;
	
	if(PIO_IsOutputPin(&(digitalIns[port]))) return 1;
	else return 0;
}

void UpdateAnalogs(void)
{	
	ADC_StartConversion(AT91C_BASE_ADC1);
	while((ADC_GetStatus(AT91C_BASE_ADC1) & 0x80) != 0x80) nop();
	//g_AnalogReading[0] = ADC_GetConvertedData(AT91C_BASE_ADC1,0);
	g_AnalogReading[1] = ADC_GetConvertedData(AT91C_BASE_ADC1,1);
	g_AnalogReading[2] = ADC_GetConvertedData(AT91C_BASE_ADC1,2);
	g_AnalogReading[3] = ADC_GetConvertedData(AT91C_BASE_ADC1,3);
	g_AnalogReading[4] = ADC_GetConvertedData(AT91C_BASE_ADC1,4);
	g_AnalogReading[5] = ADC_GetConvertedData(AT91C_BASE_ADC1,5);
	g_AnalogReading[6] = ADC_GetConvertedData(AT91C_BASE_ADC1,6);
	g_AnalogReading[7] = ADC_GetConvertedData(AT91C_BASE_ADC1,7);
	
	
	switch(g_BatteryCounter) {
		case 0:
			g_AnalogReading[0] = ADC_GetConvertedData(AT91C_BASE_ADC1, 0);
			SelectBattV();
			break;
		case 1:
		  CalculateBatteryPower(ADC_GetConvertedData(AT91C_BASE_ADC1, 0));
			SelectAng0();
			break;
		case BATTV_PERIOD:
			g_BatteryCounter = -1;
		default:
			g_AnalogReading[0] = ADC_GetConvertedData(AT91C_BASE_ADC1, 0);
	}
	g_BatteryCounter++;
}

int Analog(int port)
{
	if(port >= 0 && port <= 7) return g_AnalogReading[port];
	else return -1;
}

// state = 0 pullup is disabled
// state = 1 pullup is enabled
void AnalogPullup(int port, int state)
{
	char temp;
	int shifter = 8;
	
	switch (port) {
		case 0:
			shifter = 7;
			break;
		case 1:
			shifter = 0;
			break;
		case 2:
			shifter = 6;
			break;
		case 3:
			shifter = 1;
			break;
		case 4:
			shifter = 5;
			break;
		case 5:
			shifter = 2;
			break;
		case 6:
			shifter = 4;
			break;
		case 7:
			shifter = 3;
			break;
		default:
			return;
	}
	
	if(state){
		// set the pin to be pulled up
		g_AnalogPullups |= (1 << shifter);
	}
	else{
		// set the pin to be floating
		g_AnalogPullups &= (~(1 << shifter));
	}
	
	temp = ~g_AnalogPullups;
	// Write the data out to the chip
	WritePullupData(0x14,&temp,1);
}

void SetAnalogPullups(short mask)
{
	int i;
	
	for(i = 0;i < 8;i++) AnalogPullup(i, mask&(1<<i));
	
	g_AnalogPullupMask = mask;
}

short GetAnalogPullups()
{
	return g_AnalogPullupMask;
}

void SensorPowerOn(void)
{
	PIO_Clear(&(sensorsPower[0]));
}

void SensorPowerOff(void)
{
	PIO_Set(&(sensorsPower[0]));
}

int SensorPowerStatus(void)
{
	return PIO_Get(&(sensorsPower[1]));
}

void SelectBattV(void)
{
	// connects Analog pin 0 to Battery Voltage Divider
	PIO_Set(&(sensorsPower[2]));
}

void SelectAng0(void)
{
	// connects Analog pin 0 to Analog port 0
	PIO_Clear(&(sensorsPower[2]));
}

int BattVoltage(void)
{
	return g_BatteryAverage;
}

int Charging(void)
{
	return PIO_Get(&(sensorsPower[4]));
}

unsigned char WritePullupData(unsigned int iaddress, char *bytes, unsigned int num)
{
	unsigned int timeout;
	
	// wait for TWI bus to be ready
	while(!(TWI_TransferComplete(AT91C_BASE_TWI))) nop();
	
	// Start Writing
	TWI_StartWrite(AT91C_BASE_TWI,PULLUPADDR,iaddress,1,*bytes++);
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

