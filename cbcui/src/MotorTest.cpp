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
#include <QMessageBox>

MotorTest::MotorTest(QWidget *parent) : Page(parent)
{
    int i;
    setupUi(this);

    m_cbobData = CbobData::instance();

    QObject::connect(&m_timer, SIGNAL(timeout()), this, SLOT(updateCounters()));
    QObject::connect(m_cbobData, SIGNAL(eStop()), this, SLOT(allStop()));
    QObject::connect(ui_ClearButton0, SIGNAL(pressed()), this, SLOT(resetMotorCounter()));
    QObject::connect(ui_ClearButton1, SIGNAL(pressed()), this, SLOT(resetMotorCounter()));
    QObject::connect(ui_ClearButton2, SIGNAL(pressed()), this, SLOT(resetMotorCounter()));
    QObject::connect(ui_ClearButton3, SIGNAL(pressed()), this, SLOT(resetMotorCounter()));

    m_timer.start(100);
    m_motorNumber = 0;
     ui_MotorStack->setCurrentIndex(m_motorNumber);

    for(i=0;i<4;i++){
                m_targetPower[i] = 0;
                m_targetSpeed[i] = 0;
                m_targetPosition[i] = 0;
            }
}

MotorTest::~MotorTest()
{
    m_cbobData->motorsOff();
}

void MotorTest::show()
{
    //qWarning("Test shown");
    m_timer.start(100);
    Page::show();
}

void MotorTest::hide()
{
    //qWarning("Test hidden");
    // stop all motors if the page is hidden
    //m_cbobData->motorsOff();
    m_timer.stop();
    Page::hide();
}

void MotorTest::updateCounters()
{
    if(isVisible()) {
        m_cbobData->updateSensors();
        ui_MotorPositionLabel0->setText(QString::number(m_cbobData->motorPosition(0)));
        ui_MotorPositionLabel1->setText(QString::number(m_cbobData->motorPosition(1)));
        ui_MotorPositionLabel2->setText(QString::number(m_cbobData->motorPosition(2)));
        ui_MotorPositionLabel3->setText(QString::number(m_cbobData->motorPosition(3)));
    }
}

void MotorTest::resetMotorCounter()
{
    m_cbobData->clearMotorCounter(m_motorNumber);
}

void MotorTest::allStop()
{
    if(ui_PlayButton0->isChecked()) ui_PlayButton0->toggle();
    if(ui_PlayButton1->isChecked()) ui_PlayButton1->toggle();
    if(ui_PlayButton2->isChecked()) ui_PlayButton2->toggle();
    if(ui_PlayButton3->isChecked()) ui_PlayButton3->toggle();
}

void MotorTest::on_ui_MotorDecButton_clicked(bool)
{
    if(m_motorNumber == 0) m_motorNumber = 3;
    else m_motorNumber--;

    ui_MotorNumberLabel->setText(QString::number(m_motorNumber));
    ui_MotorStack->setCurrentIndex(m_motorNumber);
}
void MotorTest::on_ui_MotorIncButton_clicked(bool)
{
    if(m_motorNumber == 3) m_motorNumber = 0;
    else m_motorNumber++;

    ui_MotorNumberLabel->setText(QString::number(m_motorNumber));
    ui_MotorStack->setCurrentIndex(m_motorNumber);
}

void MotorTest::on_ui_PositionRadio0_clicked()
{
        ui_TargetPositionLabel0->show();
        ui_TargetPositionLine0->show();
        ui_TargetSpeedPowerLabel0->setText(QString::QString("Target Speed"));
        ui_TargetSpeedPowerLine0->setText(QString::number(m_targetSpeed[0]));
        ui_TargetPositionLine0->setText(QString::number(m_targetPosition[0]));
}
void MotorTest::on_ui_VelocityRadio0_clicked()
{
        ui_TargetPositionLabel0->hide();
        ui_TargetPositionLine0->hide();
        ui_TargetSpeedPowerLabel0->setText(QString::QString("Target Speed"));
        ui_TargetSpeedPowerLine0->setText(QString::number(m_targetSpeed[0]));
}
void MotorTest::on_ui_PowerRadio0_clicked()
{
        ui_TargetPositionLabel0->hide();
        ui_TargetPositionLine0->hide();
        ui_TargetSpeedPowerLabel0->setText(QString::QString("Target Power"));
        ui_TargetSpeedPowerLine0->setText(QString::number(m_targetPower[0]));
}

