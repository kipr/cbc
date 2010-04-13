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
#include "MainMenu.h"
#include "Page.h"
#include "UserProgram.h"
#include "CbobData.h"
#include <QMessageBox>
#include <QProcess>

MainWindow::MainWindow(QWidget *parent) : QDialog(parent), m_mainMenu(0)
{
    setupUi(this);

    setWindowState(windowState() | Qt::WindowFullScreen);

    m_mainMenu = new MainMenu(ui_widget);

    m_mainMenu->raisePage();

    QObject::connect(ui_runstopButton, SIGNAL(clicked()), UserProgram::instance(), SLOT(toggleState()));
    QObject::connect(CbobData::instance(), SIGNAL(refresh()), this, SLOT(updateBatteryDisplay()));
    QObject::connect(CbobData::instance(), SIGNAL(lowBattery(float)), this, SLOT(batteryWarning(float)));
    QObject::connect(UserProgram::instance(), SIGNAL(loadedState(bool)), ui_runstopButton, SLOT(setEnabled(bool)));
    QObject::connect(UserProgram::instance(), SIGNAL(stateChange(int)), this, SLOT(userProgramStateChange(int)));
    
    updateBatteryDisplay();

    CbobData::instance()->resetPullups();

    userProgramStateChange(0);
}

MainWindow::~MainWindow()
{
    delete m_mainMenu;
}


void MainWindow::updateBatteryDisplay()
{
    int percentage;
    // The close button has an image with the close X set as the background
    // the text that is placed on top of the button is the battery percentage however,
    // it has been shifted to the left to make it look like it is not part of the button
    // the voltage percent calculation is based on the cutoff voltage of 6.0v
    // then scaled up to the maximum voltage 8.4v (8.4v - 6.0v = 2.4)
    percentage = (int)((CbobData::instance()->batteryVoltage()-6.0)*(100.0/2.4));
    if(percentage > 100) percentage = 100;

    ui_closeButton->setText(QString::number(percentage) + "%");
}

void MainWindow::batteryWarning(float volts)
{
    if(volts <= 6.2){
        QProcess::startDetached("aplay /mnt/kiss/sounds/tri_klaxon.wav");
        QMessageBox::critical(this,
                             "Low Battery!",
                             "Shutdown imminent!",
                             QMessageBox::Ok,
                             QMessageBox::NoButton);
    }
    else{
        QProcess::startDetached("aplay /mnt/kiss/sounds/tri_klaxon.wav");
        QMessageBox::warning(this,
                             "Low Battery!",
                             "Low Battery - Please Charge Me!",
                             QMessageBox::Ok,
                             QMessageBox::NoButton);
    }
}

void MainWindow::on_ui_backButton_clicked(bool)
{
   Page::currentPage()->raiseLastPage();
}

void MainWindow::on_ui_closeButton_clicked(bool)
{
    Page::currentPage()->lower();
    m_mainMenu->raisePage();
}

void MainWindow::on_ui_estopButton_clicked(bool)
{
    // turn off motors
    // disable servos
    CbobData::instance()->allStop();
    // kill user program
    UserProgram::instance()->stop();
    // hide estop button
    //ui_estopButton->hide();
}

void MainWindow::userProgramStateChange(int state)
{
    QString name(UserProgram::instance()->getProgramName());
    int lastChar = name.count() - 1;
    name.remove(lastChar,1);

    int n = name.count() - 15;
    if(n > 0) {
        name.remove(12,n+3);
        name.append("...");
    }

    if(state) {
        name.prepend("Running\n");
        ui_runstopButton->setChecked(true);
        ui_runstopButton->setText(name);
        //ui_estopButton->show();
    }
    else {
        name.prepend("Run\n");
        ui_runstopButton->setText(name);
        ui_runstopButton->setChecked(false);
        //this->hideEStop();
    }
}

void MainWindow::hideEStop()
{
    if(UserProgram::instance()->isRunning()) return;
    //|| motorTest is running || motorTune is running || servo is running)
    else ui_estopButton->hide();
}
