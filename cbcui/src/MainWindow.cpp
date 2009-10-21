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

MainWindow::MainWindow(QWidget *parent) : QDialog(parent), m_mainMenu(0)
{
    setupUi(this);

    setWindowState(windowState() | Qt::WindowFullScreen);
    
    m_mainMenu = new MainMenu(ui_widget);
    
    m_mainMenu->raisePage();
    
    QObject::connect(ui_runstopButton, SIGNAL(clicked()), UserProgram::instance(), SLOT(toggleState()));
    QObject::connect(&m_timer, SIGNAL(timeout()), this, SLOT(updateBatteryDisplay()));
    
    m_timer.start(500);
    
    updateBatteryDisplay();
}

MainWindow::~MainWindow()
{
    delete m_mainMenu;
}


void MainWindow::updateBatteryDisplay()
{
   CbobData::instance()->updateSensors();
   ui_battery->setText(QString::number((int)((CbobData::instance()->batteryVoltage()-6.0)*(100.0/2.4))) + "%");
}

void MainWindow::on_ui_backButton_clicked(bool)
{
   Page::currentPage()->raiseLastPage();
}

void MainWindow::on_ui_closeButton_clicked(bool)
{
   m_mainMenu->raisePage();
}

