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

#include "Settings.h"
#include "CbobData.h"
#include <QFile>

Settings::Settings(QWidget *parent) : Page(parent)
{
    setupUi(this);

    QObject::connect(ui_recalibrateMotorsButton, SIGNAL(clicked()), this, SLOT(recalibrateMotors()));
    QObject::connect(ui_recalibrateAccelerometerButton, SIGNAL(clicked()), this, SLOT(recalibrateAccel()));
    QObject::connect(ui_resetPIDButton, SIGNAL(clicked()), this, SLOT(resetPID()));
    QObject::connect(ui_cameraDefaultButton, SIGNAL(clicked()), this, SLOT(setCameraDefault()));
}

Settings::~Settings()
{
}

void Settings::recalibrateMotors()
{
    CbobData::instance()->motorsRecalibrate();
}

void Settings::recalibrateAccel()
{
    CbobData::instance()->accelerometerRecalibrate();
}

void Settings::resetPID()
{
    CbobData::instance()->defaultPIDgains();
}

void Settings::setCameraDefault()
{
#ifdef QT_ARCH_ARM
  QFile::remove("/mnt/user/vision/track_colors");
#else
  QFile::remove(QDir::homePath().toStdString() + "/track_colors");
#endif
}



