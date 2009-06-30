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

#ifndef __CBOB_DATA_H__
#define __CBOB_DATA_H__

#include <QObject>
#include "SharedMem.h"
#include "cbc_data.h"

class CbobData : public QObject
{
    Q_OBJECT

public:
    CbobData();
    ~CbobData();

    /* Sensor inputs */
    int analog(int port);
    int digital(int port);
    int accelerometerX();
    int accelerometerY();
    int accelerometerZ();

    float batteryVoltage();

    int motorPosition(int motor);
    int motorVelocity(int motor);
    int motorPWM(int motor);

    void updateSensors();
private:
    int m_sensors;
    int m_pid;
    int m_pwm;
    short m_sensorData[13];
    int   m_pidData[4];
    signed char   m_pwmData[4];

};

#endif
