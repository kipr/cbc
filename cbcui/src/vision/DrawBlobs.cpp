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
#include <stdio.h>

// Self
#include "DrawBlobs.h"

const Pixel565 blob_colors[] = {
  Pixel565::fromRGB8(0x00, 0xff, 0x00),
  Pixel565::fromRGB8(0x00, 0x00, 0xff),
  Pixel565::fromRGB8(0xff, 0x00, 0x00),
  Pixel565::fromRGB8(0xff, 0x00, 0xff),
  Pixel565::fromRGB8(0x00, 0xff, 0xff),
  Pixel565::fromRGB8(0xff, 0xff, 0x00),
  Pixel565::fromRGB8(0x7f, 0xff, 0x7f),
  Pixel565::fromRGB8(0x7f, 0x7f, 0xff),
  Pixel565::fromRGB8(0xff, 0x7f, 0x7f),
  Pixel565::fromRGB8(0x07, 0x7f, 0x07),
  Pixel565::fromRGB8(0x07, 0x07, 0x7f),
  Pixel565::fromRGB8(0x7f, 0x07, 0x07),
  Pixel565::fromRGB8(0x7f, 0x07, 0x7f),
  Pixel565::fromRGB8(0x07, 0x7f, 0x7f),
  Pixel565::fromRGB8(0x7f, 0x7f, 0x07),
  Pixel565::fromRGB8(0x3f, 0x7f, 0x3f),
  Pixel565::fromRGB8(0x3f, 0x3f, 0x7f),
  Pixel565::fromRGB8(0x7f, 0x3f, 0x3f)
};

const int num_colors = sizeof(blob_colors)/sizeof(blob_colors[0]);
int mat_color=0;
    
void draw_ellipse(Image &dest, float x, float y, float angle,
                  float major_axis, float minor_axis,
                  Pixel565 color);

void draw_blob(Image &dest, Blob *blob, Pixel565 color, bool /*showseg*/, bool showell)
{
  Pixel565 accent_color = color;
  MomentStats stats;
  blob->moments.GetStats(stats);

//  if(showseg) {
//    LinkedSegment *lseg= blob->firstSegment;
//    while(lseg!=NULL) {
//      dest.draw_line((int)lseg->segment.startCol,
//                     (int)lseg->segment.row,
//                     (int)lseg->segment.endCol,
//                     (int)lseg->segment.row,
//                     color);
//      lseg = lseg->next;
//    }
//
//    // Calculate accent color to be inverse of segment color
//    unsigned char r = color.r(), g= color.g(), b= color.b();
//    accent_color = Pixel565(255-r, 255-g, 255-b);
//  }
  if(showell) {
    draw_ellipse(dest, stats.centroidX, stats.centroidY, stats.angle,
                 stats.majorDiameter/2.0, stats.minorDiameter/2.0,
                 accent_color);
  }
  dest.draw_cross((int)stats.centroidX, (int)stats.centroidY,
                  3, accent_color);

  dest.draw_box(blob->left, blob->top, blob->right, blob->bottom, color);
}


// Call this with a completed blob assembler
void DrawBlobs::draw(Image &dest, BlobAssembler &bass, 
                     int minarea, bool showseg,
                     bool showbars, bool showell, 
                     bool showtext)
{
  MomentStats stats;
  int i=0;

  if(showtext)
  {
    printf("---------------------------------------------------------\n");
    printf("Blob stats:\n");
  }

  for (Blob *blob = bass.firstBlob(); blob; blob=bass.nextBlob(blob))
  {
    blob->moments.GetStats(stats);
    if(stats.area >= minarea)
    {
      Pixel565 color = (i < num_colors) ? blob_colors[i] : Pixel565::white();

      if(showtext)
      {
	printf("\tBlob %d: area %d, centroid (%f, %f)\n", i++, stats.area, stats.centroidX, stats.centroidY);
        printf("                axis %f, aspect %f\n", stats.angle*180.0/M_PI, stats.minorDiameter / stats.majorDiameter);
      }
      draw_blob(dest, blob, color, showseg, showell);

      if(showbars && i<=num_colors) dest.draw_fillrect((i-1)*10, 0, i*10, 10, color);
    }
  }
}

