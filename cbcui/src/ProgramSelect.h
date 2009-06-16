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

#ifndef __PROGRAM_SELECT_H__
#define __PROGRAM_SELECT_H__

#include "ui_ProgramSelect.h"
#include "Compiler.h"
#include <QDialog>


class ProgramSelect : public QDialog, private Ui::ProgramSelect
{
Q_OBJECT

public:
    ProgramSelect(QWidget *parent = 0);
    ~ProgramSelect();
public slots:
    void on_ui_loadButton_clicked(bool checked = false);
    void on_ui_compileButton_clicked(bool checked = false);
    void on_ui_deleteButton_clicked(bool checked = false);
    void on_ui_programsList_itemClicked(QListWidgetItem *item);
    
    void show();
    
    void compileFile(QString filename);
    
signals:
    void run();
    void stop();
    
private:
    Compiler m_compiler;
    
    void refreshProgramsList();
};

#endif
