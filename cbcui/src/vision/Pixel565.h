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

#ifndef PIXEL565_H
#define PIXEL565_H

#include "bbvision_types.h"

struct Pixel565 {
  enum {RMAX = 31, GMAX = 63, BMAX = 31};
  enum {MAXVAL = 65535};

  uint16 rgb;
  Pixel565(uint8 r, uint8 g, uint8 b) : rgb(b + (g<<5) + (r<<11)) {}
  Pixel565(uint16 rgb_init) : rgb(rgb_init) {}
  Pixel565() {}

  bool operator!=(const Pixel565 &rhs) { return rgb != rhs.rgb; }
  bool operator==(const Pixel565 &rhs) { return rgb == rhs.rgb; }

  inline uint8 r() const { return rgb >> 11; }
  inline uint8 g() const { return (rgb >> 5) & 0x3f; }
  inline uint8 b() const { return rgb & 0x1f; }
  
  inline static Pixel565 black()        { return Pixel565(   0,    0,    0); }
  inline static Pixel565 white()        { return Pixel565(RMAX, GMAX, BMAX); }
  inline static Pixel565 red()          { return Pixel565(RMAX,    0,    0); }
  inline static Pixel565 yellow()       { return Pixel565(RMAX, GMAX,    0); }
  inline static Pixel565 green()        { return Pixel565(   0, GMAX,    0); }
  inline static Pixel565 cyan()         { return Pixel565(   0, GMAX, BMAX); }
  inline static Pixel565 blue()         { return Pixel565(   0,    0, BMAX); }
  inline static Pixel565 magenta()      { return Pixel565(RMAX,    0, BMAX); }
  inline static Pixel565 gray()         { return Pixel565(  15,   32,   15); }
  inline static Pixel565 darkGray()     { return Pixel565(   7,   14,    7); }
  inline static Pixel565 lightGray()    { return Pixel565(  28,   57,   28); }

  // 8-bit (0-255) accessors and setters
  inline uint8 r8() const { return (r() * (65535/RMAX)) >> 8; }
  inline uint8 g8() const { return (g() * (65535/GMAX)) >> 8; }
  inline uint8 b8() const { return (b() * (65535/BMAX)) >> 8; }

  static Pixel565 fromRGB8(uint8 r, uint8 g, uint8 b) {
    return Pixel565(r * RMAX / 255, g * GMAX / 255, b * BMAX / 255);
  }
} __attribute__((__packed__));

#endif
