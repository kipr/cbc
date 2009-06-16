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
#include <assert.h>
#include <stdio.h>
#include <unistd.h> // sleep
// Local includes
#include "shared_mem.h"
#include "test_struct.h"

int main()
{
  shared_mem *sm = shared_mem_create("/tmp/shared_test_struct", sizeof(test_struct));
  test_struct ts;

  assert(sm);

  int i=0;
  while (1) {
    ts.x = i;
    ts.y = i*2;
    ts.z = i*3;
    shared_mem_write(sm, &ts, sizeof(ts));
    printf("Wrote %d, %d, %d\n", ts.x, ts.y, ts.z);
    i++;
    sleep(1);
  }
}
