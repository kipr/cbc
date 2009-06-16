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

// system includes
#include <stdio.h>
#include <stdlib.h>

// qt includes
#include <QImage>

void usage() {
  fprintf(stderr, "usage:\n");
  fprintf(stderr, "png_to_565 image.png > image.565\n");
  exit(1);
}
          
int main(int argc, char **argv)
{
  if (argc != 2) usage();
  char *png_name = argv[1];
  QImage image(png_name);
  if (image.width() == 0) {
    fprintf(stderr, "Couldn't open image %s\n", png_name);
    usage();
  }
  image = image.convertToFormat(QImage::Format_RGB16);
  fprintf(stderr, "%s is %d x %d pixels\n", png_name, image.width(), image.height());

  for (int y = 0; y < image.height(); y++) {
    unsigned char *pixels = image.scanLine(y);
    fwrite(pixels, 2, image.width(), stdout);
  }
  
  return 0;
}
