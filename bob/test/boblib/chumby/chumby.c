#include "chumby.h"

#include <pio/pio.h>
#include <usb/usb.h>
#include <stdio.h>

#include <sensors/sensors.h>
#include <motors/motors.h>
#include <accel/accel.h>
#include <uart/uart.h>
#include <servos/servos.h>
#include <string.h>

#include "cbob_cmd.h"
#include "chumby_spi.h"

//typedef unsigned short
typedef unsigned char uchar;

static Pin g_ChumbyPins[] = {CHUMBY};
static Pin g_ChumbyBend = CHUMBY_BEND;
static Pin g_ChumbySS1 = CHUMBY_SS1;
 
volatile short g_ChumbyCmd[CHUMBY_CMD_COUNT];
volatile short g_ChumbyData[CHUMBY_MAX_DATA_COUNT];
volatile uchar  g_ChumbyState = -1;

static void ChumbyCallback();

static void ChumbySetStateCmd();
static void ChumbySetStateData(short count);
static void ChumbyHandleCmd(short cmd, short *data, short count);
static void ChumbySetStateWriteData();
static void ChumbySetStateWriteLen(short count);

void ChumbyInit()
{
  PIO_Configure(g_ChumbyPins, PIO_LISTSIZE(g_ChumbyPins));
  
  ChumbySpiInit();
  ChumbySpiSetCallback(ChumbyCallback);
  
  ChumbySetStateCmd();
	ChumbyBend(0);
}

static void ChumbySetStateCmd()
{
  ChumbySpiReset();
  
  g_ChumbyState = CHUMBY_STATE_RXT_CMD;
  g_ChumbyCmd[0] = 0;
  g_ChumbyCmd[1] = 0;
  g_ChumbyCmd[2] = 0 ;
  
  ChumbySpiRead((void*)g_ChumbyCmd, CHUMBY_CMD_COUNT);
}

static void ChumbySetStateData(short count)
{
  ChumbySpiRead((void*)g_ChumbyData, count);
  
  g_ChumbyState = CHUMBY_STATE_RXT_DATA;
}

static void ChumbySetStateWriteLen(short count)
{
  if(count < 1) {
    count = 1;
    g_ChumbyData[1] = 0;
  }

  g_ChumbyData[0] = count;
  ChumbySpiWrite((void *)g_ChumbyData, 2);
  // BUG: Why does this only work with
  //      count = 2?  Something is broken
  //      When count = 1, ChumbyBend(0)
  //      Has no effect. WEIRD!
  
  g_ChumbyState = CHUMBY_STATE_TXT_LEN;
}

static void ChumbySetStateWriteData()
{
  // Also really weird. The TX circuitry on this chip
  // Is totally fubared.  Oh well
  ChumbySpiWrite((void *)&(g_ChumbyData[1]), g_ChumbyData[0]+1);
  
  g_ChumbyState = CHUMBY_STATE_TXT_DATA;
}

