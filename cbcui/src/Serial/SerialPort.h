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

#ifndef __SERIAL_PORT_H__
#define __SERIAL_PORT_H__

/* Currenty hard-wired for ReadWrite at 38400 */
#include <termios.h>
#include <QIODevice>
#include <QString>

class SerialPort : public QIODevice
{
Q_OBJECT
public:
    SerialPort(QString filename, QObject *parent = 0);
    ~SerialPort();
    
    bool open(OpenMode mode = 0);
    void close();
    void setBaudRate(int baud);
    int getBaudRate();
    QString getPortName() { return m_name; }

    qint64 bytesAvailable() const;
    bool   waitForReadyRead(int msecs);
    
    bool isSequential() { return true; }

protected:
    qint64 readData(char *data, qint64 maxSize);
    qint64 writeData(const char *data, qint64 maxSize);
    
private:
    QString m_name;
    QString m_linkName;
    int m_fd;
    speed_t m_baudRate;
    
    void configure();
    bool select(int msecs) const;
};

#endif

