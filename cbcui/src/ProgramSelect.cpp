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

#include "ProgramSelect.h"
#include "Compiler.h"

#include <QDir>
#include <QMessageBox>

ProgramSelect::ProgramSelect(QWidget *parent) : QDialog(parent), m_compiler(parent)
{
    setupUi(this);

#ifdef QT_ARCH_ARM
    setWindowState(windowState() | Qt::WindowFullScreen);
#endif

    QObject::connect(&m_compiler, SIGNAL(run()), this, SLOT(close()));
    QObject::connect(&m_compiler, SIGNAL(run()), this, SIGNAL(run()));
    
    QObject::connect(&m_compiler, SIGNAL(stop()), this, SIGNAL(stop()));
    
    m_compiler.close();
}

ProgramSelect::~ProgramSelect()
{
}

void ProgramSelect::show()
{
    refreshProgramsList();
    QDialog::show();
}

void ProgramSelect::refreshProgramsList()
{
    QDir code("/mnt/user/code");
    code.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    
    QFileInfoList programs = code.entryInfoList();
    
    ui_programsList->clear();
    ui_filesList->clear();
    
    for(int i = 0;i < programs.count();i++)
        ui_programsList->addItem(programs[i].fileName());
}

void ProgramSelect::on_ui_loadButton_clicked(bool)
{
    m_compiler.show();
    m_compiler.compileFromUSB();
    
}



void ProgramSelect::compileFile(QString filename)
{
    m_compiler.show();
    m_compiler.raise();
    m_compiler.compileFile(filename);
}

void ProgramSelect::on_ui_compileButton_clicked(bool)
{
    if(ui_programsList->selectedItems().count() > 0) {
        QString baseName = ui_programsList->selectedItems()[0]->text();
        QString currentFile = "/mnt/user/code/" + baseName + "/" + baseName + ".c";
        compileFile(currentFile);
    }
}

void ProgramSelect::on_ui_deleteButton_clicked(bool checked)
{
    if(ui_programsList->selectedItems().count() > 0) {
        QString baseName = ui_programsList->selectedItems()[0]->text();
    
        QMessageBox::StandardButton ret = QMessageBox::question(this, "Are you sure?", "Are you sure you'd like to delete " + baseName + "?", QMessageBox::Yes | QMessageBox::No);
        
        if(ret != QMessageBox::Yes)
            return;
        ui_filesList->clear();
        
        QString cmd = "rm -rf /mnt/user/code/" + baseName;
        system(cmd.toLocal8Bit());
        
        refreshProgramsList();
    }
}

void ProgramSelect::on_ui_programsList_itemClicked(QListWidgetItem *item)
{
    QDir program("/mnt/user/code/" + item->text());
    program.setFilter(QDir::Files);
    
    QFileInfoList files = program.entryInfoList();
    
    ui_filesList->clear();
    
    for(int i = 0;i < files.count();i++)
        ui_filesList->addItem(files[i].fileName());
}


