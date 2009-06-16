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

#ifndef __CBOB_STRUCT_H__
#define __CBOB_STRUCT_H__

#define CBOB_DATA_KEY 0xDEADBEEF
#define CBOB_PADDING_COUNT 16

typedef unsigned char uint8;
typedef unsigned int uint32;

typedef struct __Cbob_Data {
//Verification key
    uint32 key;
// ARM ADC
    int Analog[8];  // 10 bit analog readings from analog ports 8-15
// ARM Digital
    int Digitals;  // state of digital ports 0-7
    int Enable_Digital_Outputs; // first 8 bits indicate if the digital port is an output
    int Digital_Output_Values; // state of port if configured as digital output
// Accelerometer 12 bit reading
    int Acc_X; // 12 bit reading
    int Acc_Y;
    int Acc_Z;
// Battery Voltage 12 bit reading
    int Batt_Voltage;
// Gray Button state indicator
    int ButtonState;    // state of the gray button on the right of the BOB
// Motor control
    int Motor_Thresholds[3];  // position,velocity, acceleration thresholds used in MotorMTP
    int Motor_PID_Gains[4][6];  //  integers are PID multiply gains,
    int Motor_In_Motion;   // first 4 bits indicate if motor is in Back-EMF control
    int Motor_PWM[4];    // PWM duty cycle of the motor
    int Motor_TPS[4];    // Ticks per second of the motor
    int Motor_TPS_Targets[4];  // Ticks per second intended to run at
    int Motor_Counter[4];   // Motor position counter
    int Motor_Counter_Targets[4]; // Motor target position for move to position command
    int Motor_Clear_Counter;  // first 4 bits indicate a motor position counter reset
// Servos
    int Servo_Position[4];   // Servos are 11 bit values 0-2048
    int Enable_Servos;    // Turn on servos 1, Turn off servos 0

// Serial stuff
    uint8 UART0_Buffer[48];
    uint8 UART1_Buffer[48];

    uint8 UART0_Buffer_Count;
    uint8 UART1_Buffer_Count;

    uint8 UART0_Buffer_Status;
    uint8 UART1_Buffer_Status;

    uint8 padding[CBOB_PADDING_COUNT];
} Cbob_Data;

#endif //__CBOB_STRUCT_H__
