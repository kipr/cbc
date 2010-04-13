/**************************************************************************
 *  Copyright 2008,2009 KISS Institute for Practical Robotics             *
 *                                                                        *
 *  This file is part of CBC Firmware.                                    *
 *                                                                        *
 *  CBC Firmware is free software: you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation, either version 2 of the License, or     *
 *  (at your option) any later version.                                   *
 *                                                                        *
 *  CBC Firmware is distributed in the hope that it will be useful,       *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *  GNU General Public License for more details.                          *
 *                                                                        *
 *  You should have received a copy of the GNU General Public License     *
 *  along with this copy of CBC Firmware.  Check the LICENSE file         *
 *  in the project root.  If not, see <http://www.gnu.org/licenses/>.     *
 **************************************************************************/

//CBC user library functions
//--DPM 12/27/08

//Note that button functions and display function have not been updated yet (see end of file)
//Note that voltage and sonar scaling probably need adjustment

////////////////////////////////////////////////////////////////////////////////////////////
// Function prototypes
/* Includes the Standard IO Library */

#define PENDING(func) printf("Function %s is not yet implemented\n", func)

#include "cbc.h"
#include <assert.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <shared_mem.h>
#include <sys/ioctl.h>
#include "../../../cbcui/src/UIData.h"

#include "../../../kernel/cbob/cbob.h"

int __pid_defaults[6]={30,0,-30,70,1,51};
int __position_threshold=2000;

void kissSim_init(int world, int rx, int ry, int rt){}
void kissSim_end(){}
void kissSimEnablePause(){}
void kissSimPause(){}
int kissSimActive(){return 1;}

static int g_cbc_initted = 0;
static int g_button;
static int g_digital[8];
static int g_analog[8];
static int g_battery;
static int g_pid[4];
static int g_pwm[4];
static int g_servo[4];
static int g_accX, g_accY, g_accZ;

shared_mem *g_uidata_sm = 0;
UIData *g_uidata = 0;

void libcbc_init()
{
	char devname[32];
	int i;
	
	if(g_cbc_initted) return;
	g_cbc_initted = 1;
	
        g_button = open("/dev/cbc/button", O_RDONLY);
	
	for(i = 0;i < 8;i++) {
                sprintf(devname,"/dev/cbc/digital%d", i+8);
                g_digital[i] = open(devname, O_RDWR);
		sprintf(devname, "/dev/cbc/analog%d", i);
		g_analog[i] = open(devname, O_RDONLY);
	}
	
	g_battery = open("/dev/cbc/battery", O_RDONLY);
	
	for(i = 0;i < 4;i++) {
		sprintf(devname, "/dev/cbc/pid%d", i);
		g_pid[i] = open(devname, O_RDWR);
		sprintf(devname, "/dev/cbc/pwm%d", i);
		g_pwm[i] = open(devname, O_RDWR);
		sprintf(devname, "/dev/cbc/servo%d", i);
		g_servo[i] = open(devname, O_RDWR);
	}
	
	g_accX = open("/dev/cbc/accelX", O_RDONLY);
	g_accY = open("/dev/cbc/accelY", O_RDONLY);
	g_accZ = open("/dev/cbc/accelZ", O_RDONLY);

	g_uidata_sm = shared_mem_create("/tmp/cbc_uidata", sizeof(UIData));
	assert(g_uidata_sm);
	g_uidata = (UIData *)shared_mem_ptr(g_uidata_sm);
	
	atexit(libcbc_exit);
}

void libcbc_exit()
{
    int i;

    for(i = 0;i < 8;i++){
        close(g_digital[i]);
        close(g_analog[i]);
    }
    close(g_battery);
    close(g_button);

    for(i = 0;i < 4;i++) {
        close(g_pid[i]);
        close(g_pwm[i]);
        close(g_servo[i]);
    }

    close(g_accX);
    close(g_accY);
    close(g_accZ);
}

