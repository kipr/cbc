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

CbobData::CbobData()
{
    m_sensors = open("/dev/cbc/sensors", O_RDONLY);
}

CbobData::~CbobData()
{
    close(m_sensors);
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
    return 0;
}

int CbobData::accelerometerY()
{
    return 0;
}

int CbobData::accelerometerZ()
{
    return 0;
}

float CbobData::batteryVoltage()
{
    //float volts = m_cbobData.shared().volts;
    
    //volts /= 4095.0;cpp
    
    //volts *= 8.4;
    
    return 0;
}

int CbobData::motorVelocity(int motor)
{
    return 0;
}

int CbobData::motorPosition(int motor)
{
    return 0;
}


int CbobData::motorPWM(int motor)
{
    return 0;
}

void CbobData::updateSensors()
{
    read(m_sensors, m_sensorData, 18);
}