void MotorTest::on_ui_PositionRadio1_clicked()
{
        ui_TargetPositionLabel1->show();
        ui_TargetPositionLine1->show();
        ui_TargetSpeedPowerLabel1->setText(QString::QString("Target Speed"));
        ui_TargetSpeedPowerLine1->setText(QString::number(m_targetSpeed[1]));
        ui_TargetPositionLine1->setText(QString::number(m_targetPosition[1]));
}
void MotorTest::on_ui_VelocityRadio1_clicked()
{
        ui_TargetPositionLabel1->hide();
        ui_TargetPositionLine1->hide();
        ui_TargetSpeedPowerLabel1->setText(QString::QString("Target Speed"));
        ui_TargetSpeedPowerLine1->setText(QString::number(m_targetSpeed[1]));
}
void MotorTest::on_ui_PowerRadio1_clicked()
{
        ui_TargetPositionLabel1->hide();
        ui_TargetPositionLine1->hide();
        ui_TargetSpeedPowerLabel1->setText(QString::QString("Target Power"));
        ui_TargetSpeedPowerLine1->setText(QString::number(m_targetPower[1]));
}

void MotorTest::on_ui_PositionRadio2_clicked()
{
        ui_TargetPositionLabel2->show();
        ui_TargetPositionLine2->show();
        ui_TargetSpeedPowerLabel2->setText(QString::QString("Target Speed"));
        ui_TargetSpeedPowerLine2->setText(QString::number(m_targetSpeed[2]));
        ui_TargetPositionLine2->setText(QString::number(m_targetPosition[2]));
}
void MotorTest::on_ui_VelocityRadio2_clicked()
{
        ui_TargetPositionLabel2->hide();
        ui_TargetPositionLine2->hide();
        ui_TargetSpeedPowerLabel2->setText(QString::QString("Target Speed"));
        ui_TargetSpeedPowerLine2->setText(QString::number(m_targetSpeed[2]));
}
void MotorTest::on_ui_PowerRadio2_clicked()
{
        ui_TargetPositionLabel2->hide();
        ui_TargetPositionLine2->hide();
        ui_TargetSpeedPowerLabel2->setText(QString::QString("Target Power"));
        ui_TargetSpeedPowerLine2->setText(QString::number(m_targetPower[2]));
}

void MotorTest::on_ui_PositionRadio3_clicked()
{
        ui_TargetPositionLabel3->show();
        ui_TargetPositionLine3->show();
        ui_TargetSpeedPowerLabel3->setText(QString::QString("Target Speed"));
        ui_TargetSpeedPowerLine3->setText(QString::number(m_targetSpeed[3]));
        ui_TargetPositionLine3->setText(QString::number(m_targetPosition[3]));
}
void MotorTest::on_ui_VelocityRadio3_clicked()
{
        ui_TargetPositionLabel3->hide();
        ui_TargetPositionLine3->hide();
        ui_TargetSpeedPowerLabel3->setText(QString::QString("Target Speed"));
        ui_TargetSpeedPowerLine3->setText(QString::number(m_targetSpeed[3]));
}
void MotorTest::on_ui_PowerRadio3_clicked()
{
        ui_TargetPositionLabel3->hide();
        ui_TargetPositionLine3->hide();
        ui_TargetSpeedPowerLabel3->setText(QString::QString("Target Power"));
        ui_TargetSpeedPowerLine3->setText(QString::number(m_targetPower[3]));
}

