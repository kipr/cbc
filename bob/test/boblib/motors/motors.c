#include "motors.h"

#include <bob.h>
#include <pio/pio.h>
#include <pmc/pmc.h>
#include <pwmc/pwmc.h>
#include <timer/timer.h>
#include <adc/adc.h>

#include <stdio.h>

#define M0_LEFT_ADC
#define M1_LEFT_ADC
#define M2_LEFT_ADC
#define M3_LEFT_ADC

#define M0_RIGHT_ADC
#define M1_RIGHT_ADC
#define M2_RIGHT_ADC
#define M3_RIGHT_ADC

#define MAXIERROR	 10000
#define MINIERROR	-10000

Pin g_Motor[] = {MC_PWM};
Pin g_MotorOff[] = {MC_PWM_OFF};
Pin g_MotorDirection[] = {MC_DIR};
const Pin motorFeedback[] = {MADC};

volatile int g_MotorADC[8] = {0,0,0,0,0,0,0,0};
volatile int g_MotorThreshold[3] = {100,0,1000};// Position, Velocity, Acceleration thresholds
volatile int g_BackEMF[4] = {0,0,0,0};			// Motor Back-EMF readings
volatile int g_MotorCal[4] = {0,0,0,0};			// Motor calibration 
volatile int g_MotorPWM[4] = {0,0,0,0};			// PWM duty cycle of the motor
volatile int g_MotorCounter[4] = {0,0,0,0};		// Motor position counter
volatile int g_MotorTPC[4] = {0,0,0,0};			// Ticks Per Cycle of the motor
volatile int g_MotorInMotion;					// first 4 bits indicate motor is in Back-EMF control
volatile int g_MotorCtrlType[4] = {0,0,0,0};	// holds the current control type PWM=0,Speed=1,Position=2

volatile int g_TargetTPC[4] = {0,0,0,0};		// Motor target Ticks per Cycle
volatile int g_TargetPosition[4] = {0,0,0,0};	// Motor target position for move to position command
volatile int g_MotorIError[4] = {0,0,0,0};		// Accumulated speed error for Integral term
volatile int g_MotorLastTPC[4] = {0,0,0,0};		// Previous speed error for Derivative term
volatile int g_MotorGains[4][6];                // Proportional Mult,Integral Mult,Derivative Mult,
												// Proportional Divi,Integral Divi,Derivative Divi
static void MotorCallback(void);

int MotorPositionControl(int motor);
int PIDSpeedControl(int motor, int err);
void SetPWM(int motor, int pwm);
void AllOn(void);
void AllOff(void);

// Sensors and Timers must be initialized first!!!
void MotorsInit(void)
{
	int i=0;
	PIO_Configure(g_MotorOff, PIO_LISTSIZE(g_Motor));
	PIO_Configure(g_MotorDirection, PIO_LISTSIZE(g_MotorDirection));
	PIO_Configure(motorFeedback, PIO_LISTSIZE(motorFeedback));
	
	PMC_EnablePeripheral(AT91C_ID_ADC0);
	ADC_Initialize(AT91C_BASE_ADC0, AT91C_ID_ADC0, AT91C_ADC_TRGEN_DIS, \
				   0, AT91C_ADC_SLEEP_NORMAL_MODE, AT91C_ADC_LOWRES_10_BIT, \
				   MCK, BOARD_ADC_FREQ, 10, 600);
	for(i = 0;i < 8;i++) ADC_EnableChannel(AT91C_BASE_ADC0, i);
	
	// enable the PWM peripheral if it has not already been
	if(!PMC_IsPeriphEnabled(AT91C_ID_PWMC)) PMC_EnablePeripheral(AT91C_ID_PWMC);

	PWMC_ConfigureClocks(1000000, 0, MCK);	
	
	PWMC_ConfigureChannel(4, AT91C_PWMC_CPRE_MCK, 0, 0);
	PWMC_ConfigureChannel(5, AT91C_PWMC_CPRE_MCK, 0, 0);
	PWMC_ConfigureChannel(6, AT91C_PWMC_CPRE_MCK, 0, 0);
	PWMC_ConfigureChannel(7, AT91C_PWMC_CPRE_MCK, 0, 0);
	
	PWMC_SetPeriod(4, MC_PWM_PERIOD);
	PWMC_SetPeriod(5, MC_PWM_PERIOD);
	PWMC_SetPeriod(6, MC_PWM_PERIOD);
	PWMC_SetPeriod(7, MC_PWM_PERIOD);
	
	PWMC_EnableChannel(4);
	PWMC_EnableChannel(5);
	PWMC_EnableChannel(6);
	PWMC_EnableChannel(7);

	SetPitCallback(MotorCallback,0);
	
	// intialize the motor PID gains;
	for(i=0;i<4;i++) SetPIDGainsDefault(i);
	
	usleep(1000000); // wait for power up spike to pass and all motors stop moving before calibrating
	
	// make sure the motors calibrate properly, DO NOT SPIN MOTORS DURING CALIBRATION !!
	while(i < 5){
		if(MotorCalibration()){ 
			usleep(100000);
			i++;
		}
		else break;
	}
	PIO_Set(&(g_MotorDirection[4])); // ENABLE ALL MOTORS
}

