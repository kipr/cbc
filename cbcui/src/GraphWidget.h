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

#ifndef __GRAPHWIDGET_H__
#define __GRAPHWIDGET_H__

#include <QWidget>
#include <QPoint>

class GraphWidget : public QWidget
{
    Q_OBJECT

public:
    GraphWidget(QWidget *parent = 0);
    ~GraphWidget();
    
    void setXScale(int scale);
    void setYScale(int scale);
    void setZScale(int scale);
    void setDrawXLine(bool state);
    void setDrawYLine(bool state);
    void setDrawZLine(bool state);
    void setScale(int xScale, int yScale, int zScale);

public slots:
    void addValues(int x, int y, int z);
    void resetScan();

protected:
    void paintEvent(QPaintEvent *event);

private:
    int scan_index;
    
    int m_xScale;
    int m_yScale;
    int m_zScale;

    bool m_drawX;
    bool m_drawY;
    bool m_drawZ;
    
    QPoint pointsX[100];
    QPoint pointsY[100];
    QPoint pointsZ[100];
};

#endif
