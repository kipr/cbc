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
#include <QTimer>

#define CBOB_REFRESH_FAST 100
#define CBOB_REFRESH_SLOW 2000

class CbobData : public QObject
{
    Q_OBJECT

public:

    static CbobData *instance();

    /* Sensor inputs */
    int analog(int port);
    int digital(int port);
    int accelerometerX();
    int accelerometerY();
    int accelerometerZ();
    void accelerometerRecalibrate();
    void accelerometerSetCal(short *calibration);
    void accelerometerGetCal(short *calibration);
    int analogPullups();

    float batteryVoltage();

    // motors
    int motorPosition(int motor);
    int motorVelocity(int motor);
    int motorPWM(int motor);
    void motorsRecalibrate();
    void motorsGetCal(short *calibration);
    void motorsSetCal(short *calibration);

    void motorGains(int motor,int *gains);
    void motorSetGains(int motor,int *gains);
    void moveMotorPower(int motor,int power);
    void moveAtVelocity(int motor,int velocity);
    void moveToPosition(int motor,int speed,int target_position);
    void clearMotorCounter(int motor);
    void defaultPIDgains(int motor);

    // servos
    void setServoPosition(int servo, int pos);
    int getServoPosition(int servo);

    void setFastRefresh();
    void setSlowRefresh();
    void setRefresh(int delay);

signals:
    void refresh();
    void lowBattery(float volts);

public slots:
    void resetPullups();
    void motorsOff();
    void disableServos();
    void allStop();

signals:
    void eStop();

protected:
    CbobData();
    ~CbobData();

protected slots:
    void updateSensors();

private:
    int m_sensors;
    int m_angPullups;
    int m_allPID;
    int m_pid[4];
    int m_allPWM;
    int m_pwm[4];
    int m_servo[4];
    short m_sensorData[14];
    int   m_pidData[4];
    signed char   m_pwmData[4];
    QTimer m_timer;

    //static CbobData *inst;
};

#endif