// sets the PWM of a motor in port 0-3 with a PWM of +/- MC_PWM_PERIOD (240)
void SetPWM(int motor, int pwm)
{	
	if(pwm == 0) {
		PIO_Configure(&(g_MotorOff[motor]), 1);
		return;
	}
	// else configure the pin to use the PWM controller
	PIO_Configure(&(g_Motor[motor]), 1);
	// if the PWM is negative flip the direction pin
	if(pwm < 0) {
		PIO_Set(&(g_MotorDirection[motor]));
		pwm = -pwm;
	}
	else
		PIO_Clear(&(g_MotorDirection[motor]));
	
	
	// A period less than or equal to 1 does not turn on the PWM
	if(pwm > (MC_PWM_PERIOD-2)) pwm = MC_PWM_PERIOD-2;
	//pwm = MC_PWM_PERIOD - pwm;
	// set the duty cycle of the PWM of the motor
	PWMC_SetDutyCycle(motor+4, pwm);
}

// Turns off all motors
void AllOff(void)
{
	PIO_Configure(g_MotorOff, PIO_LISTSIZE(g_Motor));
}

// Turns on all motors to the PWM value remaining in the Duty Cycle register
void AllOn(void)
{
	PIO_Configure(g_Motor, PIO_LISTSIZE(g_Motor));
}	

void UpdateBEMF(void)
{
	ADC_StartConversion(AT91C_BASE_ADC0);
	while((ADC_GetStatus(AT91C_BASE_ADC0) & 0x80) != 0x80) nop();
	
	PIO_Set(&(g_MotorDirection[4])); // ENABLE ALL MOTORS
	
	g_MotorADC[0] = ADC_GetConvertedData(AT91C_BASE_ADC0,0);
	g_MotorADC[1] = ADC_GetConvertedData(AT91C_BASE_ADC0,1);
	g_MotorADC[2] = ADC_GetConvertedData(AT91C_BASE_ADC0,2);
	g_MotorADC[3] = ADC_GetConvertedData(AT91C_BASE_ADC0,3);
	g_MotorADC[4] = ADC_GetConvertedData(AT91C_BASE_ADC0,4);
	g_MotorADC[5] = ADC_GetConvertedData(AT91C_BASE_ADC0,5);
	g_MotorADC[6] = ADC_GetConvertedData(AT91C_BASE_ADC0,6);
	g_MotorADC[7] = ADC_GetConvertedData(AT91C_BASE_ADC0,7);
		
	// take the difference between the BEMF readings to reduce error 
	g_BackEMF[0] = g_MotorADC[0] - g_MotorADC[1];
	g_BackEMF[1] = g_MotorADC[2] - g_MotorADC[3];
	g_BackEMF[2] = g_MotorADC[5] - g_MotorADC[4];
	g_BackEMF[3] = g_MotorADC[7] - g_MotorADC[6];
	
	// correct any bias using the calibration values
	g_BackEMF[0] -= g_MotorCal[0];
	g_BackEMF[1] -= g_MotorCal[1];
	g_BackEMF[2] -= g_MotorCal[2];
	g_BackEMF[3] -= g_MotorCal[3];
}

