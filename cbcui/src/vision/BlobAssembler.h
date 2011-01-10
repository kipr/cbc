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

#ifndef INCLUDE_BlobAssembler_h
#define INCLUDE_BlobAssembler_h


// Strategy for using BlobAssembler:
//
// Make one BlobAssembler for each color.
//
// At the beginning of a frame, call Reset() on each assembler
// As segments appear, call Add(segment)
// At the end of a frame, call EndFrame() on each assembler
// Get blobs from finishedBlobs.  Blobs will remain valid until
//    the next call to Reset(), at which point they will be deleted.
//
// To get statistics for a blob, do the following:
//  MomentStats stats;
//  blob->moments.GetStats(stats);
// (See imageserver.cc: draw_blob() for an example)

// System includes
#include <iostream>

// Local includes
#include "Blob.h"


struct BlobFringe {
public:
  BlobFringe(short left_init, short right_init, int blobIndex_init) :
    left(left_init), right(right_init), blobIndex(blobIndex_init) {}
  short left, right;
  int blobIndex;
  bool sorted;
};

class BlobAssembler {
  short currentRow;
  int totalArea;

  std::vector<BlobFringe> *oldFringe;
  std::vector<BlobFringe> *newFringe;
  std::vector<BlobFringe>::iterator oldFringeIter;
  bool sorted;
  std::vector<Blob*> sortedBlobs;
  std::vector<Blob> blobs;

public:
  
  BlobAssembler() {
    Reset();
    oldFringe = new vector<BlobFringe>();
    newFringe = new vector<BlobFringe>();
  }

  ~BlobAssembler() {
    // Flush any active blobs into finished blobs
    EndFrame();
    // Free any finished blobs
    Reset();
    delete oldFringe;
    delete newFringe;
  }

  // Call prior to starting a frame
  // Deletes any previously created blobs
  void Reset() {
    currentRow=-1;
    // Delete blobs
    blobs.clear();
    totalArea=0;
    sorted = false;
  }

  void SetRow(short row) {
    oldFringe->clear();
    if (row - currentRow > 1) {
      newFringe->clear();
    } else {
      std::swap(oldFringe, newFringe);
    }
    oldFringeIter = oldFringe->begin();
    currentRow = row;
  }

  //void checkArea() {
  //  int area=0;
  //  for (Blob *b= firstBlob(); b; b= nextBlob(b)) {
  //    area += b->moments.area;
  //  }
  //  ctassert(area == totalArea);
  //}
  
  // Call once for each segment in the color channel
  void Add(const Segment &segment) {
    totalArea += segment.right-segment.left+1;
    if (segment.row != currentRow) SetRow(segment.row);

    while (oldFringeIter != oldFringe->end()) {
      if (segment.left > oldFringeIter->right) {
        // Doesn't connect.  Keep searching more blobs to the right.
        ++oldFringeIter;
        continue;
      }
      if (segment.right < oldFringeIter->left) {
        // Doesn't connect to any blob.  Stop searching.
        break;
      }
      // Found a blob to connect to
      Blob *b = getBlob(oldFringeIter);
      
      b->Add(segment);
      
      // Check to see if we attach to multiple blobs
      while(oldFringeIter+1 != oldFringe->end() && segment.right >= oldFringeIter[1].left)
      {
        ++oldFringeIter;
        Blob *lhs_b = getBlob(oldFringeIter);
        if (lhs_b != b) {
          // Mark the first blob to be merged with the second
          if (lhs_b > b) std::swap(lhs_b, b);
          b->setParent(lhs_b);
        }
      }

      newFringe->push_back(BlobFringe(segment.left, segment.right, b-&blobs[0]));
      return;
    }

    // Could not attach to previous blob, insert new one before currentBlob
    int new_index=blobs.size();
    blobs.push_back(Blob(segment));
    newFringe->push_back(BlobFringe(segment.left, segment.right, new_index));
  }

  Blob *getBlob(std::vector<BlobFringe>::iterator i) {
    return blobs[i->blobIndex].root();
  }
  
  // Call at end of frame
  // Moves all active blobs to finished list
  // TODO: sort finished in descending area
  void EndFrame() {
    oldFringe->clear();
    newFringe->clear();
  }

  Blob *firstBlob() {
    if (!blobs.size()) return NULL;
    ctassert(&blobs[0]);
    return nextBlob((&blobs[0])-1);
  }
  Blob *nextBlob(Blob *b) {
    while (++b <= &blobs.back())
    {
      if (b->root() == b) return b;
    }
    return NULL;
  }

  static bool blob_area_gt(Blob *a, Blob *b) { return a->moments.area > b->moments.area; }
  
  void sortBlobs() {
    sorted = true;
    sortedBlobs.clear();

    for (Blob *b = firstBlob(); b; b = nextBlob(b))
    {
      sortedBlobs.push_back(b);
    }
    // Sort by descending area
    std::sort(sortedBlobs.begin(), sortedBlobs.end(), blob_area_gt);
  }
    
  std::vector<Blob*> &getSortedBlobs() {
    if (!sorted) sortBlobs();
    return sortedBlobs;
  }
};

#endif
