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

#ifndef __SENSOR_PORTS_H__
#define __SENSOR_PORTS_H__

#include "ui_SensorPorts.h"
#include "Page.h"
#include "CbobData.h"
#include <QTimer>

class SensorPorts : public Page, private Ui::SensorPorts
{
    Q_OBJECT

public:
    SensorPorts(QWidget *parent = 0);
    ~SensorPorts();
    
public slots:
   void updateSensors();
   void show();
   void hide();
   
   void on_ui_floatingAnalog0_clicked(bool checked = false);
   void on_ui_floatingAnalog1_clicked(bool checked = false);
   void on_ui_floatingAnalog2_clicked(bool checked = false);
   void on_ui_floatingAnalog3_clicked(bool checked = false);
   void on_ui_floatingAnalog4_clicked(bool checked = false);
   void on_ui_floatingAnalog5_clicked(bool checked = false);
   void on_ui_floatingAnalog6_clicked(bool checked = false);
   void on_ui_floatingAnalog7_clicked(bool checked = false);
   

private:
   CbobData *m_cbobData;
   
   void setAnalogPullup(int port, int enabled);

};

#endif
