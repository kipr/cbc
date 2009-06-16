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

#ifndef HSV_H
#define HSV_H

// System includes
#include <algorithm>

// Local includes
#include "bbvision_types.h"

struct HSV {
  uint16 h;   // 0 to 359
  uint8 s;    // 0 to 255
  uint8 v;    // 0 to 255
  HSV(uint16 h_init, uint8 s_init, uint8 v_init)
    : h(h_init), s(s_init), v(v_init) {}
  HSV() {}
  inline bool operator==(const HSV &rhs) {
    return h == rhs.h && s == rhs.s && v == rhs.v;
  }
  template <class PixelT>
  static HSV fromPixel(PixelT rgb) {
    uint8 r = rgb.r8();
    uint8 g = rgb.g8();
    uint8 b = rgb.b8();
    
    uint8 minval= std::min(std::min(r,g),b);
    uint8 v= std::max(std::max(r,g),b);
    uint8 delta= v-minval;
    
    // Saturation is the ratio of max-min to max. 0 if max is 0
    uint8 s = v ? 255*delta/v : 0;
    int16 h;
    
    if (!s) {
      h= 0; // If no saturation, arbitrarily select hue=0
    } else if (r == v) {
      // between -60 (magenta) and +60 (yellow)
      h=60*(g-b)/delta;
      if (h < 0) h += 360;
    } else if (g == v) {
      // between +60 (yellow) and +180 (cyan)
      h=120+60*(b-r)/delta;
    } else {
      // between +180 (cyan) and +300 (magenta)
      h=240+60*(r-g)/delta;
    }
    return HSV(h,s,v);
  }
  // adapted from http://www.acm.org/jgt/papers/SmithLyons96/hsv_rgb.html
  template <class PixelT>
  PixelT toPixel() {
    int i = h/60;
    int f = h-i*60; // 0 to 60
    if (!(i & 1)) f = 60 - f; // if i is even
    int m = v * (255-s) / 255;  // 0 to 255
    int n = v * (255*60 - s * f)/(255*60);  // 0 to 255
    
    switch (i) {
    case 1:  return PixelT::fromRGB8(n, v, m);
    case 2:  return PixelT::fromRGB8(m, v, n);
    case 3:  return PixelT::fromRGB8(m, n, v);
    case 4:  return PixelT::fromRGB8(n, m, v);
    case 5:  return PixelT::fromRGB8(v, m, n);
    default: return PixelT::fromRGB8(v, n, m);
    }
  }
};

#endif
