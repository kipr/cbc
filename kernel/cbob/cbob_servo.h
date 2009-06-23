#ifndef __CBC_DIGITAL_H__
#define __CBC_DIGITAL_H__

#define CBOB_DIGITAL_MAJOR 220
#define CBOB_DIGITAL_NAME  "cbob_digital"

struct digital_port {
  short port;
};

int  cbob_digital_init(void);
void cbob_digital_exit(void);

#endif
