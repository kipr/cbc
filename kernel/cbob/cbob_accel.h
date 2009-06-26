#ifndef __CBC_ACCEL_H__
#define __CBC_ACCEL_H__

#include "cbob.h"
#define CBOB_ACCEL_NAME  "cbob_accel"

struct accel_axis {
  short axis;
};

int  cbob_accel_init(void);
void cbob_accel_exit(void);

#endif
