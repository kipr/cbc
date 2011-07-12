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

#ifndef __CBC_DATA_H__
#define __CBC_DATA_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct __cbc_data{
	// sensors
	// From Bob to Chumby
	int analogs[8]; // 10 bits resolution
	int acc_x; // 12 bits resolution
	int acc_y;
	int acc_z;
	int digitals; //should be char? bits 0-7
	int button; //should be char? bit 0
	int motor_speed[4];
	int motor_counter[4]; //32-bit signed integrated motor position counters
	int motor_in_motion; // bits 0-3 are 1 if motor is executing BEMF command
	int volts;//the 12 bit voltage reading
	//effectors
	// From Chumby to BoB
        int motor_thresholds[3];
	int motor_pwm[4]; // pwm values for setting motor
	int motor_tps[4]; // tics per second values for motor speeds (for BEMF commands)
	int motor_counter_targets[4]; // absolute position targets (set to +/- 2147483647 if position independent BEMF command is used)
	int clear_motor_counters; // low 4 bits set to 1 by user to reset encoders. Library routine clears the bit after it receives a 0 encoder value
	int servo_targets[4]; // Servos move left 0 to right 2047
	int enable_servos; // 1 for power on to servos, 0 is off, only 1 bit for all 4 servos
	int enable_digital_outputs; 
	int digital_output_values; 
	int pid_gains[4][6];//to change the motor PID functions for each channel
	
	//int for Chumby only
	int tone_freq; // pitch in Hz for the speaker; value of 0 is speaker off
	unsigned char a;
	unsigned char b;
	unsigned char up;
	unsigned char down;
	unsigned char left;
	unsigned char right;
	
	//int for simulator only
	float fmcounters[4];
	float ptime;//previous wall time
} cbc_data;

#ifdef __cplusplus
}
#endif

#endif
