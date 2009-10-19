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
#include "UserProgram.h"
#include <QScrollBar>

Console::Console(QWidget *parent) : Page(parent)
{
    setupUi(this);
    
    QObject::connect(UserProgram::instance(), SIGNAL(consoleOutput(QString)), this, SLOT(updateText(QString)));
}

Console::~Console()
{
}

void Console::updateText(QString text)
{
    m_consoleData += text;
    m_consoleData = m_consoleData.right(CONSOLE_MAX_LENGTH);
    ui_console->setPlainText(m_consoleData);
    ui_console->verticalScrollBar()->triggerAction(QScrollBar::SliderToMaximum);
}
