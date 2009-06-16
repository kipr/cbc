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
#include <QDialog>

#include <QProcess>

#include "cbc_data.h"
#include "SharedMem.h"

#define CONSOLE_MAX_LENGTH 5000

class Console : public QDialog, private Ui::Console
{
    Q_OBJECT

public:
    Console(QWidget *parent = 0);
    ~Console();

public slots:
    void on_ui_RunButton_clicked(bool checked = false);
    void on_ui_StopButton_clicked(bool checked = false);

    void on_ui_UpButton_pressed();
    void on_ui_UpButton_released();
    void on_ui_DownButton_pressed();
    void on_ui_DownButton_released();
    void on_ui_LeftButton_pressed();
    void on_ui_LeftButton_released();
    void on_ui_RightButton_pressed();
    void on_ui_RightButton_released();
    void on_ui_AButton_pressed();
    void on_ui_AButton_released();
    void on_ui_BButton_pressed();
    void on_ui_BButton_released();

    void readStandardError();
    void readStandardOutput();

    void userProgramFinished(int exitCode, QProcess::ExitStatus exitStatus);
    
    void updateText(QString string);
    void clearText();
private:
    QProcess m_userProgram;
    QString m_consoleData;

    SharedMem<cbc_data> m_data;
};

#endif
