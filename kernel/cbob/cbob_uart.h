#ifndef __CBC_UART_H__
#define __CBC_UART_H__

#include "cbob.h"
#define CBOB_UART_NAME  "cbob_uart"
#define CBOB_UART_MINORS 2

int  cbob_uart_init(void);
void cbob_uart_exit(void);

#endif
