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
#include <QSettings>

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
  system("rmmod videodev");
  system("rmmod microdia");
  system("insmod /mnt/usb/videodev.ko");
  system("insmod /mnt/usb/microdia.ko max_urbs=20");

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
  int fd = open("/dev/video0", O_RDWR);
  
  if (fd < 0) 
    return false;
  
  m_fd = fd;
  
  struct v4l2_capability cap;
  if (ioctl (fd, VIDIOC_QUERYCAP, &cap) != 0) {
    perror("ioctl(VIDIOC_QUERYCAP)");
    return false;
  }
  // print out the capabilities of the camera: read/write/streaming/video capture
  //qWarning("%s",qPrintable(QString("capabilities = %1").arg(cap.capabilities,0,16)));

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

  this->readSettings();
  // print out the settings their locations, values, names, and max value
  //this->checkSettings();

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

      if (!m_processOneFrame && !m_processContinuousFrames) continue;
      if (len != buffer_size) printf("Error reading from camera:  expected %d bytes, got %d bytes\n", buffer_size, len);
      else
      {
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


void MicrodiaCamera::checkSettings()
{
    struct v4l2_control controls;
    struct v4l2_queryctrl queryctrl;
    struct v4l2_querymenu querymenu;

    memset (&queryctrl, 0, sizeof (queryctrl));

    for (queryctrl.id = V4L2_CID_BASE;
         queryctrl.id < V4L2_CID_LASTP1;
         queryctrl.id++) {
        if (0 == ioctl (m_fd, VIDIOC_QUERYCTRL, &queryctrl)) {
            //if (queryctrl.flags & V4L2_CTRL_FLAG_DISABLED)
              //  continue;
            qWarning("Control %s", qPrintable(QString("id %1 min %2 max %3").arg(queryctrl.id,0,16).arg(queryctrl.minimum).arg(queryctrl.maximum)));

            controls.id = queryctrl.id;
            if(0==ioctl(m_fd,VIDIOC_G_CTRL, &controls))
                qWarning("\t%s %s",queryctrl.name-3,qPrintable(QString("value %1").arg(controls.value)));
        }
    }

    for (queryctrl.id = V4L2_CID_PRIVATE_BASE;
         queryctrl.id < 24+V4L2_CID_PRIVATE_BASE;
         queryctrl.id++) {
        if (0 == ioctl (m_fd, VIDIOC_QUERYCTRL, &queryctrl)) {
            //if (queryctrl.flags & V4L2_CTRL_FLAG_DISABLED)
              //  continue;

            qWarning("PrivCtrl %s", qPrintable(QString("id %1 min %2 max %3").arg(queryctrl.id,0,16).arg(queryctrl.minimum).arg(queryctrl.maximum)));

            controls.id = queryctrl.id;
            if(0==ioctl(m_fd,VIDIOC_G_CTRL, &controls))
                qWarning("\t%s %s",queryctrl.name-3,qPrintable(QString("value %1").arg(controls.value)));

        }
    }

    for(controls.id=V4L2_CID_BASE;controls.id<24+V4L2_CID_BASE;controls.id++){

    }
}

int MicrodiaCamera::setParameter(enum cam_parms id, int value)
{
    struct v4l2_control ctrlParam;

    if(id <= 24)
        ctrlParam.id = id + V4L2_CID_BASE;
    else if(id == 25)
        ctrlParam.id = V4L2_CID_PRIVATE_BASE;
    else if(id == 26)
        ctrlParam.id = 1+V4L2_CID_PRIVATE_BASE;
    else return 0;

    ctrlParam.value = value;
    //qWarning("set %s", qPrintable(QString("id %1 value %2").arg(ctrlParam.id,0,16).arg(ctrlParam.value)));

    if(0 != ioctl(m_fd, VIDIOC_S_CTRL, &ctrlParam)){
        perror("VIDIOC_S_CTRL error");
        return 1;
    }
    this->writeSetting(id,value);
    return 0;
}

int MicrodiaCamera::getParameter(enum cam_parms id)
{
    struct v4l2_control ctrlParam;

    if(id <= 24)
        ctrlParam.id = id + V4L2_CID_BASE;
    else if(id == 25)
        ctrlParam.id = V4L2_CID_PRIVATE_BASE;
    else if(id == 26)
        ctrlParam.id = 1+V4L2_CID_PRIVATE_BASE;
    else return 0;

    if(0 != ioctl(m_fd, VIDIOC_G_CTRL, &ctrlParam)) perror("VIDIOC_G_CTRL error");
    //qWarning("get %s", qPrintable(QString("id %1 value %2").arg(ctrlParam.id,0,16).arg(ctrlParam.value)));
    return ctrlParam.value;
}

void MicrodiaCamera::readSettings()
{
    QSettings m_settings("/mnt/user/cbc_v2.config",QSettings::NativeFormat);

    //qWarning("%s",qPrintable(m_settings.fileName()));
    // reads in the Camera settings that have been saved to memory
    // if no settings file is located the defaults are input
    // write each of the saved settings to the Camera
        m_settings.beginGroup(QString("Camera"));
        this->setParameter(BRIGHTNESS,m_settings.value("Brightness",32767).toInt());
        this->setParameter(CONTRAST,m_settings.value("Contrast",32767).toInt());
        this->setParameter(AUTO_WHITE_BALANCE, m_settings.value("AutoBalance",true).toInt());
        this->setParameter(RED_BALANCE, m_settings.value("RedBalance",31).toInt());
        this->setParameter(BLUE_BALANCE, m_settings.value("BlueBalance",31).toInt());
        this->setParameter(GAMMA, m_settings.value("Gamma",13107).toInt());
        this->setParameter(EXPOSURE, m_settings.value("Exposure",3072).toInt());
        this->setParameter(H_FLIP, m_settings.value("H_flip",false).toInt());
        this->setParameter(V_FLIP, m_settings.value("V_flip",false).toInt());
        this->setParameter(SHARPNESS, m_settings.value("Sharpness",31).toInt());
        this->setParameter(AUTO_EXPOSURE, m_settings.value("AutoExposure",false).toInt());
        m_settings.endGroup();
        m_settings.sync();
        ::system("sync");
        ::system("sync");
}

void MicrodiaCamera::setDefaultParams()
{
    QSettings m_settings("/mnt/user/cbc_v2.config",QSettings::NativeFormat);
    // remove all camera settings from settings file
    m_settings.remove("Camera");
    // reset all to default values because the settings are not there
    this->readSettings();
}

void MicrodiaCamera::writeSetting(enum cam_parms id, int value)
{
    QString name;

    switch(id){
    case BRIGHTNESS:
        name = "Brightness";
        break;
    case CONTRAST:
        name = "Contrast";
        break;
    case AUTO_WHITE_BALANCE:
        name = "AutoBalance";
        break;
    case RED_BALANCE:
        name = "RedBalance";
        break;
    case BLUE_BALANCE:
        name = "BlueBalance";
        break;
    case GAMMA:
        name = "Gamma";
        break;
    case EXPOSURE:
        name = "Exposure";
        break;
    case H_FLIP:
        name = "H_flip";
        break;
    case V_FLIP:
        name = "V_flip";
        break;
    case SHARPNESS:
        name = "Sharpness";
        break;
    case AUTO_EXPOSURE:
        name = "AutoExposure";
        break;
    }

    QSettings m_settings("/mnt/user/cbc_v2.config",QSettings::NativeFormat);
    m_settings.beginGroup("Camera");
    m_settings.setValue(name,value);
    m_settings.endGroup();
    m_settings.sync();
    ::system("sync");
    ::system("sync");
}
