#ifndef __CBC_UART_H__
#define __CBC_UART_H__

#include "cbob.h"
#define CBOB_UART_NAME  "cbob_uart"

struct uart_port {
  short port;
};

int  cbob_uart_init(void);
void cbob_uart_exit(void);

#endif
