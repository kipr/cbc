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

#include "Console.h"

#include <QFileInfo>
#include <QScrollBar>

Console::Console(QWidget *parent) : QDialog(parent), m_data("/tmp/cbc_data")
{
    setupUi(this);

#ifdef QT_ARCH_ARM
    setWindowState(windowState() | Qt::WindowFullScreen);
#endif

    QObject::connect(&m_userProgram, SIGNAL(readyReadStandardError()), this, SLOT(readStandardError()));
    QObject::connect(&m_userProgram, SIGNAL(readyReadStandardOutput()), this, SLOT(readStandardOutput()));

    QObject::connect(&m_userProgram, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(userProgramFinished(int, QProcess::ExitStatus)));
}

Console::~Console()
{
}

void Console::readStandardError()
{
    updateText(QString(m_userProgram.readAllStandardError()));
}

void Console::readStandardOutput()
{
    updateText(QString(m_userProgram.readAllStandardOutput()));
}

void Console::updateText(QString text)
{
    m_consoleData += text;
    m_consoleData = m_consoleData.right(CONSOLE_MAX_LENGTH);
    ui_Output->setPlainText(m_consoleData);
    ui_Output->verticalScrollBar()->triggerAction(QScrollBar::SliderToMaximum);
}

void Console::clearText()
{
    m_consoleData.clear();
    ui_Output->setPlainText(m_consoleData);
}

void Console::on_ui_RunButton_clicked(bool)
{
    QFileInfo robot("/mnt/user/bin/robot");

    if (robot.exists()) {
        if (m_userProgram.state() == QProcess::NotRunning) {
            clearText();
            m_userProgram.start("/mnt/user/bin/robot");
        }
        else
            updateText("Program already running...\n");
    }
    else {
        updateText("Program not compiled!\n");
    }
}

void Console::on_ui_StopButton_clicked(bool)
{
    if (m_userProgram.state() != QProcess::NotRunning)
        m_userProgram.kill();
}

void Console::userProgramFinished(int , QProcess::ExitStatus)
{
    int i;

    for (i = 0;i < 4;i++) {
        m_data.shared().motor_pwm[i] = 0;
        m_data.shared().motor_tps[i] = 0;
    }
}

void Console::on_ui_UpButton_pressed()
{
    m_data.shared().up = 1;
}

void Console::on_ui_UpButton_released()
{
    m_data.shared().up = 0;
}

void Console::on_ui_DownButton_pressed()
{
    m_data.shared().down = 1;
}

void Console::on_ui_DownButton_released()
{
    m_data.shared().down = 0;
}

void Console::on_ui_LeftButton_pressed()
{
    m_data.shared().left = 1;
}

void Console::on_ui_LeftButton_released()
{
    m_data.shared().left = 0;
}

void Console::on_ui_RightButton_pressed()
{
    m_data.shared().right = 1;
}

void Console::on_ui_RightButton_released()
{
    m_data.shared().right = 0;
}

void Console::on_ui_AButton_pressed()
{
    m_data.shared().a = 1;
}

void Console::on_ui_AButton_released()
{
    m_data.shared().a = 0;
}

void Console::on_ui_BButton_pressed()
{
    m_data.shared().b = 1;
}

void Console::on_ui_BButton_released()
{
    m_data.shared().b = 0;
}

