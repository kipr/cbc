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

#ifndef Include_HSVRangeDisplay_h
#define Include_HSVRangeDisplay_h

// Qt includes
#include <QWidget>

// Local includes
#include "HSVRange.h"

class HSVRangeDisplay : public QWidget {
public:
  HSVRangeDisplay(QWidget *parent=NULL);
  QImage m_image;
  HSVRange m_range;
  void setRange(const HSVRange &range);
protected:
  void updateImage();
  virtual void paintEvent(QPaintEvent *);
  virtual void resizeEvent(QResizeEvent *event);
  virtual void showEvent(QShowEvent *event);
  inline int x2sat(int x);
  inline int x2val(int x);
  inline int y2hue(int y);
  inline int sat2x(int sat);
  inline int val2x(int val);
  inline int hue2y(int hue);

  int m_hueOffset;
  int m_width, m_height;
};
  
#endif
