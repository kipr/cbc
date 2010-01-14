#ifndef __CBOB_CMD_H__
#define __CBOB_CMD_H__

/* Format for the following entries:
 *
 * CMD can be READ, WRITE, or CONFIG 
 *
 * CMD(arg1,arg2...)->(ret1,ret2...)
 *    Description

 * READ(port)->(value)
 *     Returns the digital port value, or
 *     the values of all ports if port == -1
 * WRITE(port,value)->()
 *     Sets the digital port to the specified
 *     value, or sets all of them if port == -1
 *     value is ignored for ports that are not
 *     currently set to digital outputs
 * CONFIG(port,state)->()
 *     Sets the digital port to be an output if
 *     state == 1, sets the state of all ports if
 *     port == -1                                */

// Digital IO and Config
#define CBOB_CMD_DIGITAL_READ   1
#define CBOB_CMD_DIGITAL_WRITE  2
#define CBOB_CMD_DIGITAL_CONFIG 3

#define CBOB_CMD_DIGITAL_CONFIG_SET_OUTPUT	0
#define CBOB_CMD_DIGITAL_CONFIG_SET_INPUT		0

/* READ */
#define CBOB_CMD_ANALOG_READ    4
#define CBOB_CMD_ANALOG_CONFIG  22

// Accel read and config
/* READ CONFIG */
#define CBOB_CMD_ACCEL_READ     5
#define CBOB_CMD_ACCEL_CONFIG   6

#define CBOB_CMD_ACCEL_CONFIG_SET_SCALE 0

/* READ CONFIG */
#define CBOB_CMD_SENSORS_READ   7
#define CBOB_CMD_SENSORS_CONFIG 8

#define CBOB_CMD_SENSORS_CONFIG_PACKET 0

/* READ WRITE CONFIG */
#define CBOB_CMD_UART_READ      9
#define CBOB_CMD_UART_WRITE     10
#define CBOB_CMD_UART_CONFIG    11

/* READ WRITE CONFIG */
#define CBOB_CMD_PWM_READ       12
#define CBOB_CMD_PWM_WRITE      13
#define CBOB_CMD_PWM_CONFIG     14

/* READ WRITE CONFIG */
#define CBOB_CMD_PID_READ       15
#define CBOB_CMD_PID_WRITE      16
#define CBOB_CMD_PID_CONFIG     17

/* READ WRITE CONFIG */
#define CBOB_CMD_SERVO_READ     18
#define CBOB_CMD_SERVO_WRITE    19
#define CBOB_CMD_SERVO_CONFIG   20

#define CBOB_CMD_STATUS_READ 21

#endif
