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

#ifndef __MAIN_DIALOG_H__
#define __MAIN_DIALOG_H__

#include "ui_MainWindow.h"
#include <QDialog>

#include "Cbob.h"
#include "AdjustColorWidget.h"
#include "ColorTracker.h"
#include "Vision.h"
#include "ProgramSelect.h"
#include "CbobData.h"
#include "Sensors.h"
#include "Console.h"
#include "SerialServer.h"
#include "Settings.h"

class MainWindow : public QDialog, private Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    enum {NUM_COLOR_CHANNELS=4};


public slots:
    void on_ui_ProgramsButton_clicked(bool checked = false);
    void on_ui_SensorsButton_clicked(bool checked = false);
    void on_ui_ConsoleButton_clicked(bool checked = false);
    void on_ui_VisionButton_clicked(bool checked = false);
    void on_ui_AboutButton_clicked(bool checked = false);
    void on_ui_SettingsButton_clicked(bool checked = false);
    void on_ui_PowerButton_clicked(bool checked = false);

    void runProgram();
    void stopProgram();
    void compileProgram(QString name);
    
    void toggleSerialServer(bool enabled);

private:
    Cbob m_cbob;
    CbobData m_cbobData;
    Vision m_vision;
    AdjustColorWidget m_adjustColorWidget;
    ProgramSelect    m_programs;
    Sensors m_sensors;
    Console m_console;
    SerialServer m_serialServer;
    Settings m_settings;
};

#endif

