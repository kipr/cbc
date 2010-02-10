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

// Qt
#include <QPainter>

// Local
#include "bbvision_types.h"
#include "HSV.h"
#include "Pixel565.h"

// Self
#include "HSVRangeDisplay.h"

HSVRangeDisplay::HSVRangeDisplay(QWidget *parent) :
  QWidget(parent), m_hueOffset(300)
{
  setAttribute(Qt::WA_NoSystemBackground, true);
}

void HSVRangeDisplay::setRange(const HSVRange &range)
{
  m_range = range;
  update();
}

inline int HSVRangeDisplay::x2sat(int x) { return x * 255 / (m_width/2); }
inline int HSVRangeDisplay::x2val(int x) { return 255 - (x-m_width/2) * 255 / ((m_width+1)/2); }
inline int HSVRangeDisplay::y2hue(int y) { return (m_hueOffset + (y * 360 / m_height)) % 360; }

inline int HSVRangeDisplay::sat2x(int sat) { return sat * (m_width/2) / 255; }
inline int HSVRangeDisplay::val2x(int val) { return m_width/2 + (255 - val) * ((m_width+1)/2) / 255; }
inline int HSVRangeDisplay::hue2y(int hue) { return (((360-m_hueOffset) + hue) % 360) * m_height / 360; }

void HSVRangeDisplay::updateImage()
{
  m_width = width();
  m_height = height();
  //printf("HSVRangeDisplay::updateImage(): %d x %d\n", m_width, m_height);
  m_image = QImage(m_width, m_height, QImage::Format_RGB16);
  if (!m_width || !m_height) return;
                   
  for (int y = 0; y < m_height; y++) {
    uint16 hue = y2hue(y);
    uint16 *dest = (uint16*) m_image.scanLine(y);
    for (int x = 0; x < m_width/2; x++) {
      *dest++ = HSV(hue, x2sat(x), 255).toPixel<Pixel565>().rgb;
    }
    for (int x = m_width/2; x < m_width; x++) {
      *dest++ = HSV(hue, 255, x2val(x)).toPixel<Pixel565>().rgb;
    }
  }
}
  
void HSVRangeDisplay::paintEvent(QPaintEvent *)
{
  QPainter painter(this);
  painter.drawImage(QPoint(0,0), m_image);

  // Draw the bounding rectangle for the HSVRange
  // This is a little tricky because the range can wrap in hue (Y)

  int left = sat2x(m_range.s.min);
  int width = val2x(m_range.v.min) - left;

  int top = hue2y(m_range.h.min);
  int height = hue2y(m_range.h.max) - top;
  if (height < 0) height += m_height;

  painter.setPen(Qt::white);
  painter.drawRect(left, top, width, height);
  painter.drawRect(left, top-m_height, width, height);
}

void HSVRangeDisplay::resizeEvent(QResizeEvent *)
{
  if (isVisible()) updateImage();
}

void HSVRangeDisplay::showEvent(QShowEvent *)
{
  updateImage();
}

