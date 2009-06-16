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

#ifndef INCLUDE_Camera_h
#define INCLUDE_Camera_h

// class Camera:  Base class for cameras
//  Inherited by SimulatedCamera and (soon) a class for capturing images from the real camera

// Local inclues
#include "FrameHandler.h"

class Camera {
public:
  Camera(unsigned width, unsigned height) : m_width(width), m_height(height) {}
  virtual void requestOneFrame() = 0;
  virtual void requestContinuousFrames() = 0;
  virtual void stopFrames() = 0;
  // Add a handler to call at end of frame
  void addFrameHandler(FrameHandler *frameHandler);
  unsigned width() const { return m_width; }
  unsigned height() const { return m_height; }
  virtual ~Camera() {}
protected:
  unsigned m_width, m_height;
  void callFrameHandlers(const Image &image);
  std::vector<FrameHandler*> m_frameHandlers;
};
  
#endif
