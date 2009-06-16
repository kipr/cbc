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

// Qt
#include <QThread>

// Local includes
#include "Pixel565toHSV.h"
#include "HSVRangeLUT.h"
#include "ctdebug.h"
#include "ColorTracker.h"

// Self
#include "test.h"


void test_all()
{
  Pixel565toHSV::test();
  HSVRangeLUT::test();
}

class TestThread : public QThread {
  void run() {
    while (1) {
      check_heap();
      for (int i= 0; i < 1000; i++) {
        void *buf = malloc(i*10);
        memset(buf, 0, i*10);
        free(buf);
      }
    }
  }
};

void test_threads()
{
  int nthreads=2;
  TestThread threads[nthreads];
  printf("Testing threads and heap:  %d threads\n", nthreads);
  for (int i = 0; i < nthreads; i++) {
    threads[i].start();
  }
  for (int i = 0; i < nthreads; i++) {
    threads[i].wait();
  }
}



