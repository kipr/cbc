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
#define SERIAL2_START       ((quint16)0xCBC2)

#define SERIAL_MAX_RETRY 5
#define SERIAL_KEY (quint32)(0xB07BA11)
#define HEADER_KEY (quint32)(0xB07BA11)
#define SERIAL_DEVICE "/dev/uart0"
#define TEMP_PATH "/tmp/upload"

#define KISS_SEND_FILE_COMMAND 		1
#define KISS_REQUEST_FILE_COMMAND 	2
#define KISS_LS_COMMAND 		3
#define KISS_RUN_COMMAND 		4
#define KISS_STOP_COMMAND 		5
#define KISS_EXECUTE_COMMAND 		6
#define KISS_COMPILE_COMMAND 		7
#define KISS_CREATE_PROJECT_COMMAND 	8

#define CBC_LS_RESULT 			128
#define CBC_EXECUTE_RESULT 		129
#define CBC_COMPILE_RESULT 		130

class SerialServer : public QThread
{
    Q_OBJECT

public:
	SerialServer(QObject *parent = 0);
	~SerialServer();

	void run();
	void stop();
    
signals:
	void runProgram();
	void stopProgram();
 	void downloadFinished(QString filename);
    
private:
	SerialPort m_port;
	QDataStream m_stream;
	bool  m_quit;

	bool readPacket(QByteArray *packetData);

	void processTransfer(QByteArray& header);
	void processData(quint16 command, QByteArray& data);
	void writeFile(QString fileName, QByteArray& fileData);
	void sendOk();
	bool checkOk();
	bool writePacket(QByteArray& data);
	
	void kissSendFileCommand(const QByteArray& data);
	void kissRequestFileCommand(const QByteArray& data);
	void kissLsCommand(const QByteArray& data);
	void kissRunCommand(const QByteArray& data);
	void kissStopCommand(const QByteArray& data);
	void kissExecuteCommand(const QByteArray& data);
	void kissCompileCommand(const QByteArray& data);
	void kissCreateProjectCommand(const QByteArray& data);
};

#endif
