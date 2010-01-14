#ifndef __SENSORS_H__
#define __SENSORS_H__

void SensorsTestInit();
void SensorsInit();
void DigitalInit();

int BlackButton();

void DigitalPullup(int port, int state);
int AllDigitals();
int Digital(int port);
void DigitalOut(int port, int state);
void ADigitalOut(int port, int state);
void DigitalConfig(int port, int dir);
int GetDigitalConfig(int port);

void UpdateAnalogs(void);
int Analog(int port);
void AnalogPullup(int port, int state);
void SetAnalogPullups(short mask);
short GetAnalogPullups();

void SensorPowerOn(void);
void SensorPowerOff(void);
int SensorPowerStatus(void);

void SelectBattV(void);
void SelectAng0(void);

int BattVoltage(void);
int Charging();
unsigned char WritePullupData(unsigned int iaddress, char *bytes, unsigned int num);

#endif
