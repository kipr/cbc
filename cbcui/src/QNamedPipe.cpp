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

#include "QNamedPipe.h"

#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <errno.h>

QNamedPipe::QNamedPipe(QString filename, QObject *parent) : QIODevice(parent), m_name(filename), m_fd(-1)
{
}

QNamedPipe::~QNamedPipe()
{   
}

bool QNamedPipe::open(OpenMode mode)
{
    if(isOpen())
        return true;
    else if(mode == QIODevice::ReadOnly)
        m_fd = ::open(m_name.toLocal8Bit(), O_RDONLY);
    else if(mode == QIODevice::WriteOnly)
        m_fd = ::open(m_name.toLocal8Bit(), O_WRONLY);
    
    if(m_fd > 0)
        return QIODevice::open(ReadWrite);
    
    return false;
}

void QNamedPipe::close()
{
    if(isOpen()) {
        ::close(m_fd);
        m_fd = -1;
        QIODevice::close();
    }
}

qint64 QNamedPipe::bytesAvailable() const
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

bool QNamedPipe::waitForReadyRead(int msecs)
{
    if(isOpen()) {
        if(bytesAvailable()) return true;
        return select(msecs);
    }
    return false;
}

bool QNamedPipe::select(int msecs) const
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

qint64 QNamedPipe::readData(char *data, qint64 maxSize)
{
    if(isOpen()) {
        if(select(250)){
            int ret = ::read(m_fd, data, maxSize);
            if(ret >= 0) return ret;
        }
        return 0;
    }
    return -1;
}

qint64 QNamedPipe::writeData(const char *data, qint64 maxSize)
{
    if(isOpen()) {
        int ret = ::write(m_fd, data, maxSize);
        if(ret >= 0) return ret;
    }
    return -1;
}
