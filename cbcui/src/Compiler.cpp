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

#include "Compiler.h"

#include <QFileInfo>
#include <QScrollBar>

#include "UserProgram.h"

Compiler::Compiler(QWidget *parent) : Page(parent), m_serial(this)
{
	setupUi(this);

	QObject::connect(&m_compiler, SIGNAL(readyReadStandardError()), this, SLOT(readStandardError()));
	QObject::connect(&m_compiler, SIGNAL(readyReadStandardOutput()), this, SLOT(readStandardOutput()));
	QObject::connect(&m_compiler, SIGNAL(finished(int, QProcess::ExitStatus)), UserProgram::instance(), SLOT(compileFinished(int, QProcess::ExitStatus)));
	QObject::connect(&m_serial, SIGNAL(downloadFinished(QString)), this, SLOT(compileFile(QString)));
	QObject::connect(&m_serial, SIGNAL(runProgram()), UserProgram::instance(), SLOT(start()));
	QObject::connect(&m_serial, SIGNAL(stopProgram()), UserProgram::instance(), SLOT(stop()));

	m_serial.start();
}

Compiler::~Compiler()
{
   m_serial.stop();
}

// Deprecated?
void Compiler::compileFromUSB()
{
    if(m_compiler.state() == QProcess::NotRunning) {
        //qWarning("compile from usb");
        UserProgram::instance()->stop();
        ui_output->clear();
        m_compiler.start("/mnt/kiss/usercode/compile-usb");
    }
}

void Compiler::readStandardError()
{
    //qWarning("readStandardError");
    ui_output->insertPlainText(QString(m_compiler.readAllStandardError()));
    ui_output->verticalScrollBar()->triggerAction(QScrollBar::SliderToMaximum);
}

void Compiler::readStandardOutput()
{
    //qWarning("readStandardOutput()");
    ui_output->insertPlainText(QString(m_compiler.readAllStandardOutput()));
    ui_output->verticalScrollBar()->triggerAction(QScrollBar::SliderToMaximum);
}

void Compiler::compileFile(QString filename)
{
    //qWarning("compileFile");
    raisePage();
    if(m_compiler.state() == QProcess::NotRunning) {
        UserProgram::instance()->stop();
        UserProgram::instance()->loading();
        ui_output->clear();
        
        if(filename.endsWith(".sh"))
            m_compiler.start("sh", QStringList() << filename);
        else
            m_compiler.start("/mnt/kiss/usercode/compilers/" + filename.section(".", -1), QStringList() << filename);
    }
}