/////////////////////////////////////////////////////////////
// Tone Functions
void tone(int frequency, int duration)
{/*
	if(frequency <5 || frequency > 6000)frequency=0;
	cbc->tone_freq=frequency;
	msleep(duration);
	cbc->tone_freq=0;*/
        PENDING("tone");
//        printf("\a");
}

void beep()
{
        g_uidata->state = 1;
	printf("\a");
}

void play_sound(const char *filename)
{
        g_uidata->state = 2;
        strcpy(g_uidata->filename,filename);
        printf("\a");
}

int playing_sound()
{
    g_uidata->state = 3;
    printf("\a");
    if(g_uidata->playing < 2) return 0;
    else return 1;
}

void stop_sound()
{
        g_uidata->state = 4;
        printf("\a");
}

void start_recording(const char *filename, int length)
{
        g_uidata->state = 5;
        g_uidata->recordTime = length;
        strcpy(g_uidata->filename,filename);
        printf("\a");
}

int recording_sound()
{
    return g_uidata->recording;
}

void stop_recording()
{
        g_uidata->state = 6;
        printf("\a");
}

/////////////////////////////////////////////////////////////
// Sensor Functions

// retruns 1 or 0 unless port is out of bounds, then -1
int digital(int port)
{
	short data;
  
	if(port >= 8 && port <= 15) {
            read(g_digital[port-8], &data, 2);
                return data;
	}
	else{
		printf("Digital must be between 8 and 15\n");
		return -1;
        }
}


void set_digital_output_value(int port, int value)
{
    char state = value;

    if(port < 8 || port > 15) return;

    write(g_digital[port-8], &state, 1);
}

void set_digital_port_direction(int mask)
{
    ioctl(g_digital[0], CBOB_DIGITAL_SET_DIR, &mask);
}

int get_digital_port_direction()
{
    int mask = 0;

    ioctl(g_digital[0], CBOB_DIGITAL_GET_DIR, &mask);

    return mask;
}

int analog10(int port)
{
	unsigned short data;
	
	if(port >= 0 && port <= 7) {
		read(g_analog[port], &data, 2);
		return (int)data;
	}
	else {
		printf("Analog sensors must be between 0 and 7\n");
		return -1;
	}
}

// 8-bit analog for HB compatibility
int analog(int port)
{
  return analog10(port)>>2;
}

void set_analog_floats(int mask)
{
	mask = (~mask)&0xFF;
	
	ioctl(g_analog[0], CBOB_ANALOG_SET_PULLUPS, &mask);
}

int get_analog_floats()
{
        int mask = 0;
	
	ioctl(g_analog[0], CBOB_ANALOG_GET_PULLUPS, &mask);
	
	return (~mask)&0xFF;
}

void set_each_analog_state(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7)
{
       int mask = a0 | (a1<<1) | (a2<<2) | (a3<<3) | (a4<<4) | (a5<<5) | (a6<<6) | (a7<<7);
       set_analog_floats(mask);
}


/////////////////////////////////////////////////////////////
// Accelerometer Functions

int accel_x() 
{
	short data;
	
	read(g_accX, &data, 2);
	
	return data;
}

int accel_y() 
{
	short data;
	
	read(g_accY, &data, 2);
	
	return data;
}

int accel_z() 
{
	short data;
	
        read(g_accZ, &data, 2);
	
	return data;
}


//////////////////////////
// Sonar function: returns distance in mm

int sonar(int port)
{
	if(port >= 0 && port < 8) {
		return ((int)((float)analog10(port)*8.79));
	}
	
	printf("Sonar port must be between 0 and 7\n");
	return -1;
}

// returns distance in inches
int sonar_inches(int port)
{
	if(port >= 0 && port < 8) {
	  return ((int)(0.346*(float)analog10(port)));
	}        
	printf("Sonar port must be between 0 and 7\n");
	return -1;
}

//////////////////////////
// Battery power functions

float power_level()
{
	short data;
	float volts;
	
	read(g_battery, &data, 2);
	
	return ((float)data)/1000.0;
}



/////////////////////////////////////////////////////////////
// Servo Functions
void enable_servos()
{
	int i;
	
	for(i = 0;i < 4;i++) {
		if(get_servo_position(i) == -1)
			set_servo_position(i, 1024);
	}
}

