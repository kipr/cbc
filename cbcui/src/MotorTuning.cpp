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
#include "Keypad.h"
#include <QSettings>

MotorTuning::MotorTuning(QWidget *parent) : Page(parent)
{   
    setupUi(this);
    
    m_cbobData = CbobData::instance();

    QObject::connect(m_cbobData, SIGNAL(eStop()), this, SLOT(allStop()));
    QObject::connect(ui_ClearButton, SIGNAL(pressed()), this, SLOT(resetMotorCounter()));
    QObject::connect(ui_GainComboBox, SIGNAL(activated(int)), this, SLOT(GUIupdate()));

    m_motorNumber = 0;
    m_targetSpeed = 0;
    m_targetPosition = 0;

    ui_GainComboBox->addItem(tr("Proportional"));
    ui_GainComboBox->addItem(tr("Integral"));
    ui_GainComboBox->addItem(tr("Derivative"));
    ui_GainComboBox->setCurrentIndex(0);

    //m_cbobData->motorGains(m_motorNumber,PIDgains);

    this->readSettings();

    this->GUIupdate();
}

MotorTuning::~MotorTuning()
{
    m_cbobData->motorsOff();
}

void MotorTuning::show()
{
    m_cbobData->setFastRefresh();
    QObject::connect(m_cbobData, SIGNAL(refresh()), this, SLOT(updateCounters()));
    m_cbobData->motorGains(m_motorNumber,PIDgains);
    this->GUIupdate();
    this->motorCheckBoxes();
    Page::show();
}

void MotorTuning::hide()
{
    //qWarning("Tuning hidden");
    // turn off all motors before hiding page;
    //m_cbobData->motorsOff();
    //if(ui_PlayButton->isChecked()) ui_PlayButton->toggle();

    this->writeGains();
    m_cbobData->setSlowRefresh();
    QObject::disconnect(this, SLOT(updateCounters()));
    Page::hide();
}

void MotorTuning::updateCounters()
{
    ui_MotorPositionLabel->setText(QString::number(m_cbobData->motorPosition(m_motorNumber)));
}

void MotorTuning::resetMotorCounter()
{
    m_cbobData->clearMotorCounter(m_motorNumber);
}

void MotorTuning::allStop()
{
    if(ui_PlayButton->isChecked()) ui_PlayButton->toggle();
}

void MotorTuning::on_ui_MotorDecButton_clicked(bool)
{
    // write gains to bob from previous motor
    this->writeGains();

    if(m_motorNumber == 0) m_motorNumber = 3;
    else m_motorNumber--;
    
    // read gains from new motor
    m_cbobData->motorGains(m_motorNumber,PIDgains);

    this->GUIupdate();
    this->motorCheckBoxes();
}
void MotorTuning::on_ui_MotorIncButton_clicked(bool)
{
     // write gains to bob from previous motor
    this->writeGains();

    if(m_motorNumber == 3) m_motorNumber = 0;
    else m_motorNumber++;
    
    // read gains from new motor
    m_cbobData->motorGains(m_motorNumber,PIDgains);

    this->GUIupdate();
    this->motorCheckBoxes();
}

void MotorTuning::on_ui_NegCheck_stateChanged(int state)
{
    float gain;
    int index = ui_GainComboBox->currentIndex();
    if((state && PIDgains[index] > 0) || (!state && PIDgains[index] < 0)) PIDgains[index] = -PIDgains[index];

    ui_MultLabel->setText(QString::number(PIDgains[index]));
    gain = (float)PIDgains[index]/(float)PIDgains[index+3];
    ui_GainValueLabel->setText(QString::number(gain,'f',3));
}

void MotorTuning::on_ui_MultSlider_valueChanged(int value)
{
    float gain;
    int index = ui_GainComboBox->currentIndex();
    if(ui_NegCheck->isChecked()) value = -value;
    PIDgains[index] = value;

    ui_MultLabel->setText(QString::number(PIDgains[index]));
    gain = (float)PIDgains[index]/(float)PIDgains[index+3];
    ui_GainValueLabel->setText(QString::number(gain,'f',3));
}
void MotorTuning::on_ui_DivSlider_valueChanged(int value)
{
    float gain;
    int index = ui_GainComboBox->currentIndex();
    PIDgains[index+3] = value;

    ui_DivLabel->setText(QString::number(PIDgains[index+3]));
    gain = (float)PIDgains[index]/(float)PIDgains[index+3];
    ui_GainValueLabel->setText(QString::number(gain,'f',3));
}

void MotorTuning::on_ui_TargetSpeedLine_selectionChanged()
{
    Keypad user_keypad(this,-1000,1000);

    ui_TargetSpeedLine->setStyleSheet("QLineEdit#ui_TargetSpeedLine{background-color:red}");
    user_keypad.exec();
    m_targetSpeed = user_keypad.getValue();

    ui_TargetSpeedLine->setStyleSheet("QLineEdit#ui_TargetSpeedLine{background-color:white}");
    ui_TargetSpeedLine->setText(QString::number(m_targetSpeed));
}
void MotorTuning::on_ui_TargetPositionLine_selectionChanged()
{
    Keypad user_keypad(this,-2000000000,2000000000);
    ui_TargetPositionLine->setStyleSheet("QLineEdit#ui_TargetPositionLine{background-color:red}");
    user_keypad.exec();

    m_targetPosition = user_keypad.getValue();
    ui_TargetPositionLine->setStyleSheet("QLineEdit#ui_TargetPositionLine{background-color:white}");
    ui_TargetPositionLine->setText(QString::number(m_targetPosition));
}

