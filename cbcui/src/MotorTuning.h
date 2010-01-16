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

#ifndef __MOTOR_TUNING_H__
#define __MOTOR_TUNING_H__

#include "ui_MotorTuning.h"
#include "Page.h"

#include <QDialog>
#include <QTimer>

#include "CbobData.h"

class MotorTuning : public Page, private Ui::MotorTuning
{
    Q_OBJECT

public:
    MotorTuning(QWidget *parent = 0);
    ~MotorTuning();

public slots:
    void on_ui_MotorDecButton_clicked(bool checked = false);
    void on_ui_MotorIncButton_clicked(bool checked = false);
    void updateCounters();
    void resetMotorCounter();
    void allStop();
    void show();
    void hide();

    void on_ui_NegCheck_stateChanged(int state);
    void on_ui_MultSlider_valueChanged(int value);
    void on_ui_DivSlider_valueChanged(int value);

    void on_ui_TargetSpeedLine_selectionChanged();
    void on_ui_TargetPositionLine_selectionChanged();
    void on_ui_PlayButton_toggled(bool state);
    void GUIupdate();

protected:
    void writeGains();
    void motorCheckBoxes();
    void readSettings();
    void writeSettings(int motor);

private:
    CbobData *m_cbobData;

    int m_motorNumber;
    int m_targetSpeed;
    int m_targetPosition;
    int PIDgains[6];
};

#endif