void disable_servos()
{
	int i;
	
	for(i = 0;i < 4;i++)
		set_servo_position(i, -1);
}

int set_servo_position(int servo, int pos)
{
	short position = pos;
	
	if(servo < 0 || servo > 3) {
		printf("Servo must be 0..3\n");
		return -1;
	}
	
	if(position < -1 || position > 2048) {
		printf("Position must be from -1..2048");
		return -1;
	}
	
	write(g_servo[servo], &position, 2);
	
	return 0;
}

int get_servo_position(int servo)
{
	short position;
	
	if(servo < 0 || servo > 3) {
		printf("Servo must be 0..3\n");
		return -1;
	}
	
	read(g_servo[servo], &position, 2);
	
	return position;
}

/////////////////////////////////////////////////////////////
// Motor Functions

//////////////////////////
// CBC-specific motor functions


#define MAX_VEL   1000
#define MAX_POS_POS 1073741824
#define MAX_NEG_POS -1073741824


int get_motor_position_counter(int motor)
{
	int counter;
	
	if(motor < 0 || motor > 3) {
		printf("Motor must be 0..3\n");
		return 0;
	}
	
	read(g_pid[motor], &counter, 4);
	
	return counter;
}


int clear_motor_position_counter(int motor)
{
	if(motor < 0 || motor > 3) {
		printf("Motor must be 0..3\n");
		return -1;
	}
	
	return ioctl(g_pid[motor], CBOB_PID_CLEAR_COUNTER);
}


int move_at_velocity(int motor, int velocity)
{
	short v = velocity;
	
	if(motor < 0 || motor > 3) {
		printf("Motor must be 0..3\n");
		return -1;
	}
	
	write(g_pid[motor], &v, 2);
	
	return 0;
}

int mav(int motor, int velocity)
{ return move_at_velocity(motor, velocity); }

// Move motor to goal_pos at given velocity.  The amount actually
// moved will be goal_pos - get_motor_position().
int move_to_position(int motor, int speed, int goal_pos)
{ 
	short v = speed;
	int p = goal_pos;
	char outdata[6];
	
	if(motor < 0 || motor > 3) {
		printf("Motor must be 0..3\n");
		return -1;
	}
	
	memcpy(outdata, &v, 2);
	memcpy(outdata+2, &p, 4);
	
	write(g_pid[motor], outdata, 6);
	
	return 0;
}

int mtp(int motor, int velocity, int goal_pos)
{ return(move_to_position(motor, velocity, goal_pos)); }


// Move delta_pos relative to the current position of the motor.  The
// final motor tick will be get_motor_position() at the time of the
// call + delta_pos.
int move_relative_position(int motor, int speed, int delta_pos)
{ 
  return move_to_position(motor, speed, get_motor_position_counter(motor)+delta_pos);
}


int mrp(int motor, int velocity, int delta_pos)
{
	return(move_relative_position(motor, velocity, delta_pos));
}

//Turns off or actively holds the motor in position depending  on the situation -- but it may drift
int freeze(int motor)
{
	if(motor <0 || motor>3) {
		printf("Motors are 0 to 3\n");
		return -1;
	}
	
	return move_to_position(motor, 500, get_motor_position_counter(motor));
}

void set_pid_gains(int motor, int p, int i, int d, int pd, int id, int dd)
{
	short gains[6];
	
	gains[0] = p;
	gains[1] = i;
	gains[2] = d;
	gains[3] = pd;
	gains[4] = id;
	gains[5] = dd;
	
        ioctl(g_pid[motor], CBOB_PID_SET_GAINS, gains);
}

void get_pid_gains(int motor, int *p, int *i, int *d, int *pd, int *id, int *dd)
{
	short gains[6];
	
        ioctl(g_pid[motor], CBOB_PID_GET_GAINS, gains);
	
	*p = gains[0];
	*i = gains[1];
	*d = gains[2];
	*pd = gains[3];
	*id = gains[4];
	*dd = gains[5];
}

