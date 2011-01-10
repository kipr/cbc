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

#ifndef INCLUDE_Blob_h
#define INCLUDE_Blob_h

// System includes
#include <algorithm>

// Local includes
#include "ctdebug.h"
#include "DoublyLinkedList.h"
#include "Moments.h"

struct Segment {
  unsigned short row;
  unsigned short left;      // inclusive
  unsigned short right;     // inclusive

  enum { invalid_row = 0x3ff };

  // Sum 0^2 + 1^2 + 2^2 + ... + n^2 is (2n^3 + 3n^2 + n) / 6
  // Sum (a+1)^2 + (a+2)^2 ... b^2 is (2(b^3-a^3) + 3(b^2-a^2) + (b-a)) / 6
  //
  // Sum 0+1+2+3+...+n is (n^2 + n)/2
  // Sum (a+1) + (a+2) ... b is (b^2-a^2 + b-a)/2

  void GetMoments(Moments &moments) const {
    int s= left - 1;
    int s2= s*s;
    int e= right;
    int e2= e*e;
    int y= row;
    
    moments.area = e-s;
    moments.sumX = ( (e2-s2) + (e-s) ) / 2;
    moments.sumY = moments.area * y;

    if (Moments::computeAxes) {
      int e3= e2*e;
      int s3= s2*s;
      moments.sumXY= moments.sumX*y;
      moments.sumXX= (2*(e3-s3) + 3*(e2-s2) + (e-s)) / 6;
      moments.sumYY= moments.sumY*y;
    } else {
      moments.sumXY = moments.sumXX = moments.sumYY = 0;
    }
  }
};

class Blob {
public:
  Moments moments;
  // bounds
  unsigned short left, top, right, bottom;
  int parentOffset;

  Blob(const Segment &segment) :
    left(segment.left),
    top(segment.row),
    right(segment.right),
    bottom(segment.row),
    parentOffset(0)
  {
    segment.GetMoments(moments);
  }

  ~Blob() {}

  // Adding a new segment
  // Assumes that row #s never decrease
  void Add(const Segment &segment)
  {
    // Enlarge bounding box if necessary
    left = std::min(left, segment.left);
    right = std::max(right, segment.right);
    ctassert(segment.row >= bottom);
    bottom = segment.row;
    
    Moments segmentMoments;
    segment.GetMoments(segmentMoments);
    moments.Add(segmentMoments);
  }

  void Add(const Blob &blob) {
    moments.Add(blob.moments);
    left   = std::min(left,   blob.left  );
    top    = std::min(top,    blob.top   );
    right  = std::max(right,  blob.right );
    bottom = std::max(bottom, blob.bottom);
  }

  void setParent(Blob *parent) {
    parent->Add(*this);
    parentOffset = parent - this;
  }
  
  Blob *root()
  {
    Blob *ret=this;
    while (ret->parentOffset)
    {
      ret += ret->parentOffset;
    }
    return ret;
  }
};

#endif
