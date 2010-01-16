#ifndef __CBC_SERVO_H__
#define __CBC_SERVO_H__

#include "cbob.h"

#define CBOB_SERVO_NAME  "cbob_servo"

struct servo_port {
  short port;
};

int  cbob_servo_init(void);
void cbob_servo_exit(void);

#endif
