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

#ifndef __MOTOR_TUNE_H__
#define __MOTOR_TUNE_H__

#include "ui_MotorTune.h"
#include <QDialog>

#include "Cbob.h"

class MotorTune : public QDialog, private Ui::MotorTune
{
    Q_OBJECT

public:
    MotorTune(QWidget *parent = 0);
    ~MotorTune();

public slots:
    void updateValues();

    void on_ui_PM_valueChanged(int value);
    void on_ui_PD_valueChanged(int value);
    void on_ui_IM_valueChanged(int value);
    void on_ui_ID_valueChanged(int value);
    void on_ui_DM_valueChanged(int value);
    void on_ui_DD_valueChanged(int value);

    void on_ui_Velocity_valueChanged(int value);

    void on_ui_MotorSelect_valueChanged(int value);

protected:
    void calcPid();

private:
    int m_currentMotor;
    Cbob m_cbob;

    int m_pMul;
    int m_pDiv;
    int m_iMul;
    int m_iDiv;
    int m_dMul;
    int m_dDiv;
};

#endif
