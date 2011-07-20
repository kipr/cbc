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
#include "CbobData.h"
#include "UserProgram.h"
#include <fcntl.h>

#include <QtGlobal>
#include <QtEndian>
#include <QBuffer>
#include <QDir>
#include <QProcess>
#include <QDebug>

SerialServer::SerialServer(QObject *parent) : QThread(parent), 
                                              m_port(SERIAL_DEVICE, this),
                                              m_stream(&m_port),
                                              m_quit(false),
						m_uiData("/tmp/cbc_uidata")
{
	QObject::connect(UserProgram::instance(), SIGNAL(started()), this, SLOT(clearText()));
	QObject::connect(UserProgram::instance(), SIGNAL(consoleOutput(QString)), this, SLOT(updateText(QString)));
}

SerialServer::~SerialServer() { stop(); }

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

void SerialServer::stop() { m_quit = true; while(isRunning()); }

bool SerialServer::sendCommand(quint16 command, const QByteArray& data)
{
	QByteArray compressedData = qCompress(data, 9);

	QList<QByteArray> dataChunks;
	while(compressedData.size()) {
		dataChunks.push_front(compressedData.right(64));
		compressedData.chop(64);
	}

	compressedData.squeeze();

	QByteArray header;
	QDataStream stream(&header, QIODevice::WriteOnly);
	stream << SERIAL_START;
	stream << (quint16)dataChunks.size();
	stream << command;

	dataChunks.push_front(header);
	
	qWarning() << "Writing" << command << "with" << data;

	for(int i = 0; i < dataChunks.size(); ++i) 
		if(!writePacket(dataChunks[i])) return false;
	
	return true;
}

void SerialServer::processTransfer(QByteArray& header)
{
	QDataStream headerStream(&header, QIODevice::ReadOnly);

	quint16 startWord;
	quint16 packetCount;
	quint16 command;

	headerStream >> startWord;
	headerStream >> packetCount;
	headerStream >> command;

	if(startWord != SERIAL_START) return;
	qWarning("StartWord: %x", startWord);

	qWarning() << "Reading Header for command" << command;
	QByteArray compressedData;
	
	for(quint16 i = 0;i < packetCount;i++) {
		QByteArray data;
		if(!readPacket(&data)) return;
		compressedData += data;
	}
	qWarning() << "Uncompressing...";

	QByteArray data = qUncompress(compressedData);
	compressedData.clear();
	compressedData.squeeze();
	
	processData(command, data);
}

void SerialServer::processData(quint16 command, QByteArray& data)
{
	qWarning() << "RECV" << command << data;
	
	switch(command) {
	case KISS_SEND_FILE_COMMAND: kissSendFileCommand(data); break;
	case KISS_RUN_COMMAND: kissRunCommand(data); break;
	case KISS_STOP_COMMAND: kissStopCommand(data); break;
	case KISS_COMPILE_COMMAND: kissCompileCommand(data); break;
	case KISS_CREATE_PROJECT_COMMAND: kissCreateProjectCommand(data); break;
	case KISS_PRESS_A_COMMAND: kissPressACommand(data); break;
	case KISS_PRESS_B_COMMAND: kissPressBCommand(data); break;
	case KISS_PRESS_LEFT_COMMAND: kissPressLeftCommand(data); break;
	case KISS_PRESS_RIGHT_COMMAND: kissPressRightCommand(data); break;
	case KISS_PRESS_UP_COMMAND: kissPressUpCommand(data); break;
	case KISS_PRESS_DOWN_COMMAND: kissPressDownCommand(data); break;
	case KISS_RELEASE_A_COMMAND: kissReleaseACommand(data); break;
	case KISS_RELEASE_B_COMMAND: kissReleaseBCommand(data); break;
	case KISS_RELEASE_LEFT_COMMAND: kissReleaseLeftCommand(data); break;
	case KISS_RELEASE_RIGHT_COMMAND: kissReleaseRightCommand(data); break;
	case KISS_RELEASE_UP_COMMAND: kissReleaseUpCommand(data); break;
	case KISS_RELEASE_DOWN_COMMAND: kissReleaseDownCommand(data); break;
	case KISS_GET_STATE_COMMAND: kissGetStateCommand(data); break;
	case KISS_GET_STDOUT_COMMAND: kissGetStdoutCommand(data); break;
	}
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
   qWarning("SerialServer::readPacket");
    while(1){
        QByteArray data;
        quint32 key = 0;
        quint16 checksum = 0xFFFF;
        
        qWarning("reading stream");
        m_stream >> key;
        qWarning("checking key");
        if (key == HEADER_KEY) {
            m_stream >> data;
            m_stream >> checksum;
            qWarning("data.size()=%d", data.size());
            qWarning("checksum=%x", checksum);
            if(checksum == qChecksum(data, data.size())) {
                *packetData = data;
                sendOk();
                return true;
            }
        }
        m_stream.skipRawData(512);
        m_stream.resetStatus();
        qWarning("Retry...");
        if(m_quit) return false;
    }
    return false;
}

