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

#ifndef __SIMPLEUSERCODE_H__
#define __SIMPLEUSERCODE_H__

#include "ui_SimpleUserCode.h"
#include <QDialog>

#include "Cbob.h"
#include "AdjustColorWidget.h"
#include "ColorTracker.h"
#include "Vision.h"

#include <QProcess>

class SimpleUserCode : public QWidget, private Ui::SimpleUserCode
{
    Q_OBJECT

public:
    SimpleUserCode(QWidget *parent = 0);
    ~SimpleUserCode();

public slots:
    void on_CompileButton_clicked();
    void on_RunButton_clicked();
    void on_StopButton_clicked();

    void readStandardError();
    void readStandardOutput();
    
    void compileFile(QString filename);

signals:
    void run();
    void stop();

private:
    QProcess m_compiler;
};

#endif

