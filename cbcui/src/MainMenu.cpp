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

#include "MainMenu.h"

MainMenu::MainMenu(QWidget *parent) : Page(parent), m_programs(parent), m_vision(parent), m_utilities(parent), m_sensorsmotors(parent), m_about(parent), m_settings(parent)
{
    setupUi(this);
    
    QObject::connect(ui_programsButton, SIGNAL(clicked()), &m_programs, SLOT(raisePage()));
    QObject::connect(ui_visionButton, SIGNAL(clicked()), &m_vision, SLOT(raisePage()));
    QObject::connect(ui_sensorsmotorsButton, SIGNAL(clicked()), &m_sensorsmotors, SLOT(raisePage()));
    QObject::connect(ui_utilitiesButton, SIGNAL(clicked()), &m_utilities, SLOT(raisePage()));
    QObject::connect(ui_aboutButton, SIGNAL(clicked()), &m_about, SLOT(raisePage()));
    QObject::connect(ui_settingsButton, SIGNAL(clicked()), &m_settings, SLOT(raisePage()));

}

MainMenu::~MainMenu()
{
}

