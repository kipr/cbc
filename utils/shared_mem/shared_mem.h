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

#ifndef INCLUDE_shared_mem_h
#define INCLUDE_shared_mem_h

#include <stdlib.h>

typedef struct shared_mem_str {
  size_t size;
  int fd;
  void *shared;
} shared_mem;

shared_mem *shared_mem_create(const char *filename, size_t size);
void shared_mem_destroy(shared_mem *sm);
void shared_mem_read(shared_mem *sm, void *dest, size_t size);
void shared_mem_write(shared_mem *sm, const void *src, size_t size);
void *shared_mem_ptr(shared_mem *sm);

#endif
