#ifndef __TIMER_H__
#define __TIMER_H__


typedef void (*PITCallback)();

void PitInit(unsigned int msperiod);
unsigned int GetPITValueReset(void);
unsigned int GetPITValue(void);
int SetPitCallback(PITCallback c, int num);

void usleep(unsigned int usecs);

void TimerInit();
extern volatile unsigned int mseconds;

#endif
