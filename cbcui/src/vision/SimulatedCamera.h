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

#ifndef INCLUDE_SimulatedCamera_h
#define INCLUDE_SimulatedCamera_h

// class SimulatedCamera:  simulates a camera by loading images from disk

// Qt
#include <QImage>
#include <QTimer>
#include <QWidget>

// Local includes
#include "Camera.h"

class SimulatedCamera;

class SimulatedCameraTimer : public QWidget {
  Q_OBJECT
public:
  SimulatedCameraTimer(SimulatedCamera &camera);
  QTimer m_timer;
  virtual void foo() {}
  virtual ~SimulatedCameraTimer() {}
protected:
  SimulatedCamera &m_camera;
protected slots:
  void timeout();
};  

class SimulatedCamera : public Camera {
public:
  SimulatedCamera(int width, int height);
  virtual ~SimulatedCamera() {}
  virtual void requestOneFrame();
  virtual void requestContinuousFrames();
  virtual void stopFrames();
  void loadSimulatedImage(const QImage &image);
  void loadSimulatedImage(const Image &image);
  void createAndProcessFrame();
protected:
  void resetXY();
  Image m_simulatedImage;
  //QImage m_simulatedImage;
  int m_x, m_y, m_dx, m_dy;
  SimulatedCameraTimer m_timer;
};

  
#endif
