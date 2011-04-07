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

#define DIM_AFTER_KEY   "Dim_After"
#define BRIGHTNESS_KEY  "Brightness"
#define DIM_OFF_KEY     "Dim_Off"
#define DIM_LEVEL       45
#define MINUTE          60000

Brightness::Brightness(QWidget *parent) :
    Page(parent),
    m_settings("/mnt/kiss/config/cbc_v2.config",QSettings::NativeFormat),
    m_mouseUpdate(this),
    m_dimmer(this),
    m_dimmed(true)
{
    setupUi(this);

    m_dimAfter = m_settings.value(DIM_AFTER_KEY, 3).toInt();        // set dim delay value
    m_brightness = m_settings.value(BRIGHTNESS_KEY, 500).toInt();   // brightness values range from 0-512
    m_dimOff = m_settings.value(DIM_OFF_KEY,false).toBool();            // when screen goes dim it is turned off
    m_settings.sync();

    m_blocked = true; // when adding an item to the combo box it registers an event, ignore these events so settings can take place
    ui_dimCombo->addItem("10 Seconds"); // 0
    ui_dimCombo->addItem("30 Seconds"); // 1
    ui_dimCombo->addItem("1 Minute");   // 2
    ui_dimCombo->addItem("5 Minutes");  // 3
    ui_dimCombo->addItem("10 Minutes"); // 4
    ui_dimCombo->addItem("Never");      // 5
    m_blocked = false;

    ui_dimCheckBox->setChecked(m_dimOff);
    ui_brightness->setValue(m_brightness);
    on_ui_brightness_valueChanged(m_brightness);
    ui_dimCombo->setCurrentIndex(m_dimAfter);
    on_ui_dimCombo_currentIndexChanged(m_dimAfter); // make sure the dim delay interval gets set

    connect(&m_mouseUpdate, SIGNAL(timeout()), this, SLOT(mouseUpdateChecker()));
    connect(&m_dimmer, SIGNAL(timeout()), this, SLOT(dim()));
    m_dimmer.setSingleShot(true);
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
    
    if (i < 5 && m_brightness > DIM_LEVEL) {
        m_dimmer.setInterval(interval[i]);
    }
    else m_dimmer.stop();
    m_dimAfter = i;
}

void Brightness::on_ui_brightness_valueChanged(int i)
{
    if(i<15) i=15; // dont let the user turn the screen completely off
    if(i<DIM_LEVEL) m_dimmer.stop();
    setBrightness(i);
    m_settings.setValue(BRIGHTNESS_KEY, i);
    m_settings.sync();
    m_brightness = i;
}

void Brightness::on_ui_dimCheckBox_clicked(bool checked)
{
    m_settings.setValue(DIM_OFF_KEY, checked);
    m_settings.sync();
    m_dimOff = checked;
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
        if(m_dimAfter < 5 && m_brightness > DIM_LEVEL) m_dimmer.start();
    }
    m_lastMousePos = newMousePos;

}

void Brightness::dim()
{
    if(m_dimOff) setBrightness(0);
    else setBrightness(DIM_LEVEL);
    m_dimmed = true;
    // Speed up the updater a little
    m_mouseUpdate.start(500);
}

void Brightness::setBrightness(int i)
{
    system(QString("echo %1 > /proc/sys/sense1/brightness" ).arg(i*128).toAscii());
}
