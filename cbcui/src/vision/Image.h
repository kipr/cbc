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

#ifndef IMAGE_H
#define IMAGE_H

// System includes
#include <stdlib.h>
#include <stdio.h>

// Qt
#include <QImage>

// Local includes
#include "Pixel565.h"
#include "ctdebug.h"

using namespace std;
typedef unsigned short ushort;

// Hue from 0 to 360
// 0:   red
// 60:  yellow
// 120: green
// 180: cyan
// 240: blue
// 300: magenta

class Image {
public:
  Pixel565 *image;
  int nrows, ncols;
  int rowsize;  // in pixels
  bool do_free;
  Image(int nrows_init=0, int ncols_init=0) {
    image= NULL;
    nrows=ncols=rowsize=0;
    do_free=true;
    resize(nrows_init, ncols_init);
  }
  Image(int nrows_init, int ncols_init, int rowsize_init, Pixel565 *imgbuf) {
    image= imgbuf;
    nrows=nrows_init;
    ncols=ncols_init;
    rowsize=rowsize_init;
    do_free=false;
  }
  Image(const char *filename);

  static Image wrapQImage(QImage &image) {
    return Image(image.height(), image.width(), (image.scanLine(1)-image.scanLine(0))/sizeof(Pixel565), (Pixel565*)image.scanLine(0));
  }

  ~Image() { 
    if(do_free) 
      free(image); 
  }
  void resize(int new_nrows, int new_ncols) {
    if (new_nrows != nrows || new_ncols != ncols) {
      ctassert(do_free); // can only resize if we are managing the storage
      ctassert(sizeof(Pixel565) == 2);
      ctassert(sizeof(ushort) == 2);
      size_t new_size = new_nrows*new_ncols*sizeof(Pixel565);
      image= (Pixel565*)realloc((void*)image, new_size);
      nrows= new_nrows; rowsize= ncols= new_ncols;
    }
  }
  
  Pixel565 &pixel(int x, int y) { return image[x+y*rowsize]; }
  const Pixel565 pixel(int x, int y) const { return image[x+y*rowsize]; }
  
  Pixel565 *scanLine(int y) { return &image[y*rowsize]; }
  const Pixel565 *scanLine(int y) const { return &image[y*rowsize]; }
  
  void fill(Pixel565 pixel) {
    for (int i= 0; i< nrows*ncols; i++) image[i]= pixel;
  }
  void copy_from(const Image &src);
  void load(const QImage &image, int x=0, int y=0);
  void load(const Image &image, int x, int y);
  void loadRaw(const char *filename);
  QImage toQImage() const;
  
  void draw_line(int sx, int sy, int ex, int ey, Pixel565
                 color);
  void draw_cross(int x, int y, int size, Pixel565 color);
  void draw_box(int sx, int sy, int ex, int ey, Pixel565 color);
  void draw_fillrect(int sx, int sy, int ex, int ey, Pixel565 color);
};

#endif
