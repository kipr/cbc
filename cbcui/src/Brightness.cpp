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

// Author: Braden McDorman <bmcdorman@gmail.com>

#include "Brightness.h"

#define DIM_AFTER_KEY   "Dim After"
#define BRIGHTNESS_KEY  "Brightness"

#define MINUTE 60000

Brightness::Brightness(QWidget *parent) :
    Page(parent),
    m_settings("/mnt/user/config/brightness.config", QSettings::IniFormat),
    m_mouseUpdate(this),
    m_dimmer(this)
{
    setupUi(this);

    m_dimAfter = m_settings.value(DIM_AFTER_KEY, 5).toInt();
    m_brightness = m_settings.value(BRIGHTNESS_KEY, 10).toInt();

    blocked(true); // Prevents us from registering these events
    ui_dimCombo->addItem("Never");      // 0
    ui_dimCombo->addItem("10 Seconds"); // 1
    ui_dimCombo->addItem("30 Seconds"); // 2
    ui_dimCombo->addItem("1 Minute");   // 3
    ui_dimCombo->addItem("5 Minutes");  // 4
    ui_dimCombo->addItem("10 Minutes");  // 5
    blocked(false);

    ui_brightness->setValue(m_brightness);
    ui_dimCombo->setCurrentIndex(m_dimAfter);

    connect(&m_mouseUpdate, SIGNAL(timeout()), this, SLOT(mouseUpdateChecker()));
    connect(&m_dimmer, SIGNAL(timeout()), this, SLOT(dim()));
    m_mouseUpdate.start(5000);
}

Brightness::~Brightness()
{
}

void Brightness::on_ui_dimCombo_currentIndexChanged(int i)
{
    if(m_blocked) return;
    m_settings.setValue(DIM_AFTER_KEY, i);
    m_settings.sync();
    
    static const int interval[] = {
        MINUTE/6, MINUTE/2, MINUTE, MINUTE*5, MINUTE*10
    };
    
    if (i > 0 && i <= 5) {
        m_dimmer.setInterval(interval[i-1]);
        m_dimmer.start();
    }
    m_dimAfter = i;
}

void Brightness::on_ui_brightness_valueChanged(int i)
{
    if(m_blocked) return;
    setBrightness(i);
    m_settings.setValue(BRIGHTNESS_KEY, i);
    m_settings.sync();
    m_brightness = i;
}

void Brightness::mouseUpdateChecker()
{
    QPoint newMousePos = QCursor::pos();
    if(newMousePos != m_lastMousePos)
    {
        if(m_dimmed)
        {
            m_dimmed = false;
            setBrightness(m_brightness);
            m_mouseUpdate.start(5000);
        }
        if(m_dimAfter > 0) m_dimmer.start();
    }
    m_lastMousePos = newMousePos;

}

void Brightness::dim()
{
    setBrightness(1);
    m_dimmed = true;
    // Speed up the updater a little
    m_mouseUpdate.start(500);
    m_dimmer.stop();
}

void Brightness::blocked(bool a)
{
    m_blocked = a;
}

char Brightness::getHex(int i)
{
    static const char hex[] = "6789ABCDEF";
    if (i > 0 && i <= 10)
        return hex[i-1];
    else
        return 'F';
}

void Brightness::setBrightness(int i)
{
    QString s = "";
    s.fill(getHex(i), 4);
    system(QString(QString("echo 0x") + s + QString(" > /proc/sys/sense1/brightness")).toAscii());
}
