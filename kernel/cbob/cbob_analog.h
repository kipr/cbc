#ifndef __CBC_ANALOG_H__
#define __CBC_ANALOG_H__

#include "cbob.h"
#define CBOB_ANALOG_NAME  "cbob_analog"

struct analog_port {
  short port;
};

int  cbob_analog_init(void);
void cbob_analog_exit(void);

#endif
