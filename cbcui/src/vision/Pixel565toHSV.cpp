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

// Local includes
#include "ctdebug.h"
#include <stdio.h>
// Self
#include "Pixel565toHSV.h"

HSV Pixel565toHSV::pixel565_to_hsv_lut[Pixel565::MAXVAL+1];
bool Pixel565toHSV::pixel565_to_hsv_lut_valid;

void Pixel565toHSV::init()
{
    if (!pixel565_to_hsv_lut_valid) {
        // fill the hsv_lut array with all values in the color field
        for (unsigned p = 0; p < Pixel565::MAXVAL+1; p++) {
            pixel565_to_hsv_lut[p] = HSV::fromPixel(Pixel565(p));
        }
        pixel565_to_hsv_lut_valid = true;
    }
}


void Pixel565toHSV::test()
{
  ctassert(convert(Pixel565::black())   == HSV(  0,  0,  0));       
  ctassert(convert(Pixel565::white())   == HSV(  0,  0,255));     
  ctassert(convert(Pixel565::red())     == HSV(  0,255,255));
  ctassert(convert(Pixel565::yellow())  == HSV( 60,255,255));
  ctassert(convert(Pixel565::green())   == HSV(120,255,255));
  ctassert(convert(Pixel565::cyan())    == HSV(180,255,255));
  ctassert(convert(Pixel565::blue())    == HSV(240,255,255));
  ctassert(convert(Pixel565::magenta()) == HSV(300,255,255));
}

