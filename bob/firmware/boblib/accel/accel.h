


unsigned char AccelInit(void);
void AccelCalibration(void);
void GetAccelCal(short *calVal);
void SetAccelCal(short *calVal);
void AccelSetCalibration(void);

int Accel_X(void);
int Accel_Y(void);
int Accel_Z(void);

#ifdef STACCEL
int Accel_X_ST(void);
int Accel_Y_ST(void);
int Accel_Z_ST(void);
#endif

unsigned char AccelSetScale(int gsel);

unsigned char WriteAccelData(unsigned int iaddress, char *bytes, unsigned int num);
unsigned char ReadAccelData(unsigned int iaddress, char *bytes, unsigned int num);

