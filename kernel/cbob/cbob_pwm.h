#ifndef __CBC_PWM_H__
#define __CBC_PWM_H__

#include "cbob.h"

#define CBOB_PWM_NAME  "cbob_pwm"

struct motor_pwm {
  short port;
};

int  cbob_pwm_init(void);
void cbob_pwm_exit(void);

#endif