//-------------------------------------------------------------------------
const short sintab[512] = {0, 3, 6, 9, 13, 16, 19, 
                          22, 25, 28, 31, 34, 38, 41, 
                          44, 47, 50, 53, 56, 59, 62, 
                          65, 68, 71, 74, 77, 80, 83, 
                          86, 89, 92, 95, 98, 101, 104, 
                          107, 109, 112, 115, 118, 121, 123, 
                          126, 129, 132, 134, 137, 140, 142, 
                          145, 147, 150, 152, 155, 157, 160, 
                          162, 165, 167, 170, 172, 174, 177, 
                          179, 181, 183, 185, 188, 190, 192, 
                          194, 196, 198, 200, 202, 204, 206, 
                          207, 209, 211, 213, 215, 216, 218, 
                          220, 221, 223, 224, 226, 227, 229, 
                          230, 231, 233, 234, 235, 237, 238, 
                          239, 240, 241, 242, 243, 244, 245, 
                          246, 247, 248, 248, 249, 250, 250, 
                          251, 252, 252, 253, 253, 254, 254, 
                          254, 255, 255, 255, 256, 256, 256, 
                          256, 256, 256, 256, 256, 256, 256, 
                          256, 255, 255, 255, 254, 254, 254, 
                          253, 253, 252, 252, 251, 250, 250, 
                          249, 248, 248, 247, 246, 245, 244, 
                          243, 242, 241, 240, 239, 238, 237, 
                          235, 234, 233, 231, 230, 229, 227, 
                          226, 224, 223, 221, 220, 218, 216, 
                          215, 213, 211, 209, 207, 206, 204, 
                          202, 200, 198, 196, 194, 192, 190, 
                          188, 185, 183, 181, 179, 177, 174, 
                          172, 170, 167, 165, 162, 160, 157, 
                          155, 152, 150, 147, 145, 142, 140, 
                          137, 134, 132, 129, 126, 123, 121, 
                          118, 115, 112, 109, 107, 104, 101, 
                          98, 95, 92, 89, 86, 83, 80, 
                          77, 74, 71, 68, 65, 62, 59, 
                          56, 53, 50, 47, 44, 41, 38, 
                          34, 31, 28, 25, 22, 19, 16, 
                          13, 9, 6, 3, 0, -3, -6, 
                          -9, -13, -16, -19, -22, -25, -28, 
                          -31, -34, -38, -41, -44, -47, -50, 
                          -53, -56, -59, -62, -65, -68, -71, 
                          -74, -77, -80, -83, -86, -89, -92, 
                          -95, -98, -101, -104, -107, -109, -112, 
                          -115, -118, -121, -123, -126, -129, -132, 
                          -134, -137, -140, -142, -145, -147, -150, 
                          -152, -155, -157, -160, -162, -165, -167, 
                          -170, -172, -174, -177, -179, -181, -183, 
                          -185, -188, -190, -192, -194, -196, -198, 
                          -200, -202, -204, -206, -207, -209, -211, 
                          -213, -215, -216, -218, -220, -221, -223, 
                          -224, -226, -227, -229, -230, -231, -233, 
                          -234, -235, -237, -238, -239, -240, -241, 
                          -242, -243, -244, -245, -246, -247, -248, 
                          -248, -249, -250, -250, -251, -252, -252, 
                          -253, -253, -254, -254, -254, -255, -255, 
                          -255, -256, -256, -256, -256, -256, -256, 
                          -256, -256, -256, -256, -256, -255, -255, 
                          -255, -254, -254, -254, -253, -253, -252, 
                          -252, -251, -250, -250, -249, -248, -248, 
                          -247, -246, -245, -244, -243, -242, -241, 
                          -240, -239, -238, -237, -235, -234, -233, 
                          -231, -230, -229, -227, -226, -224, -223, 
                          -221, -220, -218, -216, -215, -213, -211, 
                          -209, -207, -206, -204, -202, -200, -198, 
                          -196, -194, -192, -190, -188, -185, -183, 
                          -181, -179, -177, -174, -172, -170, -167, 
                          -165, -162, -160, -157, -155, -152, -150, 
                          -147, -145, -142, -140, -137, -134, -132, 
                          -129, -126, -123, -121, -118, -115, -112, 
                          -109, -107, -104, -101, -98, -95, -92, 
                          -89, -86, -83, -80, -77, -74, -71, 
                          -68, -65, -62, -59, -56, -53, -50, 
                          -47, -44, -41, -38, -34, -31, -28, 
                          -25, -22, -19, -16, -13, -9, -6, 
                          -3};

