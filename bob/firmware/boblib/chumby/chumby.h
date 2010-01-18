#ifndef __CHUMBY_H__
#define __CHUMBY_H__

#include <at91sam7a3/AT91SAM7A3.h>
#include <bob.h>

#define CHUMBY_START 0xCB07

#define CHUMBY_CMD_COUNT 3
#define CHUMBY_CMD_SIZE (CHUMBY_CMD_COUNT)

#define CHUMBY_MAX_DATA_COUNT 128
#define CHUMBY_MAX_DATA_SIZE  (CHUMBY_MAX_DATA_COUNT)

#define CHUMBY_STATE_RXT_CMD  0
#define CHUMBY_STATE_RXT_DATA 1
#define CHUMBY_STATE_TXT_LEN  2
#define CHUMBY_STATE_TXT_DATA 3

void ChumbyInit();
inline void ChumbyBend(int value);
inline void ChumbySS1(int value);

void ChumbyRun();

#endif