//returns 0 if motor is in motion and 1 if it has reached its target position
int get_motor_done(int motor)
{
	int done;
	
	if(motor < 0 || motor > 3) {
		printf("Motor must be 0..3\n");
		return -1;
	}
	
	ioctl(g_pid[motor], CBOB_PID_GET_DONE, &done);
	
	return done;
}

void bmd(int motor)
{
	block_motor_done(motor);
}

void block_motor_done(int motor)
{
	if(motor < 0 || motor > 3) {
		printf("Motor must be 0..3\n");
		return;
	}
        msleep(10);
	//loop doing nothing while motor position move is in progress
	while(!get_motor_done(motor)){msleep(10);}
}

int setpwm(int motor, int pwm)
{
	signed char power = pwm;
	
	if(motor < 0 || motor > 3) {
		printf("Motor must be 0..3\n");
		return -1;
	}
	
	if(pwm < -100 || pwm > 100) {
		printf("Pwm must be -100..100\n");
		return -1;
	}
	
	write(g_pwm[motor], &power, 1);
}

int getpwm(int motor)
{
	signed char power;
	
	if(motor < 0 || motor > 3) {
		printf("Motor must be 0..3\n");
		return -1;
	}
	
	read(g_pwm[motor], &power, 1);
	
	return power;
}

//////////////////////////
// Classic pwm motor functions, scaled for XBC and Blk gear motors
/***************************************************************** */
/*                                                                */
/* MOTOR PRIMITIVES                                               */
/*                                                                */
/*   fd(n) sets motor n to full on in the green direction    */
/*   bk(n) sets motor n to full on in the red direction      */
/*   motor(n, s) sets motor n on at speed s;               */
/*     s= 100 is full on green,                  */
/*     s= -100 is full on red,                   */
/*     s= 0 is off                               */
/*   off(n)  turns off motor n      */
/*                                                                */
/*   alloff() turns off all motors                      */
/*   ao()  turns off all motors                      */
/*                                                                */
/*                                                                */
/*   motors are numbered 0 through 3.                          */
/***************************************************************** */

void fd(int motor)
{
	//    move_at_velocity(motor, MAX_VEL);
	setpwm(motor,100);
}

void bk(int motor)
{ 
	setpwm(motor,-100);
	//    move_at_velocity(motor, -MAX_VEL);
}

void motor(int port, int speed)
{
	setpwm(port, speed);
}



void off(int motor)
{
	setpwm(motor, 0);
}

void alloff()
{
	setpwm(0, 0);
	setpwm(1, 0);
	setpwm(2, 0);
	setpwm(3, 0);
}

void ao()
{
	setpwm(0, 0);
	setpwm(1, 0);
	setpwm(2, 0);
	setpwm(3, 0);
}




/////////////////////////////////////////////////////////////
// Button Functions
int black_button()
{
	short data;
	
        read(g_button, &data, 2);
	
        return data;
}

int up_button()
{
  return g_uidata->up_button;
}

int down_button()
{
  return g_uidata->down_button;
}

int left_button()
{
  return g_uidata->left_button;
}

int right_button()
{
  return g_uidata->right_button;
}

int a_button()
{
  return g_uidata->a_button;
}

int b_button()
{
  return g_uidata->b_button;
}

void display_clear()
{
  printf("%c",12);
}

/////////////////////////////////////////////////////////////
// Display Functions
// Copyright(c) KIPR, 2009
// Full screen management functions for the display window provided with the CBC console screen
//   1. cbc_display_clear()
//   2. cbc_display_printf(int column, int row, <printf arguments>)
//
// Console screen display window size is 7 rows and limited to 28 columns, indexed from 0
// (variable pitch font used by CBC necessitates using a shortened width and prevents using
//  the slicker strategy of treating the display as one very long string)
//
// Initial version: 1/27/2009 - cnw
//
// stdarg.h provides macros for accessing a function's argument list ... see K&R

#define _MAPy 8
#define _MAPx 30  // last column is for \0
char _display_map[_MAPy][_MAPx];

#include <stdarg.h>
#include <string.h>

