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

#ifndef __SERVOS_H__
#define __SERVOS_H__

#include "ui_Servos.h"
#include "Page.h"
#include "CbobData.h"
#include <QDialog>
#include <QPainter>

class Servos : public Page, private Ui::Servos
{
    Q_OBJECT

public:
    Servos(QWidget *parent = 0);
    ~Servos();

public slots:
    void on_ui_ServoDecButton_clicked(bool checked = false);
    void on_ui_ServoIncButton_clicked(bool checked = false);
    void allStop();
    void show();
    void hide();

    void on_ui_AutoCheck_stateChanged(int state);
    void on_ui_UpdateButton_clicked(bool checked = false);
    void on_ui_CenterButton_clicked(bool checked = false);
    void on_ui_ServoRightButton_clicked(bool checked = false);
    void on_ui_ServoLeftButton_clicked(bool checked = false);
    void on_ui_ServoPositionLine_selectionChanged();

protected:
    void paintEvent(QPaintEvent *event);

private:
    CbobData *m_cbobData;
    int m_servoNumber;
    int m_servoPosition[4];
};

#endif
