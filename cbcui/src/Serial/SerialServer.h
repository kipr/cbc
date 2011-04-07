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

#ifndef __CBC_SERIAL_SERVER_H__
#define __CBC_SERIAL_SERVER_H__

#include <QThread>
#include <QFile>
#include <QStringList>
#include <QVector>
#include <QDataStream>

#include "SerialPort.h"

typedef struct {
    QStringList filenames;
    QVector<int> filesizes;
} CBCSerialHeader;

#define SERIAL_MESSAGE_OK   ((quint8)1)
#define SERIAL_MESSAGE_FAIL ((quint8)2)
#define SERIAL_START        ((quint16)0xCBC)

#define HEADER_KEY (quint32)(0xB07BA11)
#define SERIAL_DEVICE "/dev/uart0"
#define TEMP_PATH "/tmp/upload"

class SerialServer : public QThread
{
    Q_OBJECT

public:
    SerialServer(QObject *parent = 0);
    ~SerialServer();

    void run();
    void stop();
    
signals:
    void downloadFinished(QString filename);
    
private:
    SerialPort m_port;
    QDataStream m_stream;
    bool  m_quit;

    bool readPacket(QByteArray *packetData);

    void processTransfer(QByteArray& header);
    void processData(QByteArray& data);
    void writeFile(QString fileName, QByteArray& fileData);
    void sendOk();
    
    QString createProject(QString projectName);
};

#endif
