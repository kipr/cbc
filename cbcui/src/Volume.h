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

#ifndef VOLUME_H
#define VOLUME_H

#include <QtGui/QWidget>
#include "Page.h"
#include "ui_Volume.h"
#include <QSettings>

class Volume : public Page, public Ui::Volume {
    Q_OBJECT
public:
    Volume(QWidget *parent = 0);
    ~Volume();

protected slots:
   void on_ui_volume_valueChanged(int i);

protected:
    void setVolume(int i);

private:
    QSettings m_settings;
};

#endif // VOLUME_H
