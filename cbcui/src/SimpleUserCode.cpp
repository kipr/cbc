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

#include "SimpleUserCode.h"

#include <QFileInfo>
#include <QScrollBar>

SimpleUserCode::SimpleUserCode(QWidget *parent) :
        QWidget(parent)
{
    setupUi(this);

#ifdef QT_ARCH_ARM
    setWindowState(windowState() | Qt::WindowFullScreen);
#endif

    QObject::connect(&m_compiler, SIGNAL(readyReadStandardError()), this, SLOT(readStandardError()));
    QObject::connect(&m_compiler, SIGNAL(readyReadStandardOutput()), this, SLOT(readStandardOutput()));
}

SimpleUserCode::~SimpleUserCode()
{
}

void SimpleUserCode::on_CompileButton_clicked()
{
    if(m_compiler.state() == QProcess::NotRunning) {
        emit stop();
        ui_Output->clear();
        m_compiler.start("/mnt/kiss/usercode/compile-usb");
    }
}

void SimpleUserCode::on_RunButton_clicked()
{
    emit run();
    close();
}

void SimpleUserCode::on_StopButton_clicked()
{
    emit stop();
    close();
}

void SimpleUserCode::readStandardError()
{
    ui_Output->insertPlainText(QString(m_compiler.readAllStandardError()));
    ui_Output->verticalScrollBar()->triggerAction(QScrollBar::SliderToMaximum);
}

void SimpleUserCode::readStandardOutput()
{
    ui_Output->insertPlainText(QString(m_compiler.readAllStandardOutput()));
    ui_Output->verticalScrollBar()->triggerAction(QScrollBar::SliderToMaximum);
}

void SimpleUserCode::compileFile(QString filename)
{
    if(m_compiler.state() == QProcess::NotRunning) {
        emit stop();
        ui_Output->clear();
        m_compiler.start("/mnt/kiss/usercode/compile", QStringList() << filename);
    }
}
