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

#ifdef HAS_MCHECK
#include <mcheck.h>
#endif

#include <stdio.h>
#include <stdlib.h>

// Self
#include "ctdebug.h"

bool check_heap_initialized = false;
bool check_heap_failed = false;


void abort()
{
  ctabort();
}

void ctabort()
{
  static int x;
  x = *(int*)0;
}

#ifdef HAS_MCHECK  
void check_heap_error_detected(enum mcheck_status status)
{
  ::printf("mcheck error detection handler: detected (%d)\n", status);
  ctabort();
  check_heap_failed = true;
}
#endif  

#ifdef SINGLE_THREAD_DEBUG  
#ifdef HAS_MCHECK  
void check_heap_initialize()
{
  check_heap_initialized = true;
  int ret = mcheck(check_heap_error_detected);
  ::printf("mcheck called, ret=%d\n", ret);
}
#endif  
#endif

void check_heap()
{
#ifdef SINGLE_THREAD_DEBUG  
#ifdef HAS_MCHECK  
  if (!check_heap_initialized) check_heap_initialize();
  mcheck_check_all();
  if (check_heap_failed) {
    ::printf("in check_heap:  aborting\n");
    ctabort();
  }
#endif
#endif
}
