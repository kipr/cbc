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

#include "SimulatedCamera.h"

SimulatedCamera::SimulatedCamera(int width, int height)
  : Camera(width, height), m_timer(*this) {
}

// Simulated camera scans through the simulated image, moving the window by dx and dy each frame
// If simulate image is the same size as simulated camera, don't scan
void SimulatedCamera::loadSimulatedImage(const QImage &image)
{
  QImage image565 = image.convertToFormat(QImage::Format_RGB16);
  m_simulatedImage.resize(image565.height(), image565.width());
  m_simulatedImage.load(image565);
  resetXY();
}

void SimulatedCamera::loadSimulatedImage(const Image &image)
{
  m_simulatedImage.resize(image.nrows, image.ncols);
  m_simulatedImage.copy_from(image);
  resetXY();
}

void SimulatedCamera::resetXY()
{
  m_x = m_y = 0;
  m_dx = (m_simulatedImage.ncols > (int)width());
  m_dy = (m_simulatedImage.nrows > (int)height());
}  

void SimulatedCamera::requestOneFrame()
{
  stopFrames();
  createAndProcessFrame();
}

void SimulatedCamera::requestContinuousFrames()
{
  int msec_per_frame = 250; // 4 frames per second
  requestOneFrame();
  m_timer.m_timer.start(msec_per_frame);
}

void SimulatedCamera::stopFrames()
{
  m_timer.m_timer.stop();
}

void SimulatedCamera::createAndProcessFrame()
{
  Image image(height(), width());
  image.load(m_simulatedImage, m_x, m_y);
  if (m_x + m_dx < 0 || m_x + m_dx + (int)width() > m_simulatedImage.ncols) m_dx = -m_dx;
  m_x += m_dx;
  if (m_y + m_dy < 0 || m_y + m_dy + (int)height() > m_simulatedImage.nrows) m_dy = -m_dy;
  m_y += m_dy;
  callFrameHandlers(image);
}

SimulatedCameraTimer::SimulatedCameraTimer(SimulatedCamera &camera)
  : m_camera(camera) {
  connect(&m_timer, SIGNAL(timeout()), this, SLOT(timeout()));
};

void SimulatedCameraTimer::timeout()
{
  m_camera.createAndProcessFrame();
}

