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

#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <errno.h>

#include "SerialPort.h"

SerialPort::SerialPort(QString filename, QObject *parent) : QIODevice(parent), m_name(filename), m_fd(-1)
{
}

SerialPort::~SerialPort()
{   
}

bool SerialPort::open(OpenMode mode)
{
    //qWarning"SerialPort::open");
   if(isOpen())
      return true;
   else if(mode == QIODevice::ReadWrite) {
      m_fd = ::open(m_name.toLocal8Bit(), O_RDWR);
      if(m_fd > 0) return QIODevice::open(ReadWrite);
   }
   else if(mode == QIODevice::ReadOnly) {
      m_fd = ::open(m_name.toLocal8Bit(), O_RDONLY);
      if(m_fd > 0) return QIODevice::open(ReadOnly);
   }
   else if(mode == QIODevice::WriteOnly) {
      m_fd = ::open(m_name.toLocal8Bit(), O_WRONLY);
      if(m_fd > 0) return QIODevice::open(WriteOnly);
   }
    
   return false;
}

void SerialPort::close()
{
    if(isOpen()) {
        ::close(m_fd);
        m_fd = -1;
        QIODevice::close();
    }
}

qint64 SerialPort::bytesAvailable() const
{
   //qWarning"SerialPort::bytesAvailable");
    if(isOpen()) {
        int byteCount = 0;
        if(ioctl(m_fd, FIONREAD, &byteCount) == 0)
            return byteCount + QIODevice::bytesAvailable();
        else
            return QIODevice::bytesAvailable();
    }
    return 0;
}

bool SerialPort::waitForReadyRead(int msecs)
{
    //qWarning"SerialPort::waitForReadyRead");
    if(isOpen()) {
        if(bytesAvailable()) return true;
        return select(msecs);
    }
    return false;
}

bool SerialPort::select(int msecs) const
{
    //qWarning"SerialPort::select m_fd = %d", m_fd);
    fd_set fds;
    struct timeval timeout;
    
    FD_ZERO(&fds);
    FD_SET(m_fd, &fds);
    
    timeout.tv_sec  = (msecs / 1000);
    timeout.tv_usec = (msecs % 1000) * 1000;
    
    //qWarning"::select");
    int n = ::select(m_fd + 1, &fds, 0, 0, &timeout);
    
    //qWarning"SerialPort::select return");
    if(n == 1) return true;
    return false;
}

qint64 SerialPort::readData(char *data, qint64 maxSize)
{
    //qWarning"SerialPort::readData");
    if(isOpen()) {
        //if(select(250)){
            //qWarning"::read");
            int ret = ::read(m_fd, data, maxSize);
            //qWarning"return");
            if(ret >= 0) return ret;
        //}
        //qWarning"return 0");
        return 0;
    }
    return -1;
}

qint64 SerialPort::writeData(const char *data, qint64 maxSize)
{
    //qWarning"SerialPort::writeData");
    if(isOpen()) {
        int ret = ::write(m_fd, data, maxSize);
        if(ret >= 0) return ret;
    }
    return -1;
}
