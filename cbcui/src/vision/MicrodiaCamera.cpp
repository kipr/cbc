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
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>

// Local includes
#include "ctdebug.h"

// Self
#include "MicrodiaCamera.h"

void MicrodiaCameraThread::run() {
  m_camera.backgroundLoop();
}
  
MicrodiaCamera::MicrodiaCamera()
  : Camera(160, 120),
    m_processOneFrame(false), 
    m_processContinuousFrames(false),
    m_exit(false),
    m_fd(-1),
    m_thread(*this)
{
  printf("Constructing MicrodiaCamera\n");
  system("mknod /dev/video0 c 81 0");
  system("insmod /mnt/usb/videodev.ko");
  system("rmmod microdia");

  int exposure=1000;
  char buf[100];
  printf("Using exposure %d\n", exposure);
  sprintf(buf, "insmod /mnt/usb/microdia.ko max_urbs=20 exposure=%d",
          exposure);
  system(buf);

  sleep(1);
  m_thread.start();
}

MicrodiaCamera::~MicrodiaCamera() {
  m_exit = true;
  // Wait for thread to exit
  m_thread.wait();
}

void MicrodiaCamera::requestOneFrame()
{
  m_processOneFrame = true;
  m_processContinuousFrames = false;
}

void MicrodiaCamera::requestContinuousFrames()
{
  m_processContinuousFrames = true;
  m_processOneFrame = false;
}

void MicrodiaCamera::stopFrames()
{
  m_processContinuousFrames = false;
  m_processOneFrame = false;
}

bool MicrodiaCamera::openCamera()
{
  if (m_fd >= 0) {
    close(m_fd);
    m_fd = -1;
  }
  //printf("MicrodiaCamera::openCamera\n");
      
  int fd = open("/dev/video0", O_RDWR);
  
  if (fd < 0) {
    //perror("open /dev/video0");
    return false;
  }
  
  m_fd = fd;
  
  struct v4l2_capability cap;
  if (ioctl (fd, VIDIOC_QUERYCAP, &cap) != 0) {
    perror("ioctl(VIDIOC_QUERYCAP)");
    return false;
  }
  
  struct v4l2_format fmt;
  memset(&fmt, 0, sizeof(fmt));
  
  fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  fmt.fmt.pix.width       = width();
  fmt.fmt.pix.height      = height();
  fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_BGR24;
  fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;

  if(ioctl(fd, VIDIOC_S_FMT, &fmt) != 0) {
    perror("iocts(VIDIOC_S_FMT)");
    return false;
  }

  printf("Camera device opened successfully\n");
  return true;
}

void MicrodiaCamera::backgroundLoop()
{
  printf("Starting MicrodiaCamera::backgroundLoop()\n");
  check_heap();
  int buffer_size  = width() * height() * 3;
  unsigned char buffer[buffer_size];
  Image image(height(), width());
  
  while (1) {
    // Repeatedly try to open camera
    while (1) {
      check_heap();
      if (m_exit) goto done;
      if (openCamera()) break;
      sleep(2);
    }

    int consecutive_readerrs=0;
    while (1) {
      check_heap();
      if (m_exit) goto done;
      int len = read (m_fd, buffer, buffer_size);
      if (len == -1) {
        consecutive_readerrs++;
        if (consecutive_readerrs >= 10) {
          printf("%d consecutive read errors:  try to reopen camera\n",
                 consecutive_readerrs);
          // Break from loop and try to reopen camera
          break;
        }
      } else {
        consecutive_readerrs=0;
      }
      if (m_exit) goto done;

      if (!m_processOneFrame && !m_processContinuousFrames) continue;
      if (len != buffer_size) {
        printf("Error reading from camera:  expected %d bytes, got %d bytes\n", buffer_size, len);
      } else {
        check_heap();
        m_processOneFrame = false;
        //printf("Got frame from camera, len=%d\n", len);
        // Copy to image
        Pixel565 *out = image.scanLine(0);
        unsigned char *in = buffer;
        for (int i = width() * height(); i > 0; i--) {
          *(out++) = Pixel565::fromRGB8(in[2], in[1], in[0]);
          in += 3;
        }
        check_heap();
      }
      callFrameHandlers(image);
    }
  }
  done:
  printf("Exiting MicrodiaCamera::backgroundLoop()\n");
}


