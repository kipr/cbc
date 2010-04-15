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

#ifndef BRIGHTNESS_H
#define BRIGHTNESS_H

#include <QtGui/QWidget>
#include "Page.h"
#include "ui_Brightness.h"
#include <QSettings>
#include <QTimer>
#include <QPoint>

class Brightness : public Page, public Ui::Brightness {
    Q_OBJECT
public:
    Brightness(QWidget *parent = 0);
    ~Brightness();

protected slots:
   void on_ui_dimCombo_currentIndexChanged(int i);
   void on_ui_brightness_valueChanged(int i);
   void on_ui_dimCheckBox_clicked(bool checked = false);
   void mouseUpdateChecker();
   void dim();

protected:
    void setBrightness(int i);

private:
    QSettings m_settings;
    int m_brightness;
    int m_dimAfter;
    QTimer m_mouseUpdate;
    QTimer m_dimmer;
    bool m_dimmed;
    QPoint m_lastMousePos;
    bool m_blocked;
    bool m_dimOff;
};

#endif // BRIGHTNESS_H
