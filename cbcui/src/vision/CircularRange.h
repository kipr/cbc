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

#ifndef CIRCULAR_RANGE_H
#define CIRCULAR_RANGE_H

// Implements Range, with wrapping

// How to tell if a value is in a range which wraps
// Cases:
//    Ordering           In?   
//    val   min---max    0    min<=max:1  min<=val:0  val<=max:1
//    min---val---max    1    min<=max:1  min<=val:1  val<=max:1
//    min---max   val    0    min<=max:1  min<=val:1  val<=max:0
// ---val---max   min--- 1    min<=max:0  min<=val:0  val<=max:1
// ---max   val   min--- 0    min<=max:0  min<=val:0  val<=max:0
// ---max   min---val--- 1    min<=max:0  min<=val:1  val<=max:0
// So you can use (min<=max) ^ (min<=val) ^ (val<=max)

template <class T>
struct CircularRange {
  T min; // inclusive
  T max; // inclusive
  CircularRange(T min_init=T(), T max_init=T()) : min(min_init), max(max_init) {}
  inline bool contains(T val) const { return (min <= max) ^ (min <= val) ^ (val <= max); }
};

#endif
