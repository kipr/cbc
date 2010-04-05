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

Servos::Servos(QWidget *parent) : Page(parent)
{
    int i;

    setupUi(this);

    m_cbobData = CbobData::instance();
    QObject::connect(m_cbobData, SIGNAL(eStop()), this, SLOT(allStop()));
    m_servoNumber = 0;
    for(i=0;i<4;i++) m_servoPosition[i] = 1024;
    ui_ServoPositionLine->setText(QString::number(1024));
    m_cbobData->disableServos();
}

Servos::~Servos()
{
    m_cbobData->disableServos();
}

void Servos::show()
{
    //qWarning("servos shown");
    ui_AutoCheck->setCheckState(Qt::Unchecked);
    //m_cbobData->disableServos();
    update();
    Page::show();
}

void Servos::hide()
{
    //qWarning("servos hidden");
    // kill servos before hiding page
    //m_cbobData->disableServos();
    Page::hide();
}

void Servos::allStop()
{
    if(ui_AutoCheck->isChecked()) ui_AutoCheck->setCheckState(Qt::Unchecked);
}

void Servos::on_ui_AutoCheck_stateChanged(int state)
{
    if(state){
        ui_UpdateButton->setDown(true);
        m_cbobData->setServoPosition(m_servoNumber,m_servoPosition[m_servoNumber]);
    }
    else ui_UpdateButton->setDown(false);
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
    Keypad user_keypad(this,0,2048);

    ui_ServoPositionLine->setStyleSheet("QLineEdit#ui_ServoPositionLine{background-color:red}");
    user_keypad.exec();
    value = user_keypad.getValue();

    m_servoPosition[m_servoNumber] = value;

    ui_ServoPositionLine->setText(QString::number(value));
    if(ui_AutoCheck->isChecked()) m_cbobData->setServoPosition(m_servoNumber,m_servoPosition[m_servoNumber]);
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
    if(ui_AutoCheck->isChecked()) m_cbobData->setServoPosition(m_servoNumber,value);
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
    if(ui_AutoCheck->isChecked()) m_cbobData->setServoPosition(m_servoNumber,value);
    m_servoPosition[m_servoNumber] = value;
    update();
}

void Servos::on_ui_CenterButton_clicked(bool)
{
    ui_ServoPositionLine->setText(QString::number(1024));
    m_servoPosition[m_servoNumber] = 1024;
    if(ui_AutoCheck->isChecked()) m_cbobData->setServoPosition(m_servoNumber,1024);
    update();
}

void Servos::on_ui_UpdateButton_clicked(bool)
{
    m_cbobData->setServoPosition(m_servoNumber,m_servoPosition[m_servoNumber]);
    update();
}

void Servos::paintEvent(QPaintEvent *)
{
    QPen marker;
    QPainter sketcher(this);
    QRect rect(-70,-70,140,140);

    marker = QPen(Qt::red,3);
    sketcher.setPen(marker);
    sketcher.setBrush(QBrush(Qt::black));
    sketcher.translate(130,114);
    sketcher.drawChord(rect,5440,3520);
    sketcher.rotate(m_servoPosition[m_servoNumber]/9.31 + 70);
    marker.setColor(Qt::white);
    marker.setCapStyle(Qt::RoundCap);
    sketcher.setPen(marker);
    sketcher.drawLine(0,0,0,65);
    sketcher.setPen(QPen(Qt::blue));
    sketcher.drawLine(0,40,0,63);
}



