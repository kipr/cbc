#ifndef	__SERVOS_H__
#define	__SERVOS_H__

void ServosInit(void);

void EnableServoPort(int port);
void EnableServos(void);
void DisableServoPort(int port);
void DisableServos(void);
void SetServoPosition(int port, int position);
int GetServoPosition(int port);
#endif
