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
		dataChunks.push_front(compressedData.right(128));
		compressedData.chop(128);
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
	bool old = false;

	quint16 startWord;
	quint16 packetCount;
	quint16 command;

	headerStream >> startWord;
	headerStream >> packetCount;
	if(headerStream.atEnd()) old = true;
	else headerStream >> command;

	if(startWord != SERIAL_START) return;

	QByteArray compressedData;
	
	for(quint16 i = 0;i < packetCount;i++) {
		QByteArray data;
		if(!readPacket(&data)) return;
		compressedData += data;
	}

	QByteArray data = qUncompress(compressedData);
	compressedData.clear();
	compressedData.squeeze();
	
	if(!old) processData(command, data); else processDataOld(data);
}

void SerialServer::processData(quint16 command, QByteArray& data)
{
	qWarning() << "RECV" << command << data;
	
	switch(command) {
	case KISS_SEND_FILE_COMMAND: kissSendFileCommand(data); break;
	case KISS_REQUEST_FILE_COMMAND: kissRequestFileCommand(data); break;
	case KISS_LS_COMMAND: kissLsCommand(data); break;
	case KISS_RUN_COMMAND: kissRunCommand(data); break;
	case KISS_STOP_COMMAND: kissStopCommand(data); break;
	case KISS_EXECUTE_COMMAND: kissExecuteCommand(data); break;
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
	case KISS_DELETE_FILE_COMMAND: kissDeleteFileCommand(data); break;
	case KISS_MKDIR_COMMAND: kissMkdirCommand(data); break;
	}
}

void SerialServer::processDataOld(QByteArray& data)
{
	
	qWarning() << "RECV OLD" << data;

	QDataStream dataStream(&data, QIODevice::ReadOnly);

	QString fileName;
	QByteArray fileData;

	dataStream >> fileName;
	dataStream >> fileData;

	if(fileName.isEmpty()) return;

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

QString SerialServer::createProject(const QString& projectName)
{
	const QString& projectPath = "/mnt/user/code/" + projectName;

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
    while(1){
        QByteArray data;
        quint32 key = 0;
        quint16 checksum = 0xFFFF;
       
        m_stream >> key;
        if (key == HEADER_KEY) {
            m_stream >> data;
            m_stream >> checksum;
            qWarning("data.size()=%d", data.size());
            if(checksum == qChecksum(data, data.size())) {
                *packetData = data;
                sendOk();
                return true;
            }
        }
        m_stream.skipRawData(1024);
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
	QFile fin(data.data());
	fin.open(QIODevice::ReadOnly);
	sendCommand(CBC_REQUEST_FILE_RESULT, fin.readAll());
	fin.close();
}

void SerialServer::kissLsCommand(const QByteArray& filename)
{
	QByteArray data;
	QDataStream stream(&data, QIODevice::WriteOnly);
	stream << QDir(QString(filename)).entryList(QDir::Files);
	sendCommand(CBC_LS_RESULT, data);
}

void SerialServer::kissRunCommand(const QByteArray& data) { emit runProgram(); }
void SerialServer::kissStopCommand(const QByteArray& data) { emit stopProgram(); }
void SerialServer::kissExecuteCommand(const QByteArray& data) { system(data.data()); }

void SerialServer::kissCompileCommand(const QByteArray& data)
{
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
	
	state["A Button"] = QString::number(m_uiData.shared().a_button);
	state["B Button"] = QString::number(m_uiData.shared().b_button);
	state["Left Button"] = QString::number(m_uiData.shared().left_button);
	state["Right Button"] = QString::number(m_uiData.shared().right_button);
	state["Up Button"] = QString::number(m_uiData.shared().up_button);
	state["Down Button"] = QString::number(m_uiData.shared().down_button);
	state["Analog 0"] = QString::number(CbobData::instance()->analog(0));
	state["Analog 1"] = QString::number(CbobData::instance()->analog(1));
	state["Analog 2"] = QString::number(CbobData::instance()->analog(2));
	state["Analog 3"] = QString::number(CbobData::instance()->analog(3));
	state["Analog 4"] = QString::number(CbobData::instance()->analog(4));
	state["Analog 5"] = QString::number(CbobData::instance()->analog(5));
	state["Analog 6"] = QString::number(CbobData::instance()->analog(6));
	state["Analog 7"] = QString::number(CbobData::instance()->analog(7));
	state["Digital 0"] = QString::number(CbobData::instance()->digital(0));
	state["Digital 1"] = QString::number(CbobData::instance()->digital(1));
	state["Digital 2"] = QString::number(CbobData::instance()->digital(2));
	state["Digital 3"] = QString::number(CbobData::instance()->digital(3));
	state["Digital 4"] = QString::number(CbobData::instance()->digital(4));
	state["Digital 5"] = QString::number(CbobData::instance()->digital(5));
	state["Digital 6"] = QString::number(CbobData::instance()->digital(6));
	state["Digital 7"] = QString::number(CbobData::instance()->digital(7));
	state["Motor 0"] = QString::number(CbobData::instance()->motorPWM(0));
	state["Motor 1"] = QString::number(CbobData::instance()->motorPWM(1));
	state["Motor 2"] = QString::number(CbobData::instance()->motorPWM(2));
	state["Motor 3"] = QString::number(CbobData::instance()->motorPWM(3));
	state["Motor 0 Position"] = QString::number(CbobData::instance()->motorPosition(0));
	state["Motor 1 Position"] = QString::number(CbobData::instance()->motorPosition(1));
	state["Motor 2 Position"] = QString::number(CbobData::instance()->motorPosition(2));
	state["Motor 3 Position"] = QString::number(CbobData::instance()->motorPosition(3));
	state["Accel x"] = QString::number(CbobData::instance()->accelerometerX());
	state["Accel y"] = QString::number(CbobData::instance()->accelerometerY());
	state["Accel z"] = QString::number(CbobData::instance()->accelerometerZ());
	state["Battery"] = QString::number(CbobData::instance()->batteryVoltage(),'f',3);
	
	QByteArray mapData;
	QDataStream mapStream(&mapData, QIODevice::WriteOnly);
	mapStream << state;
	sendCommand(CBC_STATE_RESULT, mapData);
}

void SerialServer::kissGetStdoutCommand(const QByteArray& data) { sendCommand(CBC_STDOUT_RESULT, text.toAscii()); text.clear(); }
void SerialServer::kissDeleteFileCommand(const QByteArray& data) { QFile::remove(QString(data)); }
void SerialServer::kissMkdirCommand(const QByteArray& data) { QDir().mkpath(QString(data)); }