void MotorTest::on_ui_TargetSpeedPowerLine0_selectionChanged()
{
    int value;
    Keypad user_keypad(this);

    //ui_TargetSpeedPowerLine0->setStyleSheet("QLineEdit#ui_TargetSpeedPowerLine0{background-color:red}");
    user_keypad.exec();
    value = user_keypad.getValue();

    if(ui_PowerRadio0->isChecked()){
        if(value < -100 || value > 100) {
            QMessageBox::warning(this,
                                "Input Error",
                                "Value must be between 100 and -100\n",
                                QMessageBox::Ok,
                                QMessageBox::NoButton);
            value = 0;
        }
        m_targetPower[m_motorNumber] = value;
    }
    else{
        if(value < -1000 || value > 1000) {
            QMessageBox::warning(this,
                            "Input Error",
                             "Value must be between 1000 and -1000\n",
                             QMessageBox::Ok,
                             QMessageBox::NoButton);
            value = 0;
        }
        m_targetSpeed[m_motorNumber] = value;
    }

    //ui_TargetSpeedPowerLine0->setStyleSheet("QLineEdit#ui_TargetSpeedPowerLine0{background-color:white}");
    ui_TargetSpeedPowerLine0->setText(QString::number(value));
}
void MotorTest::on_ui_TargetSpeedPowerLine1_selectionChanged()
{
    int value;
    Keypad user_keypad(this);

    //ui_TargetSpeedPowerLine1->setStyleSheet("QLineEdit#ui_TargetSpeedPowerLine1{background-color:red}");
    user_keypad.exec();
    value = user_keypad.getValue();

    if(ui_PowerRadio1->isChecked()){
        if(value < -100 || value > 100) {
            QMessageBox::warning(this,
                            "Input Error",
                             "Value must be between 100 and -100\n",
                             QMessageBox::Ok,
                             QMessageBox::NoButton);
            value = 0;
        }
        m_targetPower[m_motorNumber] = value;
    }
    else{
        if(value < -1000 || value > 1000) {
            QMessageBox::warning(this,
                            "Input Error",
                             "Value must be between 1000 and -1000\n",
                             QMessageBox::Ok,
                             QMessageBox::NoButton);
            value = 0;
        }
        m_targetSpeed[m_motorNumber] = value;
    }

    //ui_TargetSpeedPowerLine1->setStyleSheet("QLineEdit#ui_TargetSpeedPowerLine1{background-color:white}");
    ui_TargetSpeedPowerLine1->setText(QString::number(value));
}
void MotorTest::on_ui_TargetSpeedPowerLine2_selectionChanged()
{
    int value;
    Keypad user_keypad(this);

    //ui_TargetSpeedPowerLine2->setStyleSheet("QLineEdit#ui_TargetSpeedPowerLine2{background-color:red}");
    user_keypad.exec();
    value = user_keypad.getValue();

    if(ui_PowerRadio2->isChecked()){
        if(value < -100 || value > 100) {
            QMessageBox::warning(this,
                            "Input Error",
                             "Value must be between 100 and -100\n",
                             QMessageBox::Ok,
                             QMessageBox::NoButton);
            value = 0;
        }
        m_targetPower[m_motorNumber] = value;
    }
    else{
        if(value < -1000 || value > 1000) {
            QMessageBox::warning(this,
                            "Input Error",
                             "Value must be between 1000 and -1000\n",
                             QMessageBox::Ok,
                             QMessageBox::NoButton);
            value = 0;
        }
        m_targetSpeed[m_motorNumber] = value;
    }

    //ui_TargetSpeedPowerLine2->setStyleSheet("QLineEdit#ui_TargetSpeedPowerLine2{background-color:white}");
    ui_TargetSpeedPowerLine2->setText(QString::number(value));
}
void MotorTest::on_ui_TargetSpeedPowerLine3_selectionChanged()
{
    int value;
    Keypad user_keypad(this);

    //ui_TargetSpeedPowerLine3->setStyleSheet("QLineEdit#ui_TargetSpeedPowerLine3{background-color:red}");
    user_keypad.exec();
    value = user_keypad.getValue();

    if(ui_PowerRadio3->isChecked()){
        if(value < -100 || value > 100) {
            QMessageBox::warning(this,
                            "Input Error",
                             "Value must be between 100 and -100\n",
                             QMessageBox::Ok,
                             QMessageBox::NoButton);
            value = 0;
        }
        m_targetPower[m_motorNumber] = value;
    }
    else{
        if(value < -1000 || value > 1000) {
            QMessageBox::warning(this,
                            "Input Error",
                             "Value must be between 1000 and -1000\n",
                             QMessageBox::Ok,
                             QMessageBox::NoButton);
            value = 0;
        }
        m_targetSpeed[m_motorNumber] = value;
    }

    //ui_TargetSpeedPowerLine3->setStyleSheet("QLineEdit#ui_TargetSpeedPowerLine3{background-color:white}");
    ui_TargetSpeedPowerLine3->setText(QString::number(value));
}