static void ChumbyHandleCmd(short cmd, short *data, short length)
{
  int tmp, outcount = 0;
	
	switch(cmd) {
    case CBOB_CMD_DIGITAL_READ:
      if(data[0] >= 0 && data[0] <= 7)
        g_ChumbyData[1] = Digital(data[0]);
      else if(data[0] == 8)
        g_ChumbyData[1] = BlackButton();
      else
        g_ChumbyData[1] = AllDigitals() | (BlackButton()<<8);
      outcount = 1;
			break;
		case CBOB_CMD_DIGITAL_WRITE:
			if(data[0] >= 0 && data[0] <= 7)
				DigitalOut(data[0], data[1]);
		case CBOB_CMD_DIGITAL_CONFIG:
			if(data[0] == 0) {
				//Set Direction
				DigitalConfig(data[1], data[2]);
			}
			else if(data[0] == 1) {
				//Get Direction
				g_ChumbyData[1] = GetDigitalConfig(data[1]);
				outcount = 1;
			}
			break;
    case CBOB_CMD_ANALOG_READ:
			if(data[0] >= 0 && data[0] < 8) {
				g_ChumbyData[1] = Analog(data[0]);
				outcount = 1;
			}
			else if(data[0] == 8) {
				g_ChumbyData[1] = BattVoltage();
			}
			else {
				for(outcount = 1;outcount <= 8;outcount++) {
					g_ChumbyData[outcount] = Analog(outcount-1);
				}
				g_ChumbyData[outcount] = BattVoltage();
			}
      break;
		case CBOB_CMD_ANALOG_CONFIG:
			if(data[0] == 0) {
				SetAnalogPullups(data[1]);
			}
			else if(data[0] == 1) {
				data[1] = GetAnalogPullups();
				outcount += 1;
			}
			break;
		case CBOB_CMD_ACCEL_READ:
			if(data[0] == 0) {
				g_ChumbyData[1] = Accel_X();
				outcount = 1;
			}
			else if(data[0] == 1) {
				g_ChumbyData[1] = Accel_Y();
				outcount = 1;
			}
			else if(data[0] == 2) {
				g_ChumbyData[1] = Accel_Z();
				outcount = 1;
			}
			else {
				g_ChumbyData[1] = Accel_X();
				g_ChumbyData[2] = Accel_Y();
				g_ChumbyData[3] = Accel_Z();
				outcount = 3;
			}
			break;
		case CBOB_CMD_ACCEL_CONFIG:
				if(data[0] == 0) {
					// Accelerometer Calibration
					AccelCalibration();
				}
			break;
    case CBOB_CMD_SENSORS_READ:
      g_ChumbyData[1] = AllDigitals() | (BlackButton()<<8);
      for(outcount = 2;outcount <= 9;outcount++) {
				g_ChumbyData[outcount] = Analog(outcount-2);
			}
			g_ChumbyData[outcount] = BattVoltage();
			g_ChumbyData[++outcount] = Accel_X();
			g_ChumbyData[++outcount] = Accel_Y();
			g_ChumbyData[++outcount] = Accel_Z();
			g_ChumbyData[++outcount] = GetAnalogPullups();
      break;
    case CBOB_CMD_SENSORS_CONFIG:
			break;
    case CBOB_CMD_PWM_READ:
			if(data[0] >= 0 && data[0] < 4) {
				data[1] = GetMotor(data[0]);
				outcount = 1;
			}
			else {
				data[1] = GetMotor(0);
				data[2] = GetMotor(1);
				data[3] = GetMotor(2);
				data[4] = GetMotor(3);
				outcount = 4;
			}
      break;
    case CBOB_CMD_PWM_WRITE:
      if(data[0] <= 3) {
        Motor(data[0], data[1]);
      }
      else {
        Motor(0, data[1]);
        Motor(1, data[2]);
        Motor(2, data[3]);
        Motor(3, data[4]);
      }
      break;
		case CBOB_CMD_PWM_CONFIG:
			break;
		case CBOB_CMD_PID_READ:
			if(data[0] <= 3) {
				tmp = GetMotorCounter(data[0]);
				memcpy(((void*)data)+2, &tmp, 4);
				outcount = 2;
			}
			else {
				tmp = GetMotorCounter(0);
				memcpy((void*)&data[1], &tmp, 4);
				tmp = GetMotorCounter(1);
				memcpy((void*)&data[3], &tmp, 4);
				tmp = GetMotorCounter(2);
				memcpy((void*)&data[5], &tmp, 4);
				tmp = GetMotorCounter(3);
				memcpy((void*)&data[7], &tmp, 4);
				outcount = 8;
			}
			break;
		case CBOB_CMD_PID_WRITE:
			if(data[0] >= 0 && data[0] < 4) {
				if(length > 2) {
					memcpy(&tmp, ((void*)data)+4, 4);
					MoveToPosition(data[0], data[1], tmp);
				}
				else {
					MoveAtVelocity(data[0], data[1]);
				}
			}
			break;
		case CBOB_CMD_PID_CONFIG:
			if(data[0] == 0) {
				// Clear Motor Counter
				if(data[1] >= 0 && data[1] <= 3)
					ClearMotorCounter(data[1]);
			}
			else if(data[0] == 1) {
				// Set motor posotion counter
				if(data[1] >= 0 && data[1] <= 3) {
					memcpy(&tmp, (void*)&data[2], 4);
					SetMotorCounter(data[1], tmp);
				}
			}
			else if(data[0] == 2) {
				// Set pid gains
				SetPIDGains(data[1],data[2],data[3],data[4],data[5],data[6], data[7]);
				
			}
			else if(data[0] == 3) {
				// Get pid gains
				GetPIDGains(data[1], data+1, data+2, data+3, data+4, data+5, data+6);
				outcount = 6;
			}
			else if(data[0] == 4) {
				// Get motor done
				if(data[1] >= 0 && data[1] <= 3) {
					data[1] = IsMotorDone(data[1]);
					outcount = 1;
				}
			}
			else if(data[0] == 5) {
				// Reset PID Gains
				SetPIDGainsDefault(data[1]);
			}
			else if(data[0] == 6) {
				// Recalibrate Motors
				MotorCalibration();
			}
			break;
		case CBOB_CMD_SERVO_READ:
			if(data[0] >= 0 && data[0] < 4) {
				data[1] = GetServoPosition(data[0]);
				outcount = 1;
			}
			else {
				data[1] = GetServoPosition(0);
				data[2] = GetServoPosition(1);
				data[3] = GetServoPosition(2);
				data[4] = GetServoPosition(3);
				outcount = 4;
			}
		case CBOB_CMD_SERVO_WRITE:
			if(data[0] >= 0 && data[0] < 4) {
				SetServoPosition(data[0], data[1]);
			}
			break;
		case CBOB_CMD_UART_READ:
			data[1] = UartRead(data[0], (char*)(&(data[2])), data[1]);
			outcount = (data[1]>>1)+2;
			break;
		case CBOB_CMD_UART_WRITE:
			data[1] = UartWrite(data[0], (char*)&(data[2]), data[1]);
			outcount += 1;
			break;
		case CBOB_CMD_UART_CONFIG:
		  UartSetSigmask(data[0]);
			/*if(data[0] == 0) {
				UartSetSigmask(data[1]);
			}
			else if(data[0] == 1) {
				data[1] = UartGetSigmask();
				outcount += 1;
			}
			else if(data[0] == 2) {
				UartReset(data[1]);
			}*/
			break;
		case CBOB_CMD_STATUS_READ:
			data[1] = CBOB_VERSION;
			outcount += 1;
		  break;
  }
  ChumbySetStateWriteLen(outcount);
}

