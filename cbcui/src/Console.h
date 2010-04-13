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

#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include "ui_Console.h"
#include "Page.h"
#include <QString>
#include <QProcess>
#include <QFile>

#include "SharedMem.h"
#include "UIData.h"

#define CONSOLE_MAX_LENGTH 5000

class Console : public Page, private Ui::Console
{
    Q_OBJECT

public:
    Console(QWidget *parent = 0);
    ~Console();

public slots:
   void invertColors();
   void updateText(QString text);
   
   void on_ui_upButton_pressed();
   void on_ui_downButton_pressed();
   void on_ui_leftButton_pressed();
   void on_ui_rightButton_pressed();
   void on_ui_aButton_pressed();
   void on_ui_bButton_pressed();

  void on_ui_upButton_released();
   void on_ui_downButton_released();
   void on_ui_leftButton_released();
   void on_ui_rightButton_released();
   void on_ui_aButton_released();
   void on_ui_bButton_released();
   
   void bell();
   void playSoundFile(QString filename);
   void stopSoundFile();
   void playChange();
   void recordChange(QProcess::ProcessState newState);
   void manageSound();

protected:
    void setViewportColors(Qt::GlobalColor text, Qt::GlobalColor background);
private:

   SharedMem<UIData>    m_uiData;
   
    QProcess            m_recdProc;
    QFile               *m_btinput;
    QFile               *m_btoutput;
    uchar               *m_playState;
};

#endif
