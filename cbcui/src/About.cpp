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

	// chumby software version
	QFile osVersion("/etc/software_version");
    if(osVersion.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        m_OSVersion = osVersion.readLine();
        if(m_OSVersion.endsWith("\n"))
            m_OSVersion.chop(1);
        ui_OSVersion->setText(m_OSVersion);
    }
	
	// CBC software version
	QFile swVersion("/mnt/kiss/FIRMWARE_VERSION");
    if(swVersion.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        m_SWVersion = swVersion.readLine();
        if(m_SWVersion.endsWith("\n"))
            m_SWVersion.chop(1);
        ui_SWVersion->setText(m_SWVersion);
    }

	// BoB software version
    short version = 0;
    int fd = ::open("/dev/cbc/status", O_RDONLY);
    if(fd > 0) {
        ::read(fd, &version, 2);
        ::close(fd);
    }
    if(version)
        ui_FWVersion->setText(QString::number(version));
    else
        ui_FWVersion->setText("Error");

}

About::~About()
{
}