void ChumbyBend(int value)
{
  if(value) {
    PIO_Set(&g_ChumbyBend);
  }
  else {
    PIO_Clear(&g_ChumbyBend);
  }
}

void ChumbySS1(int value)
{
	if(value) {
		PIO_Set(&g_ChumbySS1);
	}
	else {
		PIO_Clear(&g_ChumbySS1);
	}
	//printf("ChumbySS1(%d)\n\r", value);
}

static void ChumbyCallback()
{
	//printf("callback...\n");
	ChumbyBend(1);
  switch(g_ChumbyState) {
    case CHUMBY_STATE_RXT_CMD:
      if((ushort)g_ChumbyCmd[0] == CHUMBY_START) {
        ChumbySetStateData(g_ChumbyCmd[2]);
      }
      else {
        ChumbySetStateCmd();
      }
      break;
    case CHUMBY_STATE_RXT_DATA:
      ChumbyHandleCmd(g_ChumbyCmd[1], (short*)g_ChumbyData, g_ChumbyCmd[2]);
      break;
    case CHUMBY_STATE_TXT_DATA:
      ChumbySetStateCmd();
      break;
    case CHUMBY_STATE_TXT_LEN:
      ChumbySetStateWriteData();
      break;
  }
	ChumbyBend(0);
}
