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

#ifndef __FILE_MANAGER_H__
#define __FILE_MANAGER_H__

#include <QFileSystemModel>
#include <QDialog>
#include <QTreeView>

#include "ui_FileManager.h"
#include "Page.h"
#include "Compiler.h"

class CopyDialog : public QDialog
{
    Q_OBJECT

    public:
        CopyDialog(QString fromPath, QWidget *parent = 0);

        QTreeView           *cpTreeView;
        QPushButton         *copyButton;
        QPushButton         *cancelButton;

    private:
        QString             cpFromPath;
        QFileSystemModel    cpDirModel;
        QModelIndex         cpIndex;

    public slots:
        void copyData();
};

class FileManager : public Page, private Ui::FileManager
{
	Q_OBJECT

public:
	FileManager(QWidget *parent = 0);
	~FileManager();

	bool isUSBMounted();

public slots:
	void on_ui_directoryBrowser_clicked(const QModelIndex &index);
	void on_ui_directoryBrowser_entered(const QModelIndex &index);
	void on_ui_homeButton_clicked();
	void on_ui_mountButton_clicked();
	void on_ui_actionButton_clicked();
	void on_ui_stageButton_clicked();
	void on_ui_stopButton_clicked();
	void on_ui_copyButton_clicked();
	void on_ui_deleteButton_clicked();
	void updateGUI();

private:
	Compiler             m_compiler;
	QFileSystemModel    m_dir;
	QModelIndex         m_index;
	bool m_compile;
};

#endif
