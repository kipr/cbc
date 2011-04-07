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

void SerialPort::configure()
{
    if(isOpen()){
        struct ::termios portdata;
        tcgetattr(m_fd, &portdata);

        portdata.c_oflag = 0;               // no output options
        portdata.c_lflag = 0;               // no local options
        portdata.c_iflag = IGNBRK | IGNPAR; // ignore break cond. and parity errors
        // portdata.c_cflag &= ~CRTSCTS;       // disable flow control
        portdata.c_cflag &= ~PARENB;        // disable parity
        portdata.c_cflag &= ~CSTOPB;        // 1 stop bit
        portdata.c_cflag &= ~CSIZE;         // no mask
        portdata.c_cflag |= CS8;            // 8 data bits
        portdata.c_cflag |= CREAD;
        portdata.c_cflag |= CLOCAL;
        portdata.c_cc[VMIN] = 1;            // minimum # of chars to read
        portdata.c_cc[VTIME] = 0;           // time to wait in tenths

        cfsetispeed(&portdata, m_baudRate);
        cfsetospeed(&portdata, m_baudRate);
        tcsetattr(m_fd, TCSANOW, &portdata);
    }
    else
        qWarning("Port not opened");
}

void SerialPort::setBaudRate(int baud)
{
    switch(baud){
    case 1200:
        m_baudRate = B1200;
        break;
    case 1800:
        m_baudRate = B1800;
        break;
    case 2400:
        m_baudRate = B2400;
        break;
    case 4800:
        m_baudRate = B4800;
        break;
    case 9600:
        m_baudRate = B9600;
        break;
    case 19200:
        m_baudRate = B19200;
        break;
    case 38400:
        m_baudRate = B38400;
        break;
    case 57600:
        m_baudRate = B57600;
        break;
    case 115200:
        m_baudRate = B115200;
        break;
    default:
        m_baudRate = B57600;
    }
    this->configure();
}

int SerialPort::getBaudRate()
{
    if(isOpen()){
        struct ::termios portdata;
        tcgetattr(m_fd, &portdata);
        speed_t baud = cfgetospeed(&portdata);

        switch(baud){
        case B1200:
            return 1200;
        case B1800:
            return 1800;
        case B2400:
            return 2400;
        case B4800:
            return 4800;
        case B9600:
            return 9600;
        case B19200:
            return 19200;
        case B38400:
            return 38400;
        case B57600:
            return 57600;
        case B115200:
            return 115200;
        default:
            return 0;
        }
    }
    else{
        qWarning("Port not open");
    }
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
    if(isOpen()) {
        if(bytesAvailable()) return true;
        return select(msecs);
    }
    return false;
}

bool SerialPort::select(int msecs) const
{
    fd_set fds;
    struct timeval timeout;
    
    FD_ZERO(&fds);
    FD_SET(m_fd, &fds);
    
    timeout.tv_sec  = (msecs / 1000);
    timeout.tv_usec = (msecs % 1000) * 1000;
    
    int n = ::select(m_fd + 1, &fds, 0, 0, &timeout);

    if(n == 1) return true;
    return false;
}

qint64 SerialPort::readData(char *data, qint64 maxSize)
{
    if(isOpen())
    {
        int ret = ::read(m_fd, data, maxSize);
        if(ret >= 0)
            return ret;
        return 0;
    }
    return -1;
}

qint64 SerialPort::writeData(const char *data, qint64 maxSize)
{
    if(isOpen()) {
        int ret = ::write(m_fd, data, maxSize);
        if(ret >= 0)
            return ret;
    }
    return -1;
}
