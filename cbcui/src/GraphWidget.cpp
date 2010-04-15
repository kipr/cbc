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

#include "GraphWidget.h"

#include <QPoint>
#include <QPainter>
#include <QRect>

#define ACCSCALE8BIT    127
#define ACCSCALE16BIT   32768

GraphWidget::GraphWidget(QWidget *parent) : QWidget(parent), m_xScale(0), m_yScale(0), m_zScale(0)
{
    int i;

    m_drawX = m_drawY = m_drawZ = true;
    scan_index = 0;
    for(i=0;i<100;i++){
        pointsX[i] = QPoint(i,0);
        pointsY[i] = QPoint(i,0);
        pointsZ[i] = QPoint(i,0);
    }
    
    setFixedWidth(280);
    setFixedHeight(160);
}

GraphWidget::~GraphWidget()
{
}

void GraphWidget::setXScale(int scale)
{
    if(scale) m_xScale = scale;
}

void GraphWidget::setYScale(int scale)
{
    if(scale) m_yScale = scale;
}

void GraphWidget::setZScale(int scale)
{
    if(scale) m_zScale = scale;
}

void GraphWidget::setDrawXLine(bool state)
{
    m_drawX = state;
}
void GraphWidget::setDrawYLine(bool state)
{
    m_drawY = state;
}
void GraphWidget::setDrawZLine(bool state)
{
    m_drawZ = state;
}

void GraphWidget::setScale(int xScale, int yScale, int zScale)
{
    setXScale(xScale);
    setYScale(yScale);
    setZScale(zScale);
}

void GraphWidget::addValues(int x, int y, int z)
{
    scan_index++;
    if(scan_index>99) scan_index = 0;
    
    if(m_xScale) pointsX[scan_index] = QPoint(scan_index, -50*x/m_xScale);
    else         pointsX[scan_index] = QPoint(scan_index, 0);
    
    if(m_yScale) pointsY[scan_index] = QPoint(scan_index, -50*y/m_yScale);
    else         pointsY[scan_index] = QPoint(scan_index, 0);
    
    if(m_zScale) pointsZ[scan_index] = QPoint(scan_index, -50*z/m_zScale);
    else         pointsZ[scan_index] = QPoint(scan_index, 0);
    update();
}

void GraphWidget::resetScan()
{
    int i;
    scan_index = 0;

    for(i=0;i<100;i++){
        pointsX[i] = QPoint(i,0);
        pointsY[i] = QPoint(i,0);
        pointsZ[i] = QPoint(i,0);
    }
}

void GraphWidget::paintEvent(QPaintEvent *)
{
    QPainter sketcher(this);
    QRect rect(0,0,100,100);
    sketcher.setPen(QPen(Qt::darkGray,1));
    sketcher.setBrush(QBrush(Qt::black));
    //sketcher.translate(20,70);
    sketcher.scale(2.8,1.6);
    sketcher.drawRect(rect);
    sketcher.translate(0,50);
    //sketcher.setPen(QPen(Qt::darkGray,0));
    //sketcher.setFont(QFont("Arial",6));
    //sketcher.drawText(99,25,QString::QString(" -g"));
    //sketcher.drawText(99,-25,QString::QString("  g"));
    sketcher.drawLine(1,25,99,25);
    sketcher.drawLine(1,0,99,0);
    sketcher.drawLine(0,-25,99,-25);
    sketcher.setPen(QPen(Qt::white,0));
    sketcher.drawLine(scan_index,50,scan_index,-50);

    if(m_xScale && m_drawX){
        sketcher.setPen(QPen(Qt::green,0));
        sketcher.drawPolyline(pointsX,100);
    }
    if(m_yScale && m_drawY){
        sketcher.setPen(QPen(Qt::yellow,0));
        sketcher.drawPolyline(pointsY,100);
    }
    if(m_zScale && m_drawZ){
        sketcher.setPen(QPen(Qt::red,0));
        sketcher.drawPolyline(pointsZ,100);
    }
}