void SerialServer::sendOk() { m_stream << SERIAL_MESSAGE_OK; }

bool SerialServer::checkOk()
{
	quint8 ret = 0;
	m_stream >> ret;
	return ret == SERIAL_MESSAGE_OK;
}

bool SerialServer::writePacket(QByteArray& data)
{
	m_stream.resetStatus();
	for(int i = 0; i < SERIAL_MAX_RETRY; ++i) {
		m_stream << SERIAL_KEY;
		
		qWarning("%x", SERIAL_KEY);
		m_stream << data;
		m_stream << qChecksum(data.constData(), data.size());
		if(checkOk()) return true;
	}
	
	return false;
}

void SerialServer::clearText() { text.clear(); }
void SerialServer::updateText(QString text) { this->text += text; }

void SerialServer::kissSendFileCommand(const QByteArray& data)
{
	qWarning() << "KISS_SEND_FILE_COMMAND";
	QString dest;
	QByteArray fileData;
	QDataStream stream(data);
	stream >> dest;
	stream >> fileData;
	qWarning() << dest;
	writeFile(dest, fileData);
}

void SerialServer::kissRequestFileCommand(const QByteArray& data)
{
		
}

void SerialServer::kissLsCommand(const QByteArray& data)
{
}

void SerialServer::kissRunCommand(const QByteArray& data)
{
	qWarning() << "KISS_RUN_COMMAND";
	emit runProgram();	
}

void SerialServer::kissStopCommand(const QByteArray& data)
{
	emit stopProgram();
}

void SerialServer::kissExecuteCommand(const QByteArray& data)
{
	system(data.data());
}

void SerialServer::kissCompileCommand(const QByteArray& data)
{
	qWarning() << "Emitting downloadFinished.";
	QProcess::startDetached("aplay /mnt/kiss/sounds/downloading.wav");
	emit downloadFinished(QString(data.data()));
}

void SerialServer::kissCreateProjectCommand(const QByteArray& data)
{
	QString projectPath = QString("/mnt/user/code/") + data.data();
	system(("rm -rf \""   + projectPath + "\"").toLocal8Bit());
	system(("mkdir -p \"" + projectPath + "\"").toLocal8Bit());
}