const short costab[512] = {256, 256, 256, 256, 256, 256, 255, 
                           255, 255, 254, 254, 254, 253, 253, 
                           252, 252, 251, 250, 250, 249, 248, 
                           248, 247, 246, 245, 244, 243, 242, 
                           241, 240, 239, 238, 237, 235, 234, 
                           233, 231, 230, 229, 227, 226, 224, 
                           223, 221, 220, 218, 216, 215, 213, 
                           211, 209, 207, 206, 204, 202, 200, 
                           198, 196, 194, 192, 190, 188, 185, 
                           183, 181, 179, 177, 174, 172, 170, 
                           167, 165, 162, 160, 157, 155, 152, 
                           150, 147, 145, 142, 140, 137, 134, 
                           132, 129, 126, 123, 121, 118, 115, 
                           112, 109, 107, 104, 101, 98, 95, 
                           92, 89, 86, 83, 80, 77, 74, 
                           71, 68, 65, 62, 59, 56, 53, 
                           50, 47, 44, 41, 38, 34, 31, 
                           28, 25, 22, 19, 16, 13, 9, 
                           6, 3, 0, -3, -6, -9, -13, 
                           -16, -19, -22, -25, -28, -31, -34, 
                           -38, -41, -44, -47, -50, -53, -56, 
                           -59, -62, -65, -68, -71, -74, -77, 
                           -80, -83, -86, -89, -92, -95, -98, 
                           -101, -104, -107, -109, -112, -115, -118, 
                           -121, -123, -126, -129, -132, -134, -137, 
                           -140, -142, -145, -147, -150, -152, -155, 
                           -157, -160, -162, -165, -167, -170, -172, 
                           -174, -177, -179, -181, -183, -185, -188, 
                           -190, -192, -194, -196, -198, -200, -202, 
                           -204, -206, -207, -209, -211, -213, -215, 
                           -216, -218, -220, -221, -223, -224, -226, 
                           -227, -229, -230, -231, -233, -234, -235, 
                           -237, -238, -239, -240, -241, -242, -243, 
                           -244, -245, -246, -247, -248, -248, -249, 
                           -250, -250, -251, -252, -252, -253, -253, 
                           -254, -254, -254, -255, -255, -255, -256, 
                           -256, -256, -256, -256, -256, -256, -256, 
                           -256, -256, -256, -255, -255, -255, -254, 
                           -254, -254, -253, -253, -252, -252, -251, 
                           -250, -250, -249, -248, -248, -247, -246, 
                           -245, -244, -243, -242, -241, -240, -239, 
                           -238, -237, -235, -234, -233, -231, -230, 
                           -229, -227, -226, -224, -223, -221, -220, 
                           -218, -216, -215, -213, -211, -209, -207, 
                           -206, -204, -202, -200, -198, -196, -194, 
                           -192, -190, -188, -185, -183, -181, -179, 
                           -177, -174, -172, -170, -167, -165, -162, 
                           -160, -157, -155, -152, -150, -147, -145, 
                           -142, -140, -137, -134, -132, -129, -126, 
                           -123, -121, -118, -115, -112, -109, -107, 
                           -104, -101, -98, -95, -92, -89, -86, 
                           -83, -80, -77, -74, -71, -68, -65, 
                           -62, -59, -56, -53, -50, -47, -44, 
                           -41, -38, -34, -31, -28, -25, -22, 
                           -19, -16, -13, -9, -6, -3, 0, 
                           3, 6, 9, 13, 16, 19, 22, 
                           25, 28, 31, 34, 38, 41, 44, 
                           47, 50, 53, 56, 59, 62, 65, 
                           68, 71, 74, 77, 80, 83, 86, 
                           89, 92, 95, 98, 101, 104, 107, 
                           109, 112, 115, 118, 121, 123, 126, 
                           129, 132, 134, 137, 140, 142, 145, 
                           147, 150, 152, 155, 157, 160, 162, 
                           165, 167, 170, 172, 174, 177, 179, 
                           181, 183, 185, 188, 190, 192, 194, 
                           196, 198, 200, 202, 204, 206, 207, 
                           209, 211, 213, 215, 216, 218, 220, 
                           221, 223, 224, 226, 227, 229, 230, 
                           231, 233, 234, 235, 237, 238, 239, 
                           240, 241, 242, 243, 244, 245, 246, 
                           247, 248, 248, 249, 250, 250, 251, 
                           252, 252, 253, 253, 254, 254, 254, 
                           255, 255, 255, 256, 256, 256, 256, 
                           256};

void draw_ellipse(Image &dest, float x, float y, float angle, float major_axis, float minor_axis, Pixel565 color)
{
  int step=1;
  int ix = (int)x, iy = (int)y;

  int a = (int)((angle/(2.0*M_PI))*512.0);
  if(a<0)
  {
    a+=512;
  }

  for(int p=0; p<512; p+=step)
  {
    // This is the x,y location at this angle around the ellipse if
    // the ellipse were centered at zero and had theta=0;
    int ce_x = (int)(major_axis * costab[p]);
    int ce_y = (int)(minor_axis * sintab[p]);

    // This calculates the actual x,y location by rotating ellipse_p
    // by theta, then adding the center x,y
    int p_x = ix + ((ce_x * costab[a] - ce_y * sintab[a])>>16);
    int p_y = iy + ((ce_y * costab[a] + ce_x * sintab[a])>>16);

    if (p_x >= 0 && p_x < dest.ncols && p_y >= 0 && p_y < dest.nrows)
      dest.pixel(p_x, p_y) = color;
  }
}