void MotorTest::on_ui_TargetPositionLine0_selectionChanged()
{
    Keypad user_keypad(this);

    //ui_TargetPositionLine0->setStyleSheet("QLineEdit#ui_TargetPositionLine0{background-color:red}");
    user_keypad.exec();

    m_targetPosition[m_motorNumber] = user_keypad.getValue();
    //ui_TargetPositionLine0->setStyleSheet("QLineEdit#ui_TargetPositionLine0{background-color:white}");
    ui_TargetPositionLine0->setText(QString::number(user_keypad.getValue()));
}
void MotorTest::on_ui_TargetPositionLine1_selectionChanged()
{
    Keypad user_keypad(this);

    //ui_TargetPositionLine1->setStyleSheet("QLineEdit#ui_TargetPositionLine1{background-color:red}");
    user_keypad.exec();

    m_targetPosition[m_motorNumber] = user_keypad.getValue();
    //ui_TargetPositionLine1->setStyleSheet("QLineEdit#ui_TargetPositionLine1{background-color:white}");
    ui_TargetPositionLine1->setText(QString::number(user_keypad.getValue()));
}
void MotorTest::on_ui_TargetPositionLine2_selectionChanged()
{
    Keypad user_keypad(this);

    //ui_TargetPositionLine2->setStyleSheet("QLineEdit#ui_TargetPositionLine2{background-color:red}");
    user_keypad.exec();

    m_targetPosition[m_motorNumber] = user_keypad.getValue();
    //ui_TargetPositionLine2->setStyleSheet("QLineEdit#ui_TargetPositionLine2{background-color:white}");
    ui_TargetPositionLine2->setText(QString::number(user_keypad.getValue()));
}
void MotorTest::on_ui_TargetPositionLine3_selectionChanged()
{
    Keypad user_keypad(this);

    //ui_TargetPositionLine3->setStyleSheet("QLineEdit#ui_TargetPositionLine3{background-color:red}");
    user_keypad.exec();

    m_targetPosition[m_motorNumber] = user_keypad.getValue();
    //ui_TargetPositionLine3->setStyleSheet("QLineEdit#ui_TargetPositionLine3{background-color:white}");
    ui_TargetPositionLine3->setText(QString::number(user_keypad.getValue()));
}