void SerialServer::kissPressACommand(const QByteArray& data) { m_uiData.shared().a_button = 1; }
void SerialServer::kissPressBCommand(const QByteArray& data) { m_uiData.shared().b_button = 1; }
void SerialServer::kissPressLeftCommand(const QByteArray& data) { m_uiData.shared().left_button = 1; }
void SerialServer::kissPressRightCommand(const QByteArray& data) { m_uiData.shared().right_button = 1; }
void SerialServer::kissPressUpCommand(const QByteArray& data) { m_uiData.shared().up_button = 1; }
void SerialServer::kissPressDownCommand(const QByteArray& data) { m_uiData.shared().down_button = 1; }
void SerialServer::kissReleaseACommand(const QByteArray& data) { m_uiData.shared().a_button = 0; }
void SerialServer::kissReleaseBCommand(const QByteArray& data) { m_uiData.shared().b_button = 0; }
void SerialServer::kissReleaseLeftCommand(const QByteArray& data) { m_uiData.shared().left_button = 0; }
void SerialServer::kissReleaseRightCommand(const QByteArray& data) { m_uiData.shared().right_button = 0; }
void SerialServer::kissReleaseUpCommand(const QByteArray& data) { m_uiData.shared().up_button = 0; }
void SerialServer::kissReleaseDownCommand(const QByteArray& data) { m_uiData.shared().down_button = 0; }

void SerialServer::kissGetStateCommand(const QByteArray& data)
{
	QMap<QString, QString> state;
	
	state["a_button"] = QString::number(m_uiData.shared().a_button);
	state["b_button"] = QString::number(m_uiData.shared().b_button);
	state["left_button"] = QString::number(m_uiData.shared().left_button);
	state["right_button"] = QString::number(m_uiData.shared().right_button);
	state["up_button"] = QString::number(m_uiData.shared().up_button);
	state["down_button"] = QString::number(m_uiData.shared().down_button);
	state["analog 0"] = QString::number(CbobData::instance()->analog(0));
	state["analog 1"] = QString::number(CbobData::instance()->analog(1));
	state["analog 2"] = QString::number(CbobData::instance()->analog(2));
	state["analog 3"] = QString::number(CbobData::instance()->analog(3));
	state["analog 4"] = QString::number(CbobData::instance()->analog(4));
	state["analog 5"] = QString::number(CbobData::instance()->analog(5));
	state["analog 6"] = QString::number(CbobData::instance()->analog(6));
	state["analog 7"] = QString::number(CbobData::instance()->analog(7));
	state["digital 0"] = QString::number(CbobData::instance()->digital(0));
	state["digital 1"] = QString::number(CbobData::instance()->digital(1));
	state["digital 2"] = QString::number(CbobData::instance()->digital(2));
	state["digital 3"] = QString::number(CbobData::instance()->digital(3));
	state["digital 4"] = QString::number(CbobData::instance()->digital(4));
	state["digital 5"] = QString::number(CbobData::instance()->digital(5));
	state["digital 6"] = QString::number(CbobData::instance()->digital(6));
	state["digital 7"] = QString::number(CbobData::instance()->digital(7));
	state["motor 0"] = QString::number(CbobData::instance()->motorPWM(0));
	state["motor 1"] = QString::number(CbobData::instance()->motorPWM(1));
	state["motor 2"] = QString::number(CbobData::instance()->motorPWM(2));
	state["motor 3"] = QString::number(CbobData::instance()->motorPWM(3));
	state["motor 0 position"] = QString::number(CbobData::instance()->motorPosition(0));
	state["motor 1 position"] = QString::number(CbobData::instance()->motorPosition(1));
	state["motor 2 position"] = QString::number(CbobData::instance()->motorPosition(2));
	state["motor 3 position"] = QString::number(CbobData::instance()->motorPosition(3));
	state["accel x"] = QString::number(CbobData::instance()->accelerometerX());
	state["accel y"] = QString::number(CbobData::instance()->accelerometerY());
	state["accel z"] = QString::number(CbobData::instance()->accelerometerZ());
	state["battery"] = QString::number(CbobData::instance()->batteryVoltage(),'f',3);
	
	QByteArray mapData;
	QDataStream mapStream(&mapData, QIODevice::WriteOnly);
	mapStream << state;
	sendCommand(CBC_STATE_RESULT, mapData);
}

void SerialServer::kissGetStdoutCommand(const QByteArray& data) { sendCommand(CBC_STDOUT_RESULT, text.toAscii()); text.clear(); }

