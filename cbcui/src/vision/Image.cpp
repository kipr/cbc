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

// System includes
#include <algorithm>

// Self
#include "Image.h"

Image::Image(const char *filename)
{
  QImage qimage = QImage(filename).convertToFormat(QImage::Format_RGB16);
  image= NULL;
  nrows=ncols=rowsize=0;
  do_free=true;
  resize(qimage.height(), qimage.width());
  load(qimage,0,0);
}

void
Image::draw_line(int sx, int sy, int ex, int ey, Pixel565 value) 
{
  // Check bounds
  if(sx<0) sx=0;
  if(sy<0) sy=0;
  if(ex>=ncols) ex=ncols-1;
  if(ey>=ncols) ey=nrows-1;

  // Can currently only draw horizontal or vertical lines
  // TODO: Make diagonal lines work.
  if(sx == ex) {
    ctassert(ey >= sy);
    for(int y = sy; y<=ey; y++) {
      pixel(sx, y) = value;
    }
  }
  else if(sy == ey) {
    ctassert(ex >= sx);
    for(int x = sx; x<=ex; x++) {
      pixel(x, sy) = value;
    }
  }
}

void 
Image::draw_cross(int x, int y, int size, Pixel565 value) 
{
  int sx = max(0, x-size), ex = min(ncols-1, x+size);
  int sy = max(0, y-size), ey = min(nrows-1, y+size);
  draw_line(sx, y, ex, y, value);
  draw_line(x, sy, x, ey, value);
}
void 
Image::draw_box(int sx, int sy, int ex, int ey, Pixel565 value) 
{
  draw_line(sx, sy, ex, sy, value);
  draw_line(sx, sy, sx, ey, value);
  draw_line(ex, sy, ex, ey, value);
  draw_line(sx, ey, ex, ey, value);
}

void 
Image::draw_fillrect(int sx, int sy, int ex, int ey, Pixel565 value) 
{
  for(int y = sy; y<=ey; y++) {
    for(int x = sx; x<=ex; x++) {
      pixel(x, y) = value;
    }
  }
}

void Image::copy_from(const Image &src)
{
  ctassert(src.nrows == nrows);
  ctassert(src.ncols == ncols);
  for (int r = 0; r < nrows; r++) {
    memcpy(scanLine(r), src.scanLine(r), ncols*sizeof(Pixel565));
  }
}

void Image::load(const QImage &image, int x, int y)
{
  ctassert(image.format() == QImage::Format_RGB16);
  ctassert(x+ncols <= image.width());
  ctassert(y+nrows <= image.height());
  for (int r = 0; r < nrows; r++) {
    memcpy((void*)scanLine(r), image.scanLine(r+y)+(x*sizeof(Pixel565)), ncols*sizeof(Pixel565));
  }
}

void Image::load(const Image &image, int x, int y)
{
  ctassert(x+ncols <= image.ncols);
  ctassert(y+nrows <= image.nrows);
  for (int r = 0; r < nrows; r++) {
    memcpy((void*)scanLine(r), image.scanLine(r+y)+x, ncols*sizeof(Pixel565));
  }
}

void Image::loadRaw(const char *filename)
{
  FILE *in=fopen(filename, "r");
  ctassert(in);
  int len = ncols*nrows*sizeof(Pixel565);
  int nread = fread(scanLine(0), 1, len, in);
  ctassert(len == nread);
  fclose(in);
}
  
QImage Image::toQImage() const
{
  QImage ret(ncols, nrows, QImage::Format_RGB16);
  for (int r = 0; r < nrows; r++) {
    memcpy(ret.scanLine(r), scanLine(r), ncols*sizeof(Pixel565));
  }
  return ret;
}
