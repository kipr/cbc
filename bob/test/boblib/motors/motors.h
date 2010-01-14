#ifndef __MOTORS_H__
#define __MOTORS_H__

void UpdateBEMF(void);
void MotorsInit(void);
int MotorCalibration(void);
void MoveToPosition(int motor, int speed, int position);
void MoveAtVelocity(int motor, int speed);
void Motor(int motor, int power);
int GetMotor(int motor);
int GetMotorCounter(int motor);
int ClearMotorCounter(int motor);
int SetMotorCounter(int motor, int counter);
int BlockMotorDone(int motor);
int IsMotorDone(int motor);
void GetPIDGains(short num, short *PM, short *IM, short *DM, short *PD, short *ID, short *DD);
void SetPIDGains(int num, short PM, short IM, short DM, short PD, short ID, short DD);
void SetPIDGainsDefault(int num);
void SetPIDAllGains(short PM, short IM, short DM, short PD, short ID, short DD);
int GetMotorError(int motor);
int GetMotorTPC(int motor);
int GetTargetTPC(int motor);
#endif
