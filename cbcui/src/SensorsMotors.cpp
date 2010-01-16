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

#include "SensorsMotors.h"

SensorsMotors::SensorsMotors(QWidget *parent) : Page(parent), m_servos(parent), m_sensorPorts(parent), m_motors(parent), m_graph(parent)
{
    setupUi(this);
    
    QObject::connect(ui_servosButton, SIGNAL(clicked()), &m_servos, SLOT(raisePage()));
    QObject::connect(ui_sensorPortsButton, SIGNAL(clicked()), &m_sensorPorts, SLOT(raisePage()));
    QObject::connect(ui_motorsButton, SIGNAL(clicked()), &m_motors, SLOT(raisePage()));
    QObject::connect(ui_graphButton, SIGNAL(clicked()), &m_graph, SLOT(raisePage()));
}

SensorsMotors::~SensorsMotors()
{
}



