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

#include "Servos.h"
#include <assert.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include "../../../kernel/cbob/cbob.h"
#include <QMessageBox>

Servos::Servos(QWidget *parent) : Page(parent)
{
    int i;
    char devname[32];

    setupUi(this);

    m_servoNumber = 0;
    for(i=0;i<4;i++){
        sprintf(devname, "/dev/cbc/servo%d",i);
        m_servo[i] = open(devname, O_RDWR);
    }
    for(i=0;i<4;i++) m_servoPosition[i] = 1024;

    ui_ServoPositionLine->setText(QString::number(1024));
}

Servos::~Servos()
{
    int i;
    for(i=0;i<4;i++) ::close(m_servo[i]);
}

void Servos::raise()
{
    int i;
    ui_AutoCheck->setCheckState(Qt::Unchecked);
    // enable servos
    for(i=0;i<4;i++) this->setServoPosition(i,m_servoPosition[i]);
    update();
}

void Servos::closeServos()
{
    int i;
    // disable servos
    for(i=0;i<4;i++) this->setServoPosition(i,-1);
    this->close();
}

void Servos::on_ui_AutoCheck_stateChanged(int state)
{
    if(state) ui_UpdateButton->setDown(true);
    else ui_UpdateButton->setDown(false);
    this->setServoPosition(m_servoNumber,m_servoPosition[m_servoNumber]);
}

void Servos::on_ui_ServoDecButton_clicked(bool)
{
    if(m_servoNumber == 0) m_servoNumber = 3;
    else m_servoNumber--;

    ui_ServoNumberLabel->setText(QString::number(m_servoNumber));
    ui_ServoPositionLine->setText(QString::number(m_servoPosition[m_servoNumber]));
    update();
}

void Servos::on_ui_ServoIncButton_clicked(bool)
{
    if(m_servoNumber == 3) m_servoNumber = 0;
    else m_servoNumber++;

    ui_ServoNumberLabel->setText(QString::number(m_servoNumber));
    ui_ServoPositionLine->setText(QString::number(m_servoPosition[m_servoNumber]));
    update();
}

void Servos::on_ui_ServoPositionLine_selectionChanged()
{
    int value;
    Keypad user_keypad(this);

    ui_ServoPositionLine->setStyleSheet("QLineEdit#ui_ServoPositionLine{background-color:red}");
    user_keypad.exec();
    value = user_keypad.getValue();

    if(value < 0 || value > 2048) {
        QMessageBox::warning(this,
                            "Input Error",
                             "Value must be between 0 and 2048\n",
                             QMessageBox::Ok,
                             QMessageBox::NoButton);
        return;
    }

    m_servoPosition[m_servoNumber] = value;

    ui_ServoPositionLine->setText(QString::number(value));
    if(ui_AutoCheck->isChecked()) this->setServoPosition(m_servoNumber,m_servoPosition[m_servoNumber]);
    ui_ServoPositionLine->setStyleSheet("QLineEdit#ui_ServoPositionLine{background-color:white}");
    update();
}

void Servos::on_ui_ServoLeftButton_clicked(bool)
{
    int value = m_servoPosition[m_servoNumber];
    int multiplier = 1;

    if(ui_TenRadio->isChecked()) multiplier = 10;
    else if(ui_HundredRadio->isChecked()) multiplier = 100;

    value -= multiplier;
    if(value < 0) value = 0;

    ui_ServoPositionLine->setText(QString::number(value));
    if(ui_AutoCheck->isChecked()) this->setServoPosition(m_servoNumber,value);
    m_servoPosition[m_servoNumber] = value;
    update();
}

void Servos::on_ui_ServoRightButton_clicked(bool)
{
    int value = m_servoPosition[m_servoNumber];
    int multiplier = 1;

    if(ui_TenRadio->isChecked()) multiplier = 10;
    else if(ui_HundredRadio->isChecked()) multiplier = 100;

    value += multiplier;
    if(value > 2048) value = 2048;

    ui_ServoPositionLine->setText(QString::number(value));
    if(ui_AutoCheck->isChecked()) this->setServoPosition(m_servoNumber,value);
    m_servoPosition[m_servoNumber] = value;
    update();
}

void Servos::on_ui_CenterButton_clicked(bool)
{
    ui_ServoPositionLine->setText(QString::number(1024));
    m_servoPosition[m_servoNumber] = 1024;
    if(ui_AutoCheck->isChecked()) this->setServoPosition(m_servoNumber,1024);
    update();
}

void Servos::on_ui_UpdateButton_clicked(bool)
{
    short position = m_servoPosition[m_servoNumber];

    write(m_servo[m_servoNumber], &position, 2);
    update();
}

void Servos::setServoPosition(int servo, int pos)
{
    short position = pos;

    write(m_servo[servo], &position, 2);
}

int Servos::getServoPosition(int servo)
{
    short position;

    read(m_servo[servo], &position, 2);

    return position;
}

void Servos::paintEvent(QPaintEvent *)
{
    QPen marker;
    QPainter sketcher(this);
    QRect rect(-70,-70,140,140);

    marker = QPen(Qt::red,3);
    sketcher.setPen(marker);
    sketcher.setBrush(QBrush(Qt::black));
    sketcher.translate(130,135);
    sketcher.drawChord(rect,5440,3520);
    sketcher.rotate(m_servoPosition[m_servoNumber]/9.31 + 70);
    marker.setColor(Qt::white);
    marker.setCapStyle(Qt::RoundCap);
    sketcher.setPen(marker);
    sketcher.drawLine(0,0,0,65);
    sketcher.setPen(QPen(Qt::blue));
    sketcher.drawLine(0,40,0,63);
}



