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

#ifndef XRC_LUT_H
#define XRC_LUT_H

// Local includes
#include "Pixel565.h"
#include "HSVRange.h"

class HSVRangeLUT {
public:
  uint8 m_lut[Pixel565::MAXVAL+1];
  uint8 lookup(uint16 value) const    { return m_lut[value]; }
  uint8 lookup(Pixel565 value) const  { return lookup(value.rgb); }
  bool contains(uint8 channel, Pixel565 value) const { return !!((1<<channel) & lookup(value)); }
  void setModel(uint8 channel, const HSVRange &range);
  HSVRange getModel(uint8 channel) const { return m_models[channel]; }
  static void test();
protected:
  HSVRange m_models[8];
};

#endif
