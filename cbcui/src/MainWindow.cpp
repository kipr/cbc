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

#include "MainWindow.h"

#include <QMessageBox>
#include <QProcess>

MainWindow::MainWindow(QWidget *parent) :
        QDialog(parent),
        m_cbob(this),
        m_adjustColorWidget(NULL, &m_vision.m_colorTracker),
        m_programs(this),
        m_sensors(&m_cbobData, this), 
        m_console(this),
        m_serialServer(this),
        m_settings(this)
{
    setupUi(this);

#ifdef QT_ARCH_ARM
    setWindowState(windowState() | Qt::WindowFullScreen);
#endif

    m_adjustColorWidget.close();
    m_console.close();
    m_programs.close();
    m_settings.close();
    m_sensors.close();
    
    //m_cbob.start();
    //usleep(5000);
    //m_serialServer.start();

    ui_VersionLabel->setText("CBC Firmware Version " + QApplication::applicationVersion());

    QObject::connect(&m_programs, SIGNAL(run()), this, SLOT(runProgram()));
    QObject::connect(&m_programs, SIGNAL(stop()), this, SLOT(stopProgram()));
    QObject::connect(&m_serialServer, SIGNAL(downloadFinished(QString)), this, SLOT(compileProgram(QString)));
    QObject::connect(&m_settings, SIGNAL(downloadsToggled(bool)), this, SLOT(toggleSerialServer(bool)));
}

MainWindow::~MainWindow()
{
    m_cbob.stop();
}

void MainWindow::on_ui_ProgramsButton_clicked(bool)
{
    m_programs.show();
    m_programs.raise();
#ifdef QT_ARCH_ARM
    m_programs.setWindowState(windowState() | Qt::WindowFullScreen);
#endif
}

void MainWindow::on_ui_SensorsButton_clicked(bool)
{
    m_sensors.show();
    m_sensors.raise();
#ifdef QT_ARCH_ARM
    m_sensors.setWindowState(windowState() | Qt::WindowFullScreen);
#endif
}

void MainWindow::on_ui_ConsoleButton_clicked(bool)
{
    m_console.show();
    m_console.raise();
#ifdef QT_ARCH_ARM
    m_sensors.setWindowState(windowState() | Qt::WindowFullScreen);
#endif
}

void MainWindow::on_ui_VisionButton_clicked(bool)
{
    m_adjustColorWidget.show();
#ifdef QT_ARCH_ARM
    m_adjustColorWidget.setWindowState(windowState() | Qt::WindowFullScreen);
#endif
}

void MainWindow::on_ui_AboutButton_clicked(bool)
{
    QString aboutString = "CBC Firmware Version " + QApplication::applicationVersion() + "\n\n";
    aboutString += "Copyright (C) 2009 KISS Institute for Practical Robotics\n";
    aboutString += "http://www.kipr.org/\nhttp://www.botball.org/\n";
    QMessageBox::about(this, "CBC Firmware", aboutString);
}

void MainWindow::on_ui_SettingsButton_clicked(bool)
{
    m_settings.show();
#ifdef QT_ARCH_ARM
    m_settings.setWindowState(windowState() | Qt::WindowFullScreen);
#endif
}

void MainWindow::on_ui_PowerButton_clicked(bool)
{
    QMessageBox shutdownDialog(0);
    QTimer timer;
    QProcess shutdown;

    QMessageBox::StandardButton ret = QMessageBox::question(this, "Power Off", "Are you sure you'd like to power off?", QMessageBox::Yes | QMessageBox::No);

    if (ret == QMessageBox::Yes) {
        shutdown.start("/mnt/kiss/shutdown.sh");

        timer.setSingleShot(true);

        shutdownDialog.setStandardButtons(QMessageBox::NoButton);
        shutdownDialog.setText("Shuting down in 3...");
        shutdownDialog.show();
        timer.start(1000);


        while (timer.isActive()) QApplication::processEvents();
        shutdownDialog.setText("Shutting down in 2...");
        timer.start(1000);

        while (timer.isActive()) QApplication::processEvents();
        shutdownDialog.setText("Shutting down in 1...");
        timer.start(1000);

        while (timer.isActive()) QApplication::processEvents();

        shutdownDialog.setText("Now :)");

        while (1) QApplication::processEvents();
    }
}

void MainWindow::runProgram()
{
    m_console.on_ui_RunButton_clicked();
    on_ui_ConsoleButton_clicked();
}

void MainWindow::stopProgram()
{
    m_console.on_ui_StopButton_clicked();
}

void MainWindow::compileProgram(QString filename)
{
    on_ui_ProgramsButton_clicked();
    m_programs.compileFile(filename);
}

void MainWindow::toggleSerialServer(bool enabled)
{
    if(m_serialServer.isRunning() && !enabled)
        m_serialServer.stop();
    else if(!m_serialServer.isRunning() && enabled)
        m_serialServer.start();
}


