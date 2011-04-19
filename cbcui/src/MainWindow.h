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

#ifndef __MAIN_WINDOW_H__
#define __MAIN_WINDOW_H__

#include "ui_MainWindow.h"
#include <QDialog>
#include <QVBoxLayout>
#include <QTimer>

#include "MainMenu.h"

class MainWindow : public QDialog, private Ui::MainWindow
{
    Q_OBJECT

protected:
    MainWindow(QWidget *parent = 0);
    static MainWindow *m_mainWin;
    ~MainWindow();

public:
    static MainWindow *initialize(QWidget *parent = 0){
        if(!m_mainWin)
            m_mainWin = new MainWindow(parent);
        return m_mainWin;
    }

    static MainWindow *instance(){
        return m_mainWin;
    }
    static MainWindow *destroy() {
        delete m_mainWin;
        m_mainWin = 0;
        return 0;
    }

public slots:
   void on_ui_backButton_clicked(bool checked = false);
   void on_ui_closeButton_clicked(bool checked = false);
   void on_ui_estopButton_clicked(bool checked = false);
   void updateBatteryDisplay();
   void userProgramStateChange(int state);
   void batteryWarning(float volts);
   void checkWifiSignal();
   void stopWifiCheck();

private:
   MainMenu *m_mainMenu;
   QTimer   *m_wifiSig;
   void hideEStop();
};

#endif
