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

#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include <QSettings>
#include "ui_Settings.h"
#include "Page.h"
#include "Brightness.h"
#include "Volume.h"

class Settings : public Page, private Ui::Settings
{
    Q_OBJECT

public:
    Settings(QWidget *parent = 0);
    ~Settings();

public slots:
    void recalibrateMotors();
    void recalibrateAccel();
    void recalibrateTS();
    void resetPID();
    void setCameraDefault();
    void on_ui_consoleShowBox_clicked(bool checked = true);

    void storePidCal();
    void loadPidCal();

    void storeAccelCal();
    void loadAccelCal();

private:
    QSettings   m_settings;
    Brightness  m_brightness;
    Volume      m_volume;
};

#endif
