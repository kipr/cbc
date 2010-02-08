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

// Self
#include "ImageDisplay.h"

ImageDisplay::ImageDisplay(QWidget *parent) :
  QWidget(parent)
{
  setAttribute(Qt::WA_NoSystemBackground, true);
}

void ImageDisplay::loadImage(const Image &src)
{
  m_Image.copy_from(src);
  update();
}

void ImageDisplay::updateImages()
{
  //printf("ImageDisplay::updateImages(): %d x %d\n", width(), height());
  m_QImage = QImage(width(), height(), QImage::Format_RGB16);
  m_Image = Image::wrapQImage(m_QImage);
}

void ImageDisplay::paintEvent(QPaintEvent *)
{
  QPainter painter(this);
  painter.drawImage(QPoint(0,0), m_QImage);
}

void ImageDisplay::resizeEvent(QResizeEvent *)
{
  if (isVisible()) updateImages();
}

void ImageDisplay::showEvent(QShowEvent *)
{
  updateImages();
}
