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
#include <QDir>
#include <QTextStream>

Console::Console(QWidget *parent) : Page(parent), m_uiData("/tmp/cbc_uidata")
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

    m_uiData.shared().state = 0;
    m_uiData.shared().playing = 0;
    m_uiData.shared().recording = 0;

    m_btinput = new QFile("/tmp/.btplay-cmdin");
    if(!m_btinput->open(QIODevice::WriteOnly | QIODevice::Text)) qWarning("btplay-cmdin not open");

    QObject::connect(&m_recdProc, SIGNAL(stateChanged(QProcess::ProcessState)), this, SLOT(recordChange(QProcess::ProcessState)));
    //QProcess::startDetached("aplay /mnt/kiss/sounds/mellow.wav");
}

Console::~Console()
{
    m_btinput->close();
    delete m_btinput;
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
        this->manageSound();
        text.remove("\a");
    }

    ui_console->insertPlainText(text);
    ui_console->verticalScrollBar()->triggerAction(QScrollBar::SliderToMaximum);
}

void Console::bell()
{
    this->invertColors();
    QTimer::singleShot(200,this,SLOT(invertColors()));
    QProcess::startDetached("aplay /mnt/kiss/sounds/beep.wav");
}

void Console::playSoundFile(QString filename)
{
    // write to BT command file to play current sound even if currently playing
    filename.prepend("btplay ");
    QProcess::startDetached(filename);
    m_uiData.shared().playing = 1;
}

void Console::stopSoundFile()
{
    // write to BT command file to stop playing current sound
    QTextStream btin(m_btinput);
    btin << "stop\n";
    m_uiData.shared().playing = 0;
}

void Console::recordChange(QProcess::ProcessState newState)
{
    if(newState == QProcess::NotRunning)
        m_uiData.shared().recording = 0;
    else
        m_uiData.shared().recording = 1;
}

void Console::manageSound()
{
    switch(m_uiData.shared().state)
    {
    case 1: //the beep signal is emitted
        {
            this->bell();
            break;
        }
    case 2: // play sound from file on USB stick
        if(m_uiData.shared().recording) break;
        else {
            QDir usbDir("/mnt/browser/usb/sound");  // make sure the sound directory is there
            if(!usbDir.exists()) break;
            // play a song here
            this->playSoundFile(QString("/mnt/browser/usb/sound/") + m_uiData.shared().filename);
            break;
        }
    case 3: // stop playing the song
        {
            this->stopSoundFile();
            break;
        }
    case 4: // record from the mic if not currently playing
        if(m_uiData.shared().playing && m_recdProc.state() != QProcess::NotRunning) break;
        else {
            QDir usbDir("/mnt/browser/usb");
            if(!usbDir.exists()) break;     // make sure the usb drive is plugged in
            usbDir.mkpath("/mnt/browser/usb/sound/"); // create the sound directory if not there
            // start recording from the mic
            QString wavFile = QString("arecord -d %1 -f cd /mnt/browser/usb/sound/").arg(m_uiData.shared().recordTime);
            m_recdProc.start(wavFile + m_uiData.shared().filename);
            ui_console->insertPlainText(QString("recording to -") + m_uiData.shared().filename + "\n");
            break;
        }
    case 5: // stop recording
        if(m_recdProc.state() != QProcess::NotRunning) m_recdProc.kill(); // stop recording
        break;

    default: break;
    }
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

