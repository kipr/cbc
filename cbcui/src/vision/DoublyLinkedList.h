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

#ifndef INCLUDE_DoublyLinkedList_h
#define INCLUDE_DoublyLinkedList_h

#include <stdio.h>

template <class T>
class DoublyLinkedList {
public:
  DoublyLinkedList() : m_prev(thisT()), m_next(thisT()) {}
  typedef DoublyLinkedList<T> BaseT;
  T *thisT() { return static_cast<T*>(this); }
  T *prev() const { return m_prev; }
  T *next() const { return m_next; }
  void remove() {
    m_prev->m_next = m_next;
    m_next->m_prev = m_prev;
    m_prev = m_next = thisT();
  }
  void insertNext(BaseT *dest) {
    ctassert(m_prev==thisT());
    ctassert(m_next==thisT());
    m_prev=dest;
    m_next=dest->m_next;
    m_prev->m_next=thisT();
    m_next->m_prev=thisT();
  }
  void insertPrev(BaseT *dest) {
    ctassert(m_prev==thisT());
    ctassert(m_next==thisT());
    m_next=dest->thisT();
    m_prev=dest->m_prev;
    m_prev->m_next=thisT();
    m_next->m_prev=thisT();
  }
  // Swap places between this element and another
  void swap(BaseT *other) {
    // Pay special attention to the case of an empty list
    if (m_next == thisT()) m_next=m_prev=other->thisT();
    if (other->m_next == other) other->m_next=other->m_prev=thisT();

    std::swap(m_prev, other->m_prev);
    std::swap(m_next, other->m_next);
  }
  void deleteRest() {
    while (m_next != thisT()) delete m_next;
  }

  ~DoublyLinkedList() {
    remove();
  }
protected:
  T *m_prev;
  T *m_next;
};
  
#endif