void cbc_printf(int col, int row, char *t, ...) { // column, row, string with format phrases, args
	va_list argp;    // argp is typed for traversing the variable part of the arg list
	int i; char *c; double d;  // working variables to receive arg values
	char *cp, *fmte, sc;  // cp traverses format string t, fmte marks end of each format phrase, sc is switch control

	int y;           // row index
	char *dp;        // pointer into display
	int maxw;        // available room on line
	char fws[_MAPx]; // formatted phrase work area
	char fmt[_MAPx]; int fl; // fmt is a working string for each format extracted

	va_start (argp,t);  // t is last named argument in cbc_printf's function header;
	// this initializes argp to point to first variable arg

	dp = &_display_map[row][col]; // starting point for printf
	maxw=_MAPx-col;               // space remaining on line
	for (cp = t; *cp; cp++)       // process printf string; stop when *cp = '\0'
	{
		if (*cp != '%')           // if not a format phrase
		{
			if(strspn(cp,"\n\t\v\b\r\f")>0) {  // is it a spec char? if so treat as if \n
				for (i=0;i<maxw;i++) { // clear balance of line
					*dp=' '; dp++;
				}
				if (row==_MAPy) break; // out of rows so proceed to display refresh
				row++;
				dp = &_display_map[row][0]; maxw=_MAPx; // set up for new line
			}
			else { // nothing special about this one so insert it
				*dp=*cp; dp++;
				maxw--; if (maxw==0) break; // no more room on line so proceed to display refresh
			}
			continue;             // return to top
		}
		// OK, if we're here we may have hit a format phrase
		fmte = strpbrk(cp+1, "dioxXucsfeEgG%"); // look for format end
		// strpbrk returns the location of 1st character of its argument that is in the scan string
		if (fmte == NULL)        // what's left is not a format phrase so insert % and return to top
		{
			*dp='%'; dp++;
			maxw--; if (maxw==0) break; // no more room on line so proceed to display refresh
			continue;            // return to top
		}
		// OK, there looks to be a format phrase
		sc = *fmte;              // set switch control for the case
		fl = 1+fmte-cp;          // pick off phrase (pointed to by cp)
		strncpy(fmt,cp,fl);      // capture the format phrase
		fmt[fl] = '\0';          // make it a string
		switch (sc)              // process the kind of format specified
		{
			case 'd': case 'i': case 'o': case 'x': case 'X': case 'u': case 'c':
			i = va_arg(argp, int);    // return next parm as type int and step argp
			sprintf(fws,fmt,i);       // use sprintf to do the formatting to fws
			break;
			case 's':
			c = va_arg(argp, char *); // return next parm as type char * and step argp
			sprintf(fws,fmt,c);       // use sprintf to do the formatting to fws
			break;
			case 'f': case 'e': case 'E': case 'g': case 'G':
			d = va_arg(argp, double); // return next parm as type double and step argp
			sprintf(fws,fmt,d);       // use sprintf to do the formatting to fws
			break;
			case '%':                 // no format specified between %'s
			sprintf(fws,fmt);
			break;
		}
		for(i=0; i<strlen(fws); i++) {// insert formatted phrase in display map
			if (maxw==0) break;       // if no more room get out of this
		    *dp = fws[i];             // insert next character from formatted phrase
			dp++; maxw--;
		}
		if (maxw==0) break;           // if no more room proceed to display refresh
		cp = fmte;                    // set cp for next pass
	}
	va_end(argp);                     // clean up
	for(y=0;y<_MAPy; y++) _display_map[y][_MAPx-1]='\0';  // make each row a string
	for(y=0;y<_MAPy; y++) printf("\n%s",_display_map[y]); // refresh the display
}

void cbc_display_clear() {
	int i;
		           //12345678901234567890123456789
	char bstring[]="                                            ";
	bstring[_MAPx]='\0';
	// blow it away and reset map
	for (i=0;i<_MAPy+1;i++) printf("\n");
	for (i=0;i<_MAPy;i++)
		strcpy(_display_map[i],bstring);
}

