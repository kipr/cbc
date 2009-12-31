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

#include "MotorTuning.h"

#include <assert.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include "../../../kernel/cbob/cbob.h"
#include <QMessageBox>

MotorTuning::MotorTuning(QWidget *parent) : Page(parent)
{   
    int i;
    char devname[32];
    setupUi(this);
    
    m_cbobData = CbobData::instance();

    QObject::connect(&m_timer, SIGNAL(timeout()), this, SLOT(updateCounters()));
    QObject::connect(ui_ClearButton, SIGNAL(pressed()), this, SLOT(clearMotorCounter()));
    QObject::connect(ui_GainComboBox, SIGNAL(activated(int)), this, SLOT(selectGain(int)));

    m_timer.start(100);
    m_motorNumber = 0;
    m_targetSpeed = 0;
    m_targetPosition = 0;

    ui_GainComboBox->addItem(tr("Proportional"));
    ui_GainComboBox->addItem(tr("Integral"));
    ui_GainComboBox->addItem(tr("Derivative"));
    ui_GainComboBox->setCurrentIndex(0);

    for(i=0;i<4;i++){
                sprintf(devname, "/dev/cbc/pid%d", i);
                m_pid[i] = ::open(devname, O_RDWR);
            }
            
    getGains(0);
   this->selectGain(0);
}

MotorTuning::~MotorTuning()
{
    int i;
    for(i = 0;i < 4;i++) ::close(m_pid[i]);
}

void MotorTuning::show()
{
   m_timer.start();
}

void MotorTuning::hide()
{
    if(ui_PlayButton->isChecked()) ui_PlayButton->toggle();
    m_timer.stop();
}

void MotorTuning::updateCounters()
{
    if(isVisible()){
        m_cbobData->updateSensors();
        ui_MotorPositionLabel->setText(QString::number(m_cbobData->motorPosition(m_motorNumber)));
    }
}

void MotorTuning::on_ui_MotorDecButton_clicked(bool)
{
    if(m_motorNumber == 0) m_motorNumber = 3;
    else m_motorNumber--;
    
    getGains(m_motorNumber);

    ui_MotorNumberLabel->setText(QString::number(m_motorNumber));
}

void MotorTuning::on_ui_MotorIncButton_clicked(bool)
{
    if(m_motorNumber == 3) m_motorNumber = 0;
    else m_motorNumber++;
    
    getGains(m_motorNumber);

    ui_MotorNumberLabel->setText(QString::number(m_motorNumber));
}

void MotorTuning::on_ui_NegCheck_stateChanged(int state)
{
    float gain;

    PIDgains[ui_GainComboBox->currentIndex()] = -PIDgains[ui_GainComboBox->currentIndex()];
    ui_MultLabel->setText(QString::number(PIDgains[ui_GainComboBox->currentIndex()]));

    gain = (float)PIDgains[ui_GainComboBox->currentIndex()]/(float)PIDgains[ui_GainComboBox->currentIndex()+3];
    ui_GainValueLabel->setText(QString::number(gain,'f',3));
}
void MotorTuning::selectGain(int index)
{
    float gain;

    ui_MultLabel->setText(QString::number(PIDgains[index]));
    if(PIDgains[index] < 0){
        ui_MultSlider->setValue(-PIDgains[index]);
        ui_NegCheck->setCheckState(Qt::Checked);
    }
    else{
        ui_MultSlider->setValue(PIDgains[index]);
        ui_NegCheck->setCheckState(Qt::Unchecked);
    }
    ui_DivLabel->setText(QString::number(PIDgains[index+3]));
    ui_DivSlider->setValue(PIDgains[index+3]);

    gain = (float)PIDgains[index]/(float)PIDgains[index+3];
    ui_GainValueLabel->setText(QString::number(gain,'f',3));
}

void MotorTuning::on_ui_MultSlider_valueChanged(int value)
{
    float gain;

    if(ui_NegCheck->isChecked()) value = -value;
    ui_MultLabel->setText(QString::number(value));
    PIDgains[ui_GainComboBox->currentIndex()] = value;
    gain = (float)value/(float)PIDgains[ui_GainComboBox->currentIndex()+3];
    ui_GainValueLabel->setText(QString::number(gain,'f',3));
}

void MotorTuning::on_ui_DivSlider_valueChanged(int value)
{
    float gain;

    ui_DivLabel->setText(QString::number(value));
    PIDgains[ui_GainComboBox->currentIndex()+3] = value;
    gain = (float)PIDgains[ui_GainComboBox->currentIndex()]/(float)value;
    ui_GainValueLabel->setText(QString::number(gain,'f',3));
}

void MotorTuning::on_ui_TargetSpeedLine_selectionChanged()
{
    int value;
    Keypad user_keypad(this);

    //ui_TargetSpeedLine->setStyleSheet("QLineEdit#ui_TargetSpeedLine{background-color:red}");
    user_keypad.exec();
    value = user_keypad.getValue();


    if(value < -1000 || value > 1000){
        QMessageBox::warning(this,
                            "Input Error",
                             "Value must be between 1000 and -1000\n",
                             QMessageBox::Ok,
                             QMessageBox::NoButton);
        value = 0;
    }
    m_targetSpeed = value;

    //ui_TargetSpeedLine->setStyleSheet("QLineEdit#ui_TargetSpeedLine{background-color:white}");
    ui_TargetSpeedLine->setText(QString::number(value));
}

void MotorTuning::on_ui_TargetPositionLine_selectionChanged()
{
    Keypad user_keypad(this);

    //ui_TargetPositionLine->setStyleSheet("QLineEdit#ui_TargetPositionLine{background-color:red}");
    user_keypad.exec();

    m_targetPosition = user_keypad.getValue();
    //ui_TargetPositionLine->setStyleSheet("QLineEdit#ui_TargetPositionLine{background-color:white}");
    ui_TargetPositionLine->setText(QString::number(user_keypad.getValue()));
}

void MotorTuning::on_ui_PlayButton_toggled(bool state)
{
    int i;
    QString value;
    short gains[6];

    ui_TargetSpeedLine->setEnabled(!state);
    ui_TargetPositionLine->setEnabled(!state);
    ui_MultSlider->setEnabled(!state);
    ui_DivSlider->setEnabled(!state);
    ui_NegCheck->setEnabled(!state);

    if(state){
        for(i=0;i<6;i++) gains[i] = PIDgains[i];
        ioctl(m_pid[0], CBOB_PID_SET_GAINS, gains);
        this->moveToPosition(m_motorNumber,m_targetSpeed,m_targetPosition);
    }
    else{
        this->moveAtVelocity(m_motorNumber,0);
    }
}


void MotorTuning::clearMotorCounter()
{
    ioctl(m_pid[m_motorNumber], CBOB_PID_CLEAR_COUNTER);
}

void MotorTuning::moveAtVelocity(int motor,int velocity)
{
    short v = velocity;
    write(m_pid[motor], &v, 2);
}

void MotorTuning::moveToPosition(int motor,int speed, int target_position)
{
        short v = speed;
        int p = target_position;
        char outdata[6];

        memcpy(outdata, &v, 2);
        memcpy(outdata+2, &p, 4);

        write(m_pid[motor], outdata, 6);
}

void MotorTuning::getGains(int motor)
{
  short gains[6];
  int i;
  ioctl(m_pid[motor], CBOB_PID_GET_GAINS, gains);
    for(i=0;i<6;i++) PIDgains[i] = gains[i];
}





