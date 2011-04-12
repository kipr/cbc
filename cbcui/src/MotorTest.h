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
        void resetMotorCounter();
        void updateGUI();
        void allStop();
        void show();
        void hide();

        void on_ui_TargetSpeedPowerLine_selectionChanged();

        void on_ui_TargetPositionLine_selectionChanged();

        void on_ui_PlayButton_toggled(bool state);

        void on_ui_PowerRadio_clicked();
        void on_ui_VelocityRadio_clicked();
        void on_ui_PositionRadio_clicked();

private:
        CbobData *m_cbobData;

        int m_motorNumber;
        int m_targetPower[4];
        int m_targetSpeed[4];
        int m_targetPosition[4];
        int m_controlState[4];
        bool m_playState[4];
};

#endif