int MotorCalibration(void)
{
	int i,j;
	int bias[] = {0,0,0,0};
	
	for(i=0;i<4;i++) g_MotorCal[i] = 0;
	
	AllOff();	// turn off all motors
	
	g_MotorInMotion = 0;
	usleep(15000);
	for(i=0;i<8;i++){
		for(j=0;j<4;j++) bias[j] += g_BackEMF[j];
		usleep(15000);
	}
	
	for(i=0;i<4;i++) g_MotorCal[i] = bias[i]/8;
	
	// if any motor calibration is out of bounds return the motor acting up
	for(i=0;i<4;i++) if(g_MotorCal[i] > 5 || g_MotorCal[i] < -5) return i;
	// otherwhise all motors are normal
	return 0;
}

static void MotorCallback(void)
{
	static int i;

	PIO_Clear(&(g_MotorDirection[4]));	// turn off motors
	
	// the following sleep must not be longer than the period of the PIT timer (< 10ms)
	usleep(MC_INDUCTION_SPIKE); // wait for induction spike to settle
	
	UpdateBEMF();			// read in back EMF analog readings
	
	// update all counters and speeds
	for(i=0;i<4;i++){
		if(g_BackEMF[i] > 3 || g_BackEMF[i] < -3){
			g_MotorTPC[i] = g_BackEMF[i];
			g_MotorCounter[i] += g_BackEMF[i];
		}
		else g_MotorTPC[i] = 0;
	}
	
	// cycle through all 4 motors
	for(i=0;i<4;i++){
		
		switch(g_MotorCtrlType[i]){
			case 0:
				// motor is not in BEMF	motion just setting the PWM
				g_MotorInMotion &= ~(1<<i);
				break;
			case 1:
				// motor is in BEMF speed control
				g_MotorInMotion |= (1<<i); // set motor in motion bit
				if(g_TargetTPC[i] == 0)  g_MotorPWM[i] = 0;
				else g_MotorPWM[i] = PIDSpeedControl(i, g_TargetTPC[i] - g_MotorTPC[i]);
				break;
			case 2:
				// motor is in BEMF position and speed control
				g_MotorInMotion |= (1<<i); // set motor in motion bit
				g_MotorPWM[i] = MotorPositionControl(i);
				break;
			default:
				g_MotorInMotion &= ~(1<<i);
				g_MotorPWM[i] = 0;
				break;
		}
		
		// update motors with new speeds
		SetPWM(i,g_MotorPWM[i]);
	}
}

int MotorPositionControl(int motor)
{	
	int diffPosition = g_TargetPosition[motor] - g_MotorCounter[motor];
	int speed = 0;
	
	// is the difference in position within the threshold values
	if((diffPosition > -g_MotorThreshold[0]) && (diffPosition < g_MotorThreshold[0]))
	{
		g_MotorInMotion &= ~(1<<motor);		// clear the Motor in motion bit
		//g_MotorCtrlType[motor] = 0;			// turn motor position control off
	}
	else
	{
		speed = (diffPosition);
		if(speed > g_TargetTPC[motor]) speed = g_TargetTPC[motor];
		else if(speed < -g_TargetTPC[motor]) speed = -g_TargetTPC[motor];
		
		speed = PIDSpeedControl(motor, speed - g_MotorTPC[motor]);
	}
	
	return speed;
}

int PIDSpeedControl(int motor, int err)
{
	int Pterm=0, Iterm=0, Dterm=0, PIDterm;
	
	// Proportional Term
	////////////////////////////////////////////////////////////////////////////////
	Pterm = (g_MotorGains[motor][0] * err)/g_MotorGains[motor][3];
	
	// Integral Term
	////////////////////////////////////////////////////////////////////////////////
	g_MotorIError[motor] += err;
	// is the error within the maximum or minimum motor power range?
	if(g_MotorIError[motor] > MAXIERROR) g_MotorIError[motor] = MAXIERROR;
	else if(g_MotorIError[motor] < MINIERROR) g_MotorIError[motor] = MINIERROR;
	
	Iterm = (g_MotorGains[motor][1] * g_MotorIError[motor])/g_MotorGains[motor][4];
	
	// Derivative Term
	////////////////////////////////////////////////////////////////////////////////
	Dterm = (g_MotorGains[motor][2] * (g_MotorTPC[motor] - g_MotorLastTPC[motor]))/g_MotorGains[motor][5];
	g_MotorLastTPC[motor] = g_MotorTPC[motor];
	
	PIDterm = Pterm + Iterm + Dterm;
	
	if(PIDterm > MC_PWM_PERIOD) return MC_PWM_PERIOD;
	else if(PIDterm < -MC_PWM_PERIOD) return -MC_PWM_PERIOD;
	else return PIDterm;
}

