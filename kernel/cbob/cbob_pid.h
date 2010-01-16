#ifndef __CBC_PID_H__
#define __CBC_PID_H__

#include "cbob.h"

#define CBOB_PID_NAME  "cbob_pid"

struct pid_port {
  short port;
};

int  cbob_pid_init(void);
void cbob_pid_exit(void);

#endif
