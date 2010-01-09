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

#ifndef __MOTOR_TEST_H__
#define __MOTOR_TEST_H__

#include <QTimer>

#include "ui_MotorTest.h"
#include "Page.h"
#include "CbobData.h"
#include "Keypad.h"
#include "cbob.h"

class MotorTest : public Page, private Ui::MotorTest
{
    Q_OBJECT

public:
    MotorTest(QWidget *parent = 0);
    ~MotorTest();

public slots:
        void on_ui_MotorDecButton_clicked(bool checked = false);
        void on_ui_MotorIncButton_clicked(bool checked = false);
        void updateCounters();
        void show();
        void hide();
        //void closeMotorTest();
        //void raise();

        void clearMotorCounter();

        void on_ui_TargetSpeedPowerLine0_selectionChanged();
        void on_ui_TargetSpeedPowerLine1_selectionChanged();
        void on_ui_TargetSpeedPowerLine2_selectionChanged();
        void on_ui_TargetSpeedPowerLine3_selectionChanged();

        void on_ui_TargetPositionLine0_selectionChanged();
        void on_ui_TargetPositionLine1_selectionChanged();
        void on_ui_TargetPositionLine2_selectionChanged();
        void on_ui_TargetPositionLine3_selectionChanged();

        void on_ui_PlayButton0_toggled(bool state);
        void on_ui_PlayButton1_toggled(bool state);
        void on_ui_PlayButton2_toggled(bool state);
        void on_ui_PlayButton3_toggled(bool state);

        void on_ui_PowerRadio0_clicked();
        void on_ui_VelocityRadio0_clicked();
        void on_ui_PositionRadio0_clicked();

        void on_ui_PowerRadio1_clicked();
        void on_ui_VelocityRadio1_clicked();
        void on_ui_PositionRadio1_clicked();

        void on_ui_PowerRadio2_clicked();
        void on_ui_VelocityRadio2_clicked();
        void on_ui_PositionRadio2_clicked();

        void on_ui_PowerRadio3_clicked();
        void on_ui_VelocityRadio3_clicked();
        void on_ui_PositionRadio3_clicked();

private:
        CbobData *m_cbobData;

        int m_pid[4];
        int m_pwm[4];

        int m_motorNumber;
        int m_targetPower[4];
        int m_targetSpeed[4];
        int m_targetPosition[4];

        void moveMotorPower(int motor,int power);
        void moveToPosition(int motor,int speed,int target_position);
        void moveAtVelocity(int motor,int velocity);

};

#endif
