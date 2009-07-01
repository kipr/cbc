#ifndef __CBOB_H__
#define __CBOB_H__

#include <asm/ioctl.h>

#define CBOB_DIGITAL_MAJOR 220
#define CBOB_ANALOG_MAJOR  221
#define CBOB_SENSORS_MAJOR 222
#define CBOB_PWM_MAJOR     223
#define CBOB_PID_MAJOR     224
#define CBOB_SERVO_MAJOR   225
#define CBOB_ACCEL_MAJOR   226
#define CBOB_UART_MAJOR    227

// ioctls
//Digitals
#define CBOB_DIGITAL_SET_DIR _IOW(CBOB_DIGITAL_MAJOR, 0, int*) 
#define CBOB_DIGITAL_GET_DIR _IOR(CBOB_DIGITAL_MAJOR, 1, int*)

//PID
#define CBOB_PID_CLEAR_COUNTER _IO(CBOB_PID_MAJOR, 0)
#define CBOB_PID_SET_COUNTER   _IOW(CBOB_PID_MAJOR, 1, int*)
#define CBOB_PID_SET_GAINS     _IOW(CBOB_PID_MAJOR, 2, short*)
#define CBOB_PID_GET_GAINS     _IOR(CBOB_PID_MAJOR, 3, short*)
#define CBOB_PID_GET_DONE      _IOR(CBOB_PID_MAJOR, 4, int*)

#endif
