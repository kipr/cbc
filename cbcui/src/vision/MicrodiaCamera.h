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

#ifndef INCLUDE_MicrodiaCamera_h
#define INCLUDE_MicrodiaCamera_h

// class MicrodiaCamera:  simulates a camera by loading images from disk

// Qt
#include <QWidget>
#include <QThread>
#include <linux/videodev2.h>
// Local includes
#include "Camera.h"

class MicrodiaCamera;

class MicrodiaCameraThread : public QThread {
public:
  MicrodiaCameraThread(MicrodiaCamera &camera)
      : m_camera(camera)
  {}

  ~MicrodiaCameraThread()
  { wait(); }

  virtual void run();

protected:
  MicrodiaCamera &m_camera;
};

class MicrodiaCamera : public Camera {
public:
  MicrodiaCamera();
  virtual ~MicrodiaCamera();
  virtual void requestOneFrame();
  virtual void requestContinuousFrames();
  virtual void stopFrames();
  int setParameter(enum cam_parms id, int value);
  int getParameter(enum cam_parms id);
  void backgroundLoop();

public slots:
  void setDefaultParams();

protected:
  bool openCamera();
  void closeCamera();
  // volatile since we're modifying and accessing from more than one thread
  // reads and writes to these are atomic, so we don't lock access to these
  volatile bool m_processOneFrame;
  volatile bool m_processContinuousFrames;
  int  m_camDevice;
  MicrodiaCameraThread m_thread;
  void checkSettings();
  void readSettings();
  void writeSetting(enum cam_parms id, int value);
};

  
#endif
