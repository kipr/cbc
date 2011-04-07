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

#include "SerialServer.h"
#include <fcntl.h>

#include <QtGlobal>
#include <QtEndian>
#include <QBuffer>
#include <QDir>
#include <QProcess>

SerialServer::SerialServer(QObject *parent) : QThread(parent), 
                                              m_port(SERIAL_DEVICE, this),
                                              m_stream(&m_port),
                                              m_quit(false)
{
}

SerialServer::~SerialServer()
{
    stop();
}

void SerialServer::run()
{
    m_port.open(QIODevice::ReadWrite);
    mkdir(TEMP_PATH, 0);

    //qWarning("SerialServer::run() looping");
    QByteArray header;
    while (!m_quit) {
        header.clear();
        if(readPacket(&header))
        {
            processTransfer(header);
        }
    }
    m_quit = false;
    m_port.close();
}

void SerialServer::stop()
{
    m_quit = true;
    while(isRunning());
}

void SerialServer::processTransfer(QByteArray& header)
{
    QDataStream headerStream(&header, QIODevice::ReadOnly);
    
    quint16 startWord;
    quint16 packetCount;
    
    headerStream >> startWord;
    headerStream >> packetCount;
    
    if(startWord != SERIAL_START)
        return;
    
    QByteArray compressedData;
    QProcess::startDetached("aplay /mnt/kiss/sounds/downloading.wav");
    for(quint16 i = 0;i < packetCount;i++) {
        QByteArray data;
        if(!readPacket(&data))
            return;
        compressedData += data;
    }
    
    
    QByteArray data = qUncompress(compressedData);
    compressedData.clear();
    compressedData.squeeze();
    
    processData(data);
}

void SerialServer::processData(QByteArray& data)
{
    QDataStream dataStream(&data, QIODevice::ReadOnly);
    
    QString fileName;
    QByteArray fileData;
    
    dataStream >> fileName;
    dataStream >> fileData;
    
    if(fileName.isEmpty())
        return;
    
    QFileInfo fileInfo(fileName);
    
    QString projectPath = createProject(fileInfo.baseName());
    writeFile(projectPath + "/" + fileName, fileData);
    
    QString mainFilePath = projectPath + "/" + fileName;
    
    while(dataStream.status() == QDataStream::Ok) {
        fileName.clear();
        fileData.clear();
        
        dataStream >> fileName;
        dataStream >> fileData;
        
        if(!fileName.isEmpty()) {
            writeFile(projectPath + "/" + fileName, fileData);
        }
    }
    emit downloadFinished(mainFilePath);
}

QString SerialServer::createProject(QString projectName)
{
    QString projectPath = "/mnt/user/code/" + projectName;
    
    system(("rm -rf \""   + projectPath + "\"").toLocal8Bit());
    system(("mkdir -p \"" + projectPath + "\"").toLocal8Bit());
    
    return projectPath;
}

void SerialServer::writeFile(QString fileName, QByteArray& fileData)
{
    QFile fout(fileName);
    fout.open(QIODevice::WriteOnly);
    fout.write(fileData);
    fout.close();
}

/* Packets look like this:
        quint32     key = 4 bytes
        QByteArray  data = n bytes
        quint16     checksum = 2 bytes */
bool SerialServer::readPacket(QByteArray *packetData)
{
   //qWarning("SerialServer::readPacket");
    while(1){
        QByteArray data;
        quint32 key = 0;
        quint16 checksum = 0xFFFF;
        
        //qWarning("reading stream");
        m_stream >> key;
        //qWarning("checking key");
        if (key == HEADER_KEY) {
            m_stream >> data;
            m_stream >> checksum;
            //qWarning("data.size()=%d", data.size());
            //qWarning("checksum=%x", checksum);
            if(checksum == qChecksum(data, data.size())) {
                *packetData = data;
                sendOk();
                return true;
            }
        }
        m_stream.skipRawData(512);
        m_stream.resetStatus();
        //qWarning("Retry...");
        if(m_quit) return false;
    }
    return false;
}

void SerialServer::sendOk()
{
    m_stream << SERIAL_MESSAGE_OK;
}
