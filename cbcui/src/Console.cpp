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
#include <QTimer>

Console::Console(QWidget *parent) : Page(parent), m_uiData("/tmp/cbc_uidata"), m_bell("/mnt/kiss/sounds/beep.wav", this)
{
    setupUi(this);
    
    QObject::connect(UserProgram::instance(), SIGNAL(consoleOutput(QString)), this, SLOT(updateText(QString)));
    QObject::connect(UserProgram::instance(), SIGNAL(consoleRaise()), this, SLOT(raisePage()));
    QObject::connect(UserProgram::instance(), SIGNAL(started()), ui_console, SLOT(clear()));
    
    m_uiData.shared().a_button = 0;
    m_uiData.shared().b_button = 0;
    m_uiData.shared().up_button = 0;
    m_uiData.shared().down_button = 0;
    m_uiData.shared().left_button = 0;
    m_uiData.shared().right_button = 0;
    m_bell.play();
}

Console::~Console()
{
}

void Console::setViewportColors(Qt::GlobalColor text, Qt::GlobalColor background)
{
    //set the background and text color of the console
    QPalette p(ui_console->viewport()->palette());
    p.setColor(QPalette::Active,QPalette::Base, background);
    p.setColor(QPalette::Active,QPalette::Text, text);
    ui_console->viewport()->setPalette(p);
}

void Console::invertColors()
{
    if(ui_console->viewport()->palette().color(QPalette::Base) == Qt::white)
        this->setViewportColors(Qt::white,Qt::black);
    else
        this->setViewportColors(Qt::black,Qt::white);
}

void Console::updateText(QString text)
{
    if(text.contains("\a",Qt::CaseInsensitive)){
         bell();
        text.remove("\a");
    }

    ui_console->insertPlainText(text);
    ui_console->verticalScrollBar()->triggerAction(QScrollBar::SliderToMaximum);
}

void Console::bell()
{
    this->invertColors();
    QTimer::singleShot(200,this,SLOT(invertColors()));
    m_bell.play();
}

void Console::on_ui_upButton_pressed()
{
  m_uiData.shared().up_button = 1;
}
   
void Console::on_ui_downButton_pressed()
{
  m_uiData.shared().down_button = 1; 
}

void Console::on_ui_leftButton_pressed()
{
  m_uiData.shared().left_button = 1; 
}

void Console::on_ui_rightButton_pressed()
{
  m_uiData.shared().right_button = 1; 
}

void Console::on_ui_aButton_pressed()
{
  m_uiData.shared().a_button = 1; 
}

void Console::on_ui_bButton_pressed()
{
  m_uiData.shared().b_button = 1; 
}

void Console::on_ui_upButton_released()
{
  m_uiData.shared().up_button = 0; 
}
   
void Console::on_ui_downButton_released()
{
  m_uiData.shared().down_button = 0; 
}

void Console::on_ui_leftButton_released()
{
  m_uiData.shared().left_button = 0; 
}

void Console::on_ui_rightButton_released()
{
  m_uiData.shared().right_button = 0; 
}

void Console::on_ui_aButton_released()
{
  m_uiData.shared().a_button = 0; 
}

void Console::on_ui_bButton_released()
{
  m_uiData.shared().b_button = 0; 
}

