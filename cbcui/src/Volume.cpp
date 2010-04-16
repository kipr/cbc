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

#include "Volume.h"

#define VOLUME_KEY   "Volume"

Volume::Volume(QWidget *parent) :
    Page(parent),
    m_settings("/mnt/user/cbc_v2.config",QSettings::NativeFormat)
{
    setupUi(this);

    int volume = m_settings.value(VOLUME_KEY, 90).toInt();   // volume values range from 0-100
    on_ui_volume_valueChanged(volume);
}

Volume::~Volume()
{
}

void Volume::on_ui_volume_valueChanged(int i)
{
    system(QString("chumby_set_volume %1" ).arg(i).toAscii());
    m_settings.setValue(VOLUME_KEY,i);
    m_settings.sync();
}
