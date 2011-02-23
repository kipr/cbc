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

#include "VisionSelect.h"

VisionSelect::VisionSelect(QWidget *parent) :
        Page(parent),
        m_tracking(parent, &m_vision.m_colorTracker),
        m_setting(parent, m_vision.m_camera,&m_vision.m_rawCameraView)
{
    setupUi(this);

    QObject::connect(ui_trackingButton, SIGNAL(clicked()), &m_tracking, SLOT(raisePage()));
    QObject::connect(ui_settingButton, SIGNAL(clicked()), &m_setting, SLOT(raisePage()));

    m_tracking.raisePage();
}

VisionSelect::~VisionSelect()
{
}
