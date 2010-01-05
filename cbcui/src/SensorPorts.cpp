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

#include "SensorPorts.h"

#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "../../kernel/cbob/cbob.h"

SensorPorts::SensorPorts(QWidget *parent) : Page(parent)
{
    setupUi(this);
    
    m_cbobData = CbobData::instance();
    QObject::connect(&m_timer, SIGNAL(timeout()), this, SLOT(updateSensors()));
    m_timer.start(100);
    
    for(int i = 0;i < 8;i++) setAnalogPullup(i, 1);
}

SensorPorts::~SensorPorts()
{
}

void SensorPorts::show()
{
   m_timer.start(100);
   Page::show();
}

void SensorPorts::hide()
{
   m_timer.stop();
   Page::hide();
}

void SensorPorts::updateSensors()
{
      if(isVisible()) {
         m_cbobData->updateSensors();
        ui_Digital0->setText(QString::number(m_cbobData->digital(0)));
        ui_Digital1->setText(QString::number(m_cbobData->digital(1)));
        ui_Digital2->setText(QString::number(m_cbobData->digital(2)));
        ui_Digital3->setText(QString::number(m_cbobData->digital(3)));
        ui_Digital4->setText(QString::number(m_cbobData->digital(4)));
        ui_Digital5->setText(QString::number(m_cbobData->digital(5)));
        ui_Digital6->setText(QString::number(m_cbobData->digital(6)));
        ui_Digital7->setText(QString::number(m_cbobData->digital(7)));

        ui_Analog8->setText (QString::number(m_cbobData->analog(0)));
        ui_Analog9->setText (QString::number(m_cbobData->analog(1)));
        ui_Analog10->setText(QString::number(m_cbobData->analog(2)));
        ui_Analog11->setText(QString::number(m_cbobData->analog(3)));
        ui_Analog12->setText(QString::number(m_cbobData->analog(4)));
        ui_Analog13->setText(QString::number(m_cbobData->analog(5)));
        ui_Analog14->setText(QString::number(m_cbobData->analog(6)));
        ui_Analog15->setText(QString::number(m_cbobData->analog(7)));

        ui_Motor0Power->setText(QString::number(m_cbobData->motorPWM(0)));
        ui_Motor1Power->setText(QString::number(m_cbobData->motorPWM(1)));
        ui_Motor2Power->setText(QString::number(m_cbobData->motorPWM(2)));
        ui_Motor3Power->setText(QString::number(m_cbobData->motorPWM(3)));

        ui_Motor0Position->setText(QString::number(m_cbobData->motorPosition(0)));
        ui_Motor1Position->setText(QString::number(m_cbobData->motorPosition(1)));
        ui_Motor2Position->setText(QString::number(m_cbobData->motorPosition(2)));
        ui_Motor3Position->setText(QString::number(m_cbobData->motorPosition(3)));
        
        ui_AccelerometerX->setText(QString::number(m_cbobData->accelerometerX()));
        ui_AccelerometerY->setText(QString::number(m_cbobData->accelerometerY()));
        ui_AccelerometerZ->setText(QString::number(m_cbobData->accelerometerZ()));
        
        ui_BatteryVoltage->setText(QString::number(m_cbobData->batteryVoltage()));
   }
}

void SensorPorts::on_ui_floatingAnalog0_clicked(bool checked)
{
  setAnalogPullup(0, !checked);
}

void SensorPorts::on_ui_floatingAnalog1_clicked(bool checked)
{
  setAnalogPullup(1, !checked);
}

void SensorPorts::on_ui_floatingAnalog2_clicked(bool checked)
{
  setAnalogPullup(2, !checked);
}

void SensorPorts::on_ui_floatingAnalog3_clicked(bool checked)
{
  setAnalogPullup(3, !checked);
}

void SensorPorts::on_ui_floatingAnalog4_clicked(bool checked)
{
  setAnalogPullup(4, !checked);
}

void SensorPorts::on_ui_floatingAnalog5_clicked(bool checked)
{
  setAnalogPullup(5, !checked);
}

void SensorPorts::on_ui_floatingAnalog6_clicked(bool checked)
{
  setAnalogPullup(6, !checked);
}

void SensorPorts::on_ui_floatingAnalog7_clicked(bool checked)
{
  setAnalogPullup(7, !checked);
}

void SensorPorts::setAnalogPullup(int port, int enabled)
{
  int fd, mask;
  char *port_string = "/dev/cbc/analog0";
  
  if(enabled) mask = m_cbobData->analogPullups()|(1<<port);
  else        mask = m_cbobData->analogPullups()&(~(1<<port));
  
  fd = ::open(port_string, O_RDWR);
  if(fd > 0) {
    ::ioctl(fd, CBOB_ANALOG_SET_PULLUPS, &mask);
    ::close(fd);
  }
}