void MotorTest::on_ui_PlayButton0_toggled(bool state)
{
    QString value;

    ui_PowerRadio0->setEnabled(!state);
    ui_VelocityRadio0->setEnabled(!state);
    ui_PositionRadio0->setEnabled(!state);
    ui_TargetSpeedPowerLine0->setEnabled(!state);
    ui_TargetPositionLine0->setEnabled(!state);

    if(state){
        //ui_PlayButton0->setText("Stop");
        value = ui_TargetSpeedPowerLine0->text();
        if(ui_PowerRadio0->isChecked()){
            m_targetPower[m_motorNumber] = value.toInt();
            m_cbobData->moveMotorPower(m_motorNumber,m_targetPower[m_motorNumber]);
        }
        else{
            m_targetSpeed[m_motorNumber] = value.toInt();
            value = ui_TargetPositionLine0->text();
            if(ui_VelocityRadio0->isChecked()){
                m_cbobData->moveAtVelocity(m_motorNumber,m_targetSpeed[m_motorNumber]);
            }
            else{
                m_targetPosition[m_motorNumber] = value.toInt();
                m_cbobData->moveToPosition(m_motorNumber,m_targetSpeed[m_motorNumber],m_targetPosition[m_motorNumber]);
            }
        }

    }
    else{
        //ui_PlayButton0->setText("Go");
        //m_cbobData->moveMotorPower(0,0);
        m_cbobData->moveAtVelocity(0,0);
    }
}
void MotorTest::on_ui_PlayButton1_toggled(bool state)
{
    QString value;

    ui_PowerRadio1->setEnabled(!state);
    ui_VelocityRadio1->setEnabled(!state);
    ui_PositionRadio1->setEnabled(!state);
    ui_TargetSpeedPowerLine1->setEnabled(!state);
    ui_TargetPositionLine1->setEnabled(!state);

    if(state){
        //ui_PlayButton1->setText("Stop");
        value = ui_TargetSpeedPowerLine1->text();
        if(ui_PowerRadio1->isChecked()){
            m_targetPower[m_motorNumber] = value.toInt();
            m_cbobData->moveMotorPower(m_motorNumber,m_targetPower[m_motorNumber]);
        }
        else{
            m_targetSpeed[m_motorNumber] = value.toInt();
            value = ui_TargetPositionLine1->text();
            if(ui_VelocityRadio1->isChecked()){
                m_cbobData->moveAtVelocity(m_motorNumber,m_targetSpeed[m_motorNumber]);
            }
            else{
                m_targetPosition[m_motorNumber] = value.toInt();
                m_cbobData->moveToPosition(m_motorNumber,m_targetSpeed[m_motorNumber],m_targetPosition[m_motorNumber]);
            }
        }

    }
    else{
        //ui_PlayButton1->setText("Go");
        //m_cbobData->moveMotorPower(1,0);
        m_cbobData->moveAtVelocity(1,0);
    }
}
void MotorTest::on_ui_PlayButton2_toggled(bool state)
{
    QString value;

    ui_PowerRadio2->setEnabled(!state);
    ui_VelocityRadio2->setEnabled(!state);
    ui_PositionRadio2->setEnabled(!state);
    ui_TargetSpeedPowerLine2->setEnabled(!state);
    ui_TargetPositionLine2->setEnabled(!state);

    if(state){
        //ui_PlayButton2->setText("Stop");
        value = ui_TargetSpeedPowerLine2->text();
        if(ui_PowerRadio2->isChecked()){
            m_targetPower[m_motorNumber] = value.toInt();
            m_cbobData->moveMotorPower(m_motorNumber,m_targetPower[m_motorNumber]);
        }
        else{
            m_targetSpeed[m_motorNumber] = value.toInt();
            value = ui_TargetPositionLine2->text();
            if(ui_VelocityRadio2->isChecked()){
                m_cbobData->moveAtVelocity(m_motorNumber,m_targetSpeed[m_motorNumber]);
            }
            else{
                m_targetPosition[m_motorNumber] = value.toInt();
                m_cbobData->moveToPosition(m_motorNumber,m_targetSpeed[m_motorNumber],m_targetPosition[m_motorNumber]);
            }
        }

    }
    else{
        //ui_PlayButton2->setText("Go");
        //m_cbobData->moveMotorPower(2,0);
        m_cbobData->moveAtVelocity(2,0);
    }
}
void MotorTest::on_ui_PlayButton3_toggled(bool state)
{
    QString value;

    ui_PowerRadio3->setEnabled(!state);
    ui_VelocityRadio3->setEnabled(!state);
    ui_PositionRadio3->setEnabled(!state);
    ui_TargetSpeedPowerLine3->setEnabled(!state);
    ui_TargetPositionLine3->setEnabled(!state);

    if(state){
        //ui_PlayButton3->setText("Stop");
        value = ui_TargetSpeedPowerLine3->text();
        if(ui_PowerRadio3->isChecked()){
            m_targetPower[m_motorNumber] = value.toInt();
            m_cbobData->moveMotorPower(m_motorNumber,m_targetPower[m_motorNumber]);
        }
        else{
            m_targetSpeed[m_motorNumber] = value.toInt();
            value = ui_TargetPositionLine3->text();
            if(ui_VelocityRadio3->isChecked()){
                m_cbobData->moveAtVelocity(m_motorNumber,m_targetSpeed[m_motorNumber]);
            }
            else{
                m_targetPosition[m_motorNumber] = value.toInt();
                m_cbobData->moveToPosition(m_motorNumber,m_targetSpeed[m_motorNumber],m_targetPosition[m_motorNumber]);
            }
        }

    }
    else{
        //ui_PlayButton3->setText("Go");
        //m_cbobData->moveMotorPower(3,0);
        m_cbobData->moveAtVelocity(3,0);
    }
}
