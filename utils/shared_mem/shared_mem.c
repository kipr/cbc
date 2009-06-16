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
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

// Self
#include "shared_mem.h"

shared_mem *shared_mem_create(const char *filename, size_t size) {
  shared_mem *ret = malloc(sizeof(shared_mem));
  if (!ret) {
    fprintf(stderr, "shared_mem_create:malloc failed\n");
    return NULL;
  }
  memset(ret, 0, sizeof(shared_mem));
  ret->size = size;
  ret->fd = open(filename, O_RDWR | O_CREAT);
  
  if (ret->fd < 0) {
    perror("shared_mem_create:open");
    shared_mem_destroy(ret);
    return NULL;
  }
  fchmod(ret->fd, 0666);
  if (ftruncate(ret->fd, ret->size) < 0) {
    perror("shared_mem:ftruncate");
    shared_mem_destroy(ret);
    return NULL;
  }
  ret->shared = mmap(0, size, PROT_READ|PROT_WRITE, MAP_SHARED, ret->fd, 0);
  if (!ret->shared) {
    perror("SharedMem:mmap");
    shared_mem_destroy(ret);
    return NULL;
  }
  return ret;
}

void shared_mem_destroy(shared_mem *sm) {
  if (sm->fd >= 0) close(sm->fd);
  if (sm->shared) munmap(sm->shared, sm->size);
  free(sm);
}

void shared_mem_read(shared_mem *sm, void *dest, size_t size) {
  assert(size == sm->size);
  memcpy(dest, sm->shared, size);
}

void shared_mem_write(shared_mem *sm, const void *src, size_t size) {
  assert(size == sm->size);
  memcpy(sm->shared, src, size);
}

void *shared_mem_ptr(shared_mem *sm) {
  return sm->shared;
}
