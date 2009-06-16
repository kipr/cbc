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

#ifndef PIXEL565_TO_HSV_H
#define PIXEL565_TO_HSV_H

#include "HSV.h"
#include "Pixel565.h"

class Pixel565toHSV {
public:
  static HSV pixel565_to_hsv_lut[Pixel565::MAXVAL+1];
  static inline HSV convert(Pixel565 in) { return pixel565_to_hsv_lut[in.rgb]; }
  static void test();
  static void init();
private:
  static bool pixel565_to_hsv_lut_valid;
};

#endif
