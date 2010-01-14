#ifndef __UART_H__
#define __UART_H__

#define UART_BUFFER_SIZE 2048

int UartInit();
int UartWrite(int uart, char *data, int len);
int UartRead(int uart, char *data, int len);
int UartReset(int uart);
void UartSetSigmask(int mask);
void UartStartRead();

#endif