void MotorTuning::on_ui_PlayButton_toggled(bool state)
{
    ui_TargetSpeedLine->setEnabled(!state);
    ui_TargetPositionLine->setEnabled(!state);
    ui_MultSlider->setEnabled(!state);
    ui_DivSlider->setEnabled(!state);
    ui_NegCheck->setEnabled(!state);

    if(state){
        // set the motor gains before running the motor
        this->writeGains();
        m_cbobData->moveToPosition(m_motorNumber,m_targetSpeed,m_targetPosition);
    }
    else{
        m_cbobData->moveAtVelocity(m_motorNumber,0);
    }
}

void MotorTuning::GUIupdate()
{
    float gain;
    int index = ui_GainComboBox->currentIndex();
    ui_MotorNumberLabel->setText(QString::number(m_motorNumber));

    ui_MultLabel->setText(QString::number(PIDgains[index]));
    ui_DivLabel->setText(QString::number(PIDgains[index+3]));

    gain = (float)PIDgains[index]/(float)PIDgains[index+3];
    ui_GainValueLabel->setText(QString::number(gain,'f',3));

    if(PIDgains[index] < 0){
        ui_MultSlider->setValue(-PIDgains[index]);
        ui_NegCheck->setCheckState(Qt::Checked);
        //ui_NegCheck->setDown(true);
    }
    else{
        ui_MultSlider->setValue(PIDgains[index]);
        ui_NegCheck->setCheckState(Qt::Unchecked);
        //ui_NegCheck->setDown(false);
    }
    ui_DivSlider->setValue(PIDgains[index+3]);
}

void MotorTuning::writeGains()
{
    if(ui_motorCheck0->isChecked()){
        m_cbobData->motorSetGains(0,PIDgains);
        this->writeSettings(0);
    }
    if(ui_motorCheck1->isChecked()){
        m_cbobData->motorSetGains(1,PIDgains);
        this->writeSettings(1);
    }
    if(ui_motorCheck2->isChecked()){
        m_cbobData->motorSetGains(2,PIDgains);
        this->writeSettings(2);
    }
    if(ui_motorCheck3->isChecked()){
        m_cbobData->motorSetGains(3,PIDgains);
        this->writeSettings(3);
    }
}

void MotorTuning::motorCheckBoxes()
{
    if(m_motorNumber == 0) ui_motorCheck0->setCheckState(Qt::Checked);
    else ui_motorCheck0->setCheckState(Qt::Unchecked);

    if(m_motorNumber == 1) ui_motorCheck1->setCheckState(Qt::Checked);
    else ui_motorCheck1->setCheckState(Qt::Unchecked);

    if(m_motorNumber == 2) ui_motorCheck2->setCheckState(Qt::Checked);
    else ui_motorCheck2->setCheckState(Qt::Unchecked);

    if(m_motorNumber == 3) ui_motorCheck3->setCheckState(Qt::Checked);
     else ui_motorCheck3->setCheckState(Qt::Unchecked);

}

void MotorTuning::readSettings()
{
    int i;
    QSettings m_settings("/mnt/user/cbc_v2.config",QSettings::NativeFormat);

    //qWarning("%s",qPrintable(m_settings.fileName()));
    // reads in the PID settings that have been saved to memory
    // if no settings file is located the defaults are input
    for(i=0;i<4;i++){
        m_settings.beginGroup(QString("PIDgainsMotor%1").arg(i));
        PIDgains[0] = m_settings.value("ProportionalMult",4).toInt();
        PIDgains[1] = m_settings.value("IntegralMult",1).toInt();
        PIDgains[2] = m_settings.value("DerivativeMult",-1).toInt();
        PIDgains[3] = m_settings.value("ProportionalDiv",1).toInt();
        PIDgains[4] = m_settings.value("IntegralDiv",1).toInt();
        PIDgains[5] = m_settings.value("DerivativeDiv",3).toInt();
        m_settings.endGroup();
        // write each of the saved settings to the BoB
        m_cbobData->motorSetGains(i,PIDgains);
    }
}

void MotorTuning::writeSettings(int motor)
{
    QSettings m_settings("/mnt/user/cbc_v2.config",QSettings::NativeFormat);

    m_settings.beginGroup(QString("PIDgainsMotor%1").arg(motor));
    m_settings.setValue("ProportionalMult",PIDgains[0]);
    m_settings.setValue("IntegralMult",PIDgains[1]);
    m_settings.setValue("DerivativeMult",PIDgains[2]);
    m_settings.setValue("ProportionalDiv",PIDgains[3]);
    m_settings.setValue("IntegralDiv",PIDgains[4]);
    m_settings.setValue("DerivativeDiv",PIDgains[5]);
    m_settings.endGroup();
    m_settings.sync();
    ::system("sync");
    ::system("sync");
}
