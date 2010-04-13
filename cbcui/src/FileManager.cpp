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

#include "FileManager.h"

#include <QProcess>

#define DEFAULT_PATH "/mnt/browser"

FileManager::FileManager(QWidget *parent) : Page(parent), m_compiler(parent)
{
    setupUi(this);
    
    m_dir.setRootPath(DEFAULT_PATH);
    m_dir.setResolveSymlinks(true);
    ui_directoryBrowser->setModel(&m_dir);
    m_dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);

    ui_directoryBrowser->setRootIndex(m_dir.index(DEFAULT_PATH));

    ui_unmountButton->hide();
    ui_openButton->hide();
    ui_compileButton->hide();
}

FileManager::~FileManager()
{
}

bool FileManager::isUSBMounted()
{
    QFileInfo info("/mnt/browser/usb");

    return info.exists();
}

void FileManager::on_ui_directoryBrowser_entered(const QModelIndex &index)
{
    if(m_dir.isDir(index)) {
        ui_compileButton->hide();
        ui_openButton->show();
    }
    else {
        ui_compileButton->show();
        ui_openButton->hide();
    }
}

void FileManager::on_ui_openButton_clicked()
{
    QString path = m_dir.fileInfo(ui_directoryBrowser->currentIndex()).canonicalFilePath();

    if(path == DEFAULT_PATH)
        m_dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    else
        m_dir.setFilter(QDir::AllEntries);

    ui_directoryBrowser->setRootIndex(m_dir.index(path));
}

void FileManager::on_ui_mountButton_clicked()
{
    QProcess mount;

    mount.start("/mnt/kiss/usercode/mount-usb");
    mount.waitForFinished();

    if(mount.exitCode()) return;

    ui_mountButton->hide();
    ui_unmountButton->show();
}

void FileManager::on_ui_unmountButton_clicked()
{
    QProcess umount;

    ui_directoryBrowser->setRootIndex(m_dir.index(DEFAULT_PATH));
    m_dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);

    umount.start("/mnt/kiss/usercode/umount-usb");
    umount.waitForFinished();

    if(umount.exitCode()) return;

    ui_mountButton->show();
    ui_unmountButton->hide();
}

void FileManager::on_ui_compileButton_clicked()
{
    if(m_dir.isDir(ui_directoryBrowser->currentIndex())) return;

    QString filePath = m_dir.filePath(ui_directoryBrowser->currentIndex());

    QProcess::startDetached("aplay /mnt/kiss/sounds/compiling.wav");
    m_compiler.compileFile(filePath);
}

void FileManager::on_ui_deleteButton_clicked()
{
    QString deleteString = "rm -rf " + m_dir.filePath(ui_directoryBrowser->currentIndex());

    ::system(deleteString.toLocal8Bit());
}



