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

#include "Graph.h"

#include <QPoint>
#include <QPainter>
#include <QRect>
#include <QString>

#define ACCSCALE8BIT    127
#define ACCSCALE16BIT   32768
#define ACCSCALE        ACCSCALE8BIT
#define DIGSCALE        2
#define ANGSCALE        1024
#define ANGOFF          -512

Graph::Graph(QWidget *parent) : Page(parent)
{
    setupUi(this);

    m_cbobData = CbobData::instance();
    
    ui_delTimeBox->setRange(0.1,600.0);
    ui_delTimeBox->setSuffix("s");
    ui_delTimeBox->setValue(0.1);
    ui_delTimeBox->setSingleStep(0.1);
    QObject::connect(ui_delTimeBox, SIGNAL(valueChanged(double)), this, SLOT(setDelTime(double)));
    m_graph = new GraphWidget(ui_graphWidget);
    
    m_graph->setScale(ACCSCALE, ACCSCALE, ACCSCALE);
    
    ui_graphWidget->show();
    m_graph->show();
}

Graph::~Graph()
{
    delete m_graph;
}

void Graph::show()
{
    //m_cbobData->setRefresh(100);
    this->setDelTime(ui_delTimeBox->value());
    QObject::connect(m_cbobData, SIGNAL(refresh()), this, SLOT(updateGraph()));  
    Page::show();
}

void Graph::hide()
{
    QObject::disconnect(this, SLOT(updateGraph()));
    Page::hide();
}

void Graph::setDelTime(double dt)
{
    int ms = (int)(dt*1000);
    m_cbobData->setRefresh(ms);
    if(dt > 30) ui_delTimeBox->setSingleStep(5.0);
    else if(dt > 10.0) ui_delTimeBox->setSingleStep(1.0);
    else if(dt > 5.0) ui_delTimeBox->setSingleStep(0.5);
    else ui_delTimeBox->setSingleStep(0.1);
}

void Graph::updateGraph()
{
    int xIndex = ui_xSelect->currentIndex();
    int yIndex = ui_ySelect->currentIndex();
    int zIndex = ui_zSelect->currentIndex();
    
    int xScale = getScale(xIndex);
    int yScale = getScale(yIndex);
    int zScale = getScale(zIndex);
    
    int xValue = getValue(xIndex);
    int yValue = getValue(yIndex);
    int zValue = getValue(zIndex);
    
    m_graph->setXScale(xScale);
    m_graph->setYScale(yScale);
    m_graph->setZScale(zScale);
    
    m_graph->addValues(xValue, yValue, zValue);
   
    ui_xValue->setText(QString::number(xValue));
    ui_yValue->setText(QString::number(yValue));
    ui_zValue->setText(QString::number(zValue));
}

int Graph::getValue(int index)
{
    if(index == 1) return m_cbobData->accelerometerX();
    if(index == 2) return m_cbobData->accelerometerY();
    if(index == 3) return m_cbobData->accelerometerZ();
    if(index >= 4 && index <= 11) return ANGOFF + m_cbobData->analog(index-4);
    if(index >= 12 && index <= 19) {
      if(m_cbobData->digital(index-12)) return 1;
      else                             return -1;
    }
    return 0;
}

int Graph::getScale(int index)
{
    if(index > 0 && index < 4) return ACCSCALE;
    if(index >= 4 && index <= 11) return ANGSCALE;
    if(index >= 12 && index <= 19) return DIGSCALE;
    return 0;
}