int GetMotorError(int motor)
{
	return g_MotorIError[motor];
}

int GetMotorTPC(int motor)
{
	return g_MotorTPC[motor];
}

int GetTargetTPC(int motor)
{
	return g_TargetTPC[motor];
}


void GetPIDGains(short num, short *PM, short *IM, short *DM, short *PD, short *ID, short *DD)
{
	*PM = g_MotorGains[num][0];
	*IM = g_MotorGains[num][1];
	*DM = g_MotorGains[num][2];
	*PD = g_MotorGains[num][3];
	*ID = g_MotorGains[num][4];
	*DD = g_MotorGains[num][5];
}
/*
void GetPIDGains(short *PM, short *IM, short *DM, short *PD, short *ID, short *DD)
{
	int num = 0;
	*PM = g_MotorGains[num][0];
	*IM = g_MotorGains[num][1];
	*DM = g_MotorGains[num][2];
	*PD = g_MotorGains[num][3];
	*ID = g_MotorGains[num][4];
	*DD = g_MotorGains[num][5];
} */

void SetPIDGains(int num, short PM, short IM, short DM, short PD, short ID, short DD)
{
	g_MotorGains[num][0] = PM;
	g_MotorGains[num][1] = IM;
	g_MotorGains[num][2] = DM;
	g_MotorGains[num][3] = PD;
	g_MotorGains[num][4] = ID;
	g_MotorGains[num][5] = DD;
}

void SetPIDGainsDefault(int num)
{
	g_MotorGains[num][0] = 4;
	g_MotorGains[num][1] = 1;
	g_MotorGains[num][2] = -1;
	g_MotorGains[num][3] = 1;
	g_MotorGains[num][4] = 1;// 5;
	g_MotorGains[num][5] = 3;
}

void SetPIDAllGains(short PM, short IM, short DM, short PD, short ID, short DD)
{
	int i;
	for(i=0;i<4;i++) SetPIDGains(i,PM,IM,DM,PD,ID,DD);
}

void MoveToPosition(int motor, int speed, int position)
{	
	g_MotorCtrlType[motor] = 2;
	// set target position to maximum distance or 2^32/4 (dealing with signed ints)
	// for black modded servo motors 1 rev = 44000 ticks
	// rescale the motor speed by dividing by 2
	if(speed < 0) speed = -speed;
	speed = (speed >> 1);
		
	g_TargetTPC[motor] = speed;
	
	g_TargetPosition[motor] = position * MC_POSITION_SCALE;
}

void MoveAtVelocity(int motor, int speed)
{
	g_MotorCtrlType[motor] = 1;
	// rescale the speed by dividing by 2
	if(speed < 0){
		speed = -speed;
		speed = speed >> 1;
		speed = -speed;
	}
	else speed = speed >> 1;
	
	g_TargetTPC[motor] = speed;
}

void Motor(int motor, int power)
{	
	g_MotorCtrlType[motor] = 0;
	g_MotorPWM[motor] = MC_PWM_PERIOD*power/100;	// re-scale the motor power to pwm settings
}

// returns 1 when motor has reached target position since last MTP command
int BlockMotorDone(int motor)
{
	while(g_MotorInMotion & (1<<motor)) nop();
	return 1;
}

// returns 0 if motor is still moving to position
// returns 1 if the motor has reached the position since the last MTP command
int IsMotorDone(int motor)
{
	if(g_MotorInMotion & (1<<motor)) return 0;
	else return 1;
}

int GetMotorCounter(int motor)
{
	return (g_MotorCounter[motor]/MC_POSITION_SCALE);
}

int ClearMotorCounter(int motor)
{
	g_MotorCounter[motor] = 0;
	g_MotorIError[motor] = 0;
	return g_MotorCounter[motor];
}

int SetMotorCounter(int motor, int counter)
{
	g_MotorCounter[motor] = counter*MC_POSITION_SCALE;
	g_MotorIError[motor] = 0;
	return g_MotorCounter[motor];
}

int GetMotor(int motor)
{
	int power;
	power = (g_MotorPWM[motor]*100)/MC_PWM_PERIOD;
	return power;
}
