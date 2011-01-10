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

#ifndef INCLUDE_SharedMem_h
#define INCLUDE_SharedMem_h

// System includes
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>

template <class T>
class SharedMem {
public:
  SharedMem(const std::string &filename) :
    m_size(sizeof(T)),
    m_filename(filename),
    m_shared(NULL) {
    m_fd = open(filename.c_str(), O_RDWR | O_CREAT, 0666);  // added 0666 for file creation permissions
    if (m_fd < 0) {
      perror("SharedMem:open");
      return;
    }
    fchmod(m_fd, 0666);
    if (ftruncate(m_fd, m_size) < 0) {
      perror("SharedMem:ftruncate");
      return;
    }
    m_shared = (T*)mmap(0, m_size, PROT_READ|PROT_WRITE, MAP_SHARED, m_fd, 0);
    if (!m_shared) {
      perror("SharedMem:mmap");
      return;
    }
  }
  // Read snapshot of shared memory copy to val
  void read(T &dest) {
    dest = *m_shared;
  }
  // Copy val to shared memory
  void write(const T &src) {
    *m_shared = src;
  }
  // Return pointer to shared memory.  Warning:  accessing this directly leaves you open
  // for synchronization problems.  Consider only using this for atomic reads and writes
  // (e.g. 32-bit or less)
  T &shared() { return *m_shared; }
  
  ~SharedMem() {
    if (m_shared) munmap((void*)m_shared, m_size);
    if (m_fd >= 0) close(m_fd);
  }
  
protected:
  size_t m_size;
  std::string m_filename;
  int m_fd;
  T *m_shared;
};

#endif
