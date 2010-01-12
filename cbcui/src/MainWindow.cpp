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

MainWindow::MainWindow(QWidget *parent) : QDialog(parent), m_mainMenu(0)
{
    setupUi(this);

    setWindowState(windowState() | Qt::WindowFullScreen);
    
    m_mainMenu = new MainMenu(ui_widget);
    
    m_mainMenu->raisePage();
    
    QObject::connect(ui_runstopButton, SIGNAL(clicked()), UserProgram::instance(), SLOT(toggleState()));
    QObject::connect(CbobData::instance(), SIGNAL(refresh()), this, SLOT(updateBatteryDisplay()));
    QObject::connect(UserProgram::instance(), SIGNAL(stateChange(int)), this, SLOT(userProgramStateChange(int)));
    
    updateBatteryDisplay();
    
    userProgramStateChange(0);
}

MainWindow::~MainWindow()
{
    delete m_mainMenu;
}


void MainWindow::updateBatteryDisplay()
{
    // This is confusing.  The purpose here is to allow the battery percentage to 
    // be seen as if it is hiding under the clear half of the close button.
   ui_closeButton->setText(QString::number((int)((CbobData::instance()->batteryVoltage()-6.0)*(100.0/2.41))) + "%");
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
        name.remove(13,n+2);
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
