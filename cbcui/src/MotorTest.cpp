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

#include "MotorTest.h"

MotorTest::MotorTest(QWidget *parent) : Page(parent)
{
    int i;
    setupUi(this);

    m_cbobData = CbobData::instance();

    QObject::connect(m_cbobData, SIGNAL(eStop()), this, SLOT(allStop()));
    QObject::connect(ui_ClearButton, SIGNAL(pressed()), this, SLOT(resetMotorCounter()));

    m_motorNumber = 0;

    for(i=0;i<4;i++){
                m_targetPower[i] = 0;
                m_targetSpeed[i] = 0;
                m_targetPosition[i] = 0;
                m_controlState[i] = 2;
                m_playState[i] = false;
            }
}

MotorTest::~MotorTest()
{
    m_cbobData->motorsOff();
}

void MotorTest::show()
{
    m_cbobData->setFastRefresh();
    QObject::connect(m_cbobData, SIGNAL(refresh()), this, SLOT(updateCounters()));
    this->updateGUI();
    Page::show();
}

void MotorTest::hide()
{
    // stop all motors if the page is hidden
    // this->allStop();
    QObject::disconnect(this, SLOT(updateCounters()));
    m_cbobData->setSlowRefresh();
    Page::hide();
}

void MotorTest::updateCounters()
{
    ui_MotorPositionLabel->setText(QString::number(m_cbobData->motorPosition(m_motorNumber)));
}

void MotorTest::resetMotorCounter()
{
    m_cbobData->clearMotorCounter(m_motorNumber);
}

void MotorTest::updateGUI()
{
    ui_PowerRadio->setEnabled(true);
    ui_VelocityRadio->setEnabled(true);
    ui_PositionRadio->setEnabled(true);
    ui_TargetSpeedPowerLine->setEnabled(true);
    ui_TargetPositionLine->setEnabled(true);

    switch(m_controlState[m_motorNumber]){
        case 0:
        {
            ui_PowerRadio->click();
        }
        break;
        case 1:
        {
            ui_VelocityRadio->click();
        }
        break;
        case 2:
        {
            ui_PositionRadio->click();
        }
        break;
        default:
            return;
    }
}

void MotorTest::allStop()
{
    int i;
    m_cbobData->motorsOff();
    ui_PlayButton->setChecked(false);
    for(i=0;i<4;i++) m_playState[i] = false;
}

void MotorTest::on_ui_MotorDecButton_clicked(bool)
{
    if(m_motorNumber == 0) m_motorNumber = 3;
    else m_motorNumber--;

    ui_MotorNumberLabel->setText(QString::number(m_motorNumber));
    this->updateGUI();
}
void MotorTest::on_ui_MotorIncButton_clicked(bool)
{
    if(m_motorNumber == 3) m_motorNumber = 0;
    else m_motorNumber++;

    ui_MotorNumberLabel->setText(QString::number(m_motorNumber));
    this->updateGUI();
}

void MotorTest::on_ui_PositionRadio_clicked()
{
    m_controlState[m_motorNumber] = 2;
    ui_TargetPositionLabel->show();
    ui_TargetPositionLine->show();
    ui_TargetSpeedPowerLabel->setText("Target Speed");
    ui_TargetSpeedPowerLine->setText(QString::number(m_targetSpeed[m_motorNumber]));
    ui_TargetPositionLine->setText(QString::number(m_targetPosition[m_motorNumber]));
    ui_PlayButton->setChecked(m_playState[m_motorNumber]);
}
void MotorTest::on_ui_VelocityRadio_clicked()
{
    m_controlState[m_motorNumber] = 1;
    ui_TargetPositionLabel->hide();
    ui_TargetPositionLine->hide();
    ui_TargetSpeedPowerLabel->setText("Target Speed");
    ui_TargetSpeedPowerLine->setText(QString::number(m_targetSpeed[m_motorNumber]));
    ui_PlayButton->setChecked(m_playState[m_motorNumber]);
}
void MotorTest::on_ui_PowerRadio_clicked()
{
    m_controlState[m_motorNumber] = 0;
    ui_TargetPositionLabel->hide();
    ui_TargetPositionLine->hide();
    ui_TargetSpeedPowerLabel->setText("Target Power");
    ui_TargetSpeedPowerLine->setText(QString::number(m_targetPower[m_motorNumber]));
    ui_PlayButton->setChecked(m_playState[m_motorNumber]);
}

void MotorTest::on_ui_TargetSpeedPowerLine_selectionChanged()
{
    int value;
    Keypad user_keypad(this,-1000,1000);

    if(m_controlState[m_motorNumber] == 0)
        user_keypad.setRange(-100,100);


    ui_TargetSpeedPowerLine->setStyleSheet("QLineEdit#ui_TargetSpeedPowerLine0{background-color:red}");
    user_keypad.exec();
    value = user_keypad.getValue();

   if(m_controlState[m_motorNumber] == 0){
        m_targetPower[m_motorNumber] = value;
    }
    else{
        m_targetSpeed[m_motorNumber] = value;
    }

    ui_TargetSpeedPowerLine->setStyleSheet("QLineEdit#ui_TargetSpeedPowerLine0{background-color:white}");
    ui_TargetSpeedPowerLine->setText(QString::number(value));
}
void MotorTest::on_ui_TargetPositionLine_selectionChanged()
{
    Keypad user_keypad(this,-2000000000,2000000000);

    ui_TargetPositionLine->setStyleSheet("QLineEdit#ui_TargetPositionLine0{background-color:red}");
    user_keypad.exec();

    m_targetPosition[m_motorNumber] = user_keypad.getValue();
    ui_TargetPositionLine->setStyleSheet("QLineEdit#ui_TargetPositionLine0{background-color:white}");
    ui_TargetPositionLine->setText(QString::number(user_keypad.getValue()));
}

void MotorTest::on_ui_PlayButton_toggled(bool state)
{
    ui_PowerRadio->setEnabled(!state);
    ui_VelocityRadio->setEnabled(!state);
    ui_PositionRadio->setEnabled(!state);
    ui_TargetSpeedPowerLine->setEnabled(!state);
    ui_TargetPositionLine->setEnabled(!state);

    if(m_playState[m_motorNumber] == state) return;
    m_playState[m_motorNumber] = state;

    if(state){
        switch(m_controlState[m_motorNumber]){
            case 0:
            {
                m_cbobData->moveMotorPower(m_motorNumber,m_targetPower[m_motorNumber]);
            }
            break;
            case 1:
            {
                m_cbobData->moveAtVelocity(m_motorNumber,m_targetSpeed[m_motorNumber]);
            }
            break;
            case 2:
            {
                m_cbobData->moveToPosition(m_motorNumber,m_targetSpeed[m_motorNumber],m_targetPosition[m_motorNumber]);
            }
            break;
            default:
                return;
        }
    }
    else{
        m_cbobData->moveAtVelocity(m_motorNumber,0);
    }
}
