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

#include "About.h"

#include <QFile>
#include <unistd.h>
#include <fcntl.h>

About::About(QWidget *parent) : Page(parent)
{
    setupUi(this);

    QFile osVersion("/etc/software_version");
    QFile swVersion("/mnt/usb/FIRMWARE_VERSION");

    if(osVersion.open(QIODevice::ReadOnly | QIODevice::Text))
        ui_OSVersion->setText(osVersion.readAll());
    if(swVersion.open(QIODevice::ReadOnly | QIODevice::Text))
        ui_SWVersion->setText(swVersion.readAll());

    char version = 0;
    int fd = ::open("/dev/cbc/status", O_RDONLY);
    if(fd == 0) {
        ::read(fd, &version, 1);
        ::close(fd);
    }

    if(version)
        ui_FWVersion->setText(QString::number(((float)version)/10.0));
    else
        ui_FWVersion->setText("Error");

}

About::~About()
{
}



