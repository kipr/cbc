#ifndef __CBC_SPI__
#define __CBC_SPI__

#include <linux/config.h>
#include <linux/types.h>
#include <asm/semaphore.h>

/* 
 * CBC_DELAY is in microseconds
 * CBC_DELAY_COUNT is how many loops before timeout
 * Timeout is CBC_DELAY*CBC_DELAY_COUNT usecs
 */
#define CBC_DELAY 25
#define CBC_DELAY_COUNT 60

void cbob_spi_init(void);
void cbob_spi_exit(void);
int  cbob_spi_message(short cmd, short *outbuf, short outcount, short *inbuf, short incount);

#endif
