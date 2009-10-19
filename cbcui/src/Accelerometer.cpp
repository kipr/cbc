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

#include "Accelerometer.h"

#include <QPoint>
#include <QPainter>
#include <QRect>

#define ACCSCALE8BIT    127
#define ACCSCALE16BIT   32768

Accelerometer::Accelerometer(QWidget *parent) : Page(parent)
{
    int i;

    setupUi(this);

    QObject::connect(&m_timer, SIGNAL(timeout()), this, SLOT(updateAccel()));

    m_timer.start(100);
    scan_index = 0;
    for(i=0;i<100;i++){
        pointsX[i] = QPoint(i,0);
        pointsY[i] = QPoint(i,0);
        pointsZ[i] = QPoint(i,0);
    }
    
    m_cbobData = CbobData::instance();
}

Accelerometer::~Accelerometer()
{
}

void Accelerometer::show()
{
    m_timer.start();
    Page::show();
}

void Accelerometer::hide()
{
    m_timer.stop();
    Page::hide();
}

void Accelerometer::updateAccel()
{
    if (isVisible()) {
        m_cbobData->updateSensors();
        ui_AccelerationX->setText(QString::number(m_cbobData->accelerometerX()));
        ui_AccelerationY->setText(QString::number(m_cbobData->accelerometerY()));
        ui_AccelerationZ->setText(QString::number(m_cbobData->accelerometerZ()));
    }
    scan_index++;
    if(scan_index>99)scan_index=0;
    pointsX[scan_index] = QPoint(scan_index,-50*m_cbobData->accelerometerX()/ACCSCALE16BIT);
    pointsY[scan_index] = QPoint(scan_index,-50*m_cbobData->accelerometerY()/ACCSCALE16BIT);
    pointsZ[scan_index] = QPoint(scan_index,-50*m_cbobData->accelerometerZ()/ACCSCALE16BIT);
    update();
}

void Accelerometer::paintEvent(QPaintEvent *)
{
    QPainter sketcher(this);
    QRect rect(-1,0,101,100);
    sketcher.setPen(QPen(Qt::darkGray,1));
    sketcher.setBrush(QBrush(Qt::black));
    sketcher.translate(20,70);
    sketcher.scale(2.8,1.6);
    sketcher.drawRect(rect);
    sketcher.translate(0,50);
    sketcher.setPen(QPen(Qt::darkGray,0));
    sketcher.setFont(QFont("Arial",6));
    sketcher.drawText(99,25,QString::QString(" -g"));
    sketcher.drawText(99,-25,QString::QString("  g"));
    sketcher.drawLine(1,25,99,25);
    sketcher.drawLine(1,0,99,0);
    sketcher.drawLine(0,-25,99,-25);
    sketcher.setPen(QPen(Qt::white,0));
    sketcher.drawLine(scan_index,50,scan_index,-50);

    if(ui_checkX->isChecked()){
        sketcher.setPen(QPen(Qt::green,0));
        sketcher.drawPolyline(pointsX,100);
    }
    if(ui_checkY->isChecked()){
        sketcher.setPen(QPen(Qt::yellow,0));
        sketcher.drawPolyline(pointsY,100);
    }
    if(ui_checkZ->isChecked()){
        sketcher.setPen(QPen(Qt::red,0));
        sketcher.drawPolyline(pointsZ,100);
    }
}


