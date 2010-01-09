/**************************************************************************
 *  Copyright 2008,2009 KISS Institute for Practical Robotics             *
 *                                                                        *
 *  This file is part of CBC Firmware.                                    *
 *                                                                        *
 *  CBC Firmware is free software: you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation, either version 2 of the License, or     *
 *  (at your option) any later version.                                   *
 *                                                                        *
 *  CBC Firmware is distributed in the hope that it will be useful,       *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *  GNU General Public License for more details.                          *
 *                                                                        *
 *  You should have received a copy of the GNU General Public License     *
 *  along with this copy of CBC Firmware.  Check the LICENSE file         *
 *  in the project root.  If not, see <http://www.gnu.org/licenses/>.     *
 **************************************************************************/

#include "CbobData.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

CbobData::CbobData()
{
    m_sensors = open("/dev/cbc/sensors", O_RDONLY);
    m_pid = open("/dev/cbc/pid", O_RDONLY);
    m_pwm = open("/dev/cbc/pwm", O_RDONLY);
    
    QObject::connect(&m_timer, SIGNAL(timeout()), this, SLOT(updateSensors()));
    
    updateSensors();
    setSlowRefresh();
}

CbobData::~CbobData()
{
    close(m_pid);
    close(m_sensors);
    close(m_pwm);
}

CbobData *CbobData::instance()
{
    static CbobData cbob_data;
    
    return &cbob_data;
}

int CbobData::analog(int port)
{
    return m_sensorData[port+1];
}

int CbobData::digital(int port)
{
    return (m_sensorData[0]&(1<<port)) && 1;
}

int CbobData::accelerometerX()
{
    return m_sensorData[10];
}

int CbobData::accelerometerY()
{
    return m_sensorData[11];
}

int CbobData::accelerometerZ()
{
    return m_sensorData[12];
}

float CbobData::batteryVoltage()
{
    //float volts = m_cbobData.shared().volts;
    
    //volts /= 4095.0;cpp
    
    //volts *= 8.4;
   return ((float)m_sensorData[9])/1000.0; 
}

int CbobData::motorVelocity(int motor)
{
    return 0;
}

int CbobData::motorPosition(int motor)
{
    if(motor >= 0 && motor <= 3)
      return m_pidData[motor];
    return 0;
}

int CbobData::analogPullups()
{
  return m_sensorData[13];
}


int CbobData::motorPWM(int motor)
{
	if(motor >= 0 && motor < 4)
		return m_pwmData[motor];
	return 0;
}

void CbobData::setRefresh(int delay)
{
	m_timer.start(delay);
}

void CbobData::setFastRefresh()
{
	setRefresh(CBOB_REFRESH_FAST);
}

void CbobData::setSlowRefresh()
{
	setRefresh(CBOB_REFRESH_SLOW);
}

void CbobData::updateSensors()
{
    int error;
    error = read(m_sensors, m_sensorData, 28);
    if(error < 0) perror("Got an error reading the sensor data");
    error = read(m_pid, m_pidData, 16);
    if(error < 0) perror("Got an error reading the motor counters");
    error = read(m_pwm, m_pwmData, 4);
    if(error < 0) perror("Got an error reading the motor pwm values");
    
    emit refresh();
}


