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

#include "Keyboard/Keypad.h"
#include <QPoint>
#include <QPainter>
#include <QRect>
#include <QString>

#define ACCSCALE8BIT    127
#define ACCSCALE16BIT   32768
#define ACCSCALE        ACCSCALE8BIT
#define DIGSCALE        2
#define ANGSCALE        1024
#define ANGOFF          512

Graph::Graph(QWidget *parent) : Page(parent)
{
    setupUi(this);

    m_cbobData = CbobData::instance();
    
    ui_delTimeLine->setText(QString::number(100));

    m_graph = new GraphWidget(ui_graphWidget);

    QObject::connect(ui_resetButton, SIGNAL(clicked()), m_graph, SLOT(resetScan()));

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
    m_cbobData->setRefresh(ui_delTimeLine->text().toInt());
    QObject::connect(m_cbobData, SIGNAL(refresh()), this, SLOT(updateGraph()));  
    Page::show();
}

void Graph::hide()
{
    QObject::disconnect(this, SLOT(updateGraph()));
    Page::hide();
}

void Graph::on_ui_delTimeLine_selectionChanged()
{
    int ms;
    Keypad *user_keypad = Keypad::instance();
    user_keypad->setRange(100,60000);
    user_keypad->setType(0);

    ui_delTimeLine->setStyleSheet("QLineEdit#ui_delTimeLine{background-color:red}");
    user_keypad->exec();

    ms = user_keypad->getValue();

    ui_delTimeLine->setStyleSheet("QLineEdit#ui_delTimeLine{background-color:white}");
    ui_delTimeLine->setText(QString::number(ms));
    m_cbobData->setRefresh(ms);
}

void Graph::updateGraph()
{
    int xIndex = ui_xSelect->currentIndex();
    int yIndex = ui_ySelect->currentIndex();
    int zIndex = ui_zSelect->currentIndex();
    
    int xScale = getScale(xIndex);
    int yScale = getScale(yIndex);
    int zScale = getScale(zIndex);
    
    if(xIndex == 0) m_graph->setDrawXLine(false);
    else m_graph->setDrawXLine(true);
    if(yIndex == 0) m_graph->setDrawYLine(false);
    else m_graph->setDrawYLine(true);
    if(zIndex == 0) m_graph->setDrawZLine(false);
    else m_graph->setDrawZLine(true);

    ui_xValue->setText(QString::number(getValue(xIndex)));
    ui_yValue->setText(QString::number(getValue(yIndex)));
    ui_zValue->setText(QString::number(getValue(zIndex)));

    int xValue = getOffsetValue(xIndex);
    int yValue = getOffsetValue(yIndex);
    int zValue = getOffsetValue(zIndex);

    m_graph->setXScale(xScale);
    m_graph->setYScale(yScale);
    m_graph->setZScale(zScale);
    
    m_graph->addValues(xValue, yValue, zValue);
}

int Graph::getValue(int index)
{
    if(index == 1) return m_cbobData->accelerometerX();
    if(index == 2) return m_cbobData->accelerometerY();
    if(index == 3) return m_cbobData->accelerometerZ();
    if(index >= 4 && index <= 11) return m_cbobData->analog(index-4);
    if(index >= 12 && index <= 19) {
      if(m_cbobData->digital(index-12)) return 1;
      else                             return 0;
    }
    return 0;
}

int Graph::getOffsetValue(int index)
{
    if(index == 1) return m_cbobData->accelerometerX();
    if(index == 2) return m_cbobData->accelerometerY();
    if(index == 3) return m_cbobData->accelerometerZ();
    if(index >= 4 && index <= 11) return m_cbobData->analog(index-4) - ANGOFF;
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

