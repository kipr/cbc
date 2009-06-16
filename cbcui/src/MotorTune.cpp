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

#include "MotorTune.h"

MotorTune::MotorTune(QWidget *parent) : QDialog(parent), m_currentMotor(0)
{
    setupUi(this);

#ifdef QT_ARCH_ARM
    setWindowState(windowState() | Qt::WindowFullScreen);
#endif

    m_cbob.start();

    m_pMul = ui_PM->value();
    m_pDiv = ui_PD->value();
    m_iMul = ui_IM->value();
    m_iDiv = ui_ID->value();
    m_dMul = ui_DM->value();
    m_dDiv = ui_DD->value();

    //calcPid();

    QObject::connect(&m_cbob, SIGNAL(refresh()), this, SLOT(updateValues()));
}

MotorTune::~MotorTune()
{
    m_cbob.stop();
}

void MotorTune::updateValues()
{
    ui_MotorPower->setText(QString::number(m_cbob.getPWM(m_currentMotor)));
    ui_MotorPosition->setText(QString::number(m_cbob.getPosition(m_currentMotor)));
    ui_MotorTPS->setText(QString::number(m_cbob.getVelocity(m_currentMotor)));
}

void MotorTune::on_ui_PM_valueChanged(int value)
{
    m_pMul = value;
    m_cbob.setGain(m_currentMotor,0,value);
    //calcPid();
}

void MotorTune::on_ui_PD_valueChanged(int value)
{
    m_pDiv = value;
    m_cbob.setGain(m_currentMotor,3,value);
    //calcPid();
}

void MotorTune::on_ui_IM_valueChanged(int value)
{
    m_iMul = value;
    m_cbob.setGain(m_currentMotor,1,value);
// calcPid();
}

void MotorTune::on_ui_ID_valueChanged(int value)
{
    m_iDiv = value;
    m_cbob.setGain(m_currentMotor,4,value);
// calcPid();
}

void MotorTune::on_ui_DM_valueChanged(int value)
{
    m_dMul = value;
    m_cbob.setGain(m_currentMotor,2,value);
// calcPid();
}

void MotorTune::on_ui_DD_valueChanged(int value)
{
    m_dDiv = value;
    m_cbob.setGain(m_currentMotor,5,value);
    //calcPid();
}

void MotorTune::calcPid()
{
    double p,i,d;

    p = (double)m_pMul/(double)m_pDiv;
    i = (double)m_iMul/(double)m_iDiv;
    d = (double)m_dMul/(double)m_dDiv;

    ui_P->setText(QString::number(p));
    ui_I->setText(QString::number(i));
    ui_D->setText(QString::number(d));
}

void MotorTune::on_ui_Velocity_valueChanged(int value)
{
    m_cbob.setVelocity(m_currentMotor, value);

    ui_MotorTargetTPS->setText(QString::number(value));
}

void MotorTune::on_ui_MotorSelect_valueChanged(int value)
{
    m_currentMotor = value;
}

