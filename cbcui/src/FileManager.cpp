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
#define INTERNAL_USER_PATH "/mnt/browser/code"
#define USB_USER_PATH "/mnt/browser/usb"

FileManager::FileManager(QWidget *parent) : Page(parent), m_compiler(parent)
{
    setupUi(this);
    
    m_dir.setRootPath(DEFAULT_PATH);
    m_dir.setResolveSymlinks(true);
    ui_directoryBrowser->setModel(&m_dir);
    m_dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);

    ui_directoryBrowser->setRootIndex(m_dir.index(DEFAULT_PATH));

    QObject::connect(&m_dir, SIGNAL(layoutChanged()), this, SLOT(updateGUI()));

    ui_unmountButton->hide();
    ui_actionButton->hide();
    ui_stopButton->hide();
    ui_deleteButton->hide();
    updateGUI();
}

FileManager::~FileManager()
{
}

bool FileManager::isUSBMounted()
{
    QFileInfo info("/mnt/browser/usb");

    return info.exists();
}

void FileManager::on_ui_directoryBrowser_clicked(const QModelIndex &index)
{
    m_index = index;
    // forward this to catch all clicks
    //this->on_ui_directoryBrowser_entered(index);
    this->updateGUI();
}

void FileManager::on_ui_directoryBrowser_entered(const QModelIndex &index)
{
    m_index = index;

    this->updateGUI();
}

void FileManager::updateGUI()
{
    //m_index = index;
    QString indexName = m_dir.filePath(m_index);

    ui_deleteButton->show();

    // Directory user interface setup
    if(m_dir.isDir(m_index)) {
        ui_actionButton->setText("Open");
        ui_actionButton->show();
        ui_stopButton->hide();

        if(indexName == DEFAULT_PATH ||
           indexName == INTERNAL_USER_PATH ||
           indexName == USB_USER_PATH ||
           indexName.endsWith("."))
        {
            ui_deleteButton->hide();
        }
    }
    // File user interface setup
    else {
        QString filename = m_dir.fileName(m_index);
        if(filename.endsWith(".c")){
            ui_actionButton->setText("Compile");
            ui_actionButton->show();
            ui_stopButton->hide();
        }
        else if(filename.endsWith(".wav",Qt::CaseInsensitive) || filename.endsWith(".mp3",Qt::CaseInsensitive)){
            ui_actionButton->setText("Play");
            ui_actionButton->show();
            ui_stopButton->show();
        }
        else{
            ui_actionButton->hide();
            ui_stopButton->hide();
        }
    }
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

    QProcess::startDetached("btplay /mnt/kiss/sounds/disconnected.wav");
    m_index = m_dir.index(DEFAULT_PATH);
    ui_directoryBrowser->setRootIndex(m_index);
    m_dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);

    this->updateGUI();

    umount.start("/mnt/kiss/usercode/umount-usb");
    umount.waitForFinished();

    if(umount.exitCode()) return;

    ui_mountButton->show();
    ui_unmountButton->hide();
    ui_actionButton->hide();
    ui_stopButton->hide();
}

void FileManager::on_ui_actionButton_clicked()
{
    if(m_dir.isDir(m_index)){
        QString path = m_dir.fileInfo(m_index).canonicalFilePath();

        if(path == DEFAULT_PATH)
            m_dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
        else
            m_dir.setFilter(QDir::AllEntries);

        ui_directoryBrowser->setRootIndex(m_dir.index(path));
        ui_actionButton->hide();
    }
    else{
        QString filePath = m_dir.filePath(m_index);
        if(filePath.endsWith(".c")){
            QProcess::startDetached("aplay /mnt/kiss/sounds/compiling.wav");
            m_compiler.compileFile(filePath);
        }
        else if(filePath.endsWith(".wav",Qt::CaseInsensitive) || filePath.endsWith(".mp3",Qt::CaseInsensitive)){
            filePath.prepend("btplay ");
            qWarning("%s",qPrintable(filePath));
            QProcess::startDetached(filePath.toLocal8Bit());
        }
    }
}

void FileManager::on_ui_stopButton_clicked()
{
    ::system("echo stop > /tmp/.btplay-cmdin");
}

void FileManager::on_ui_deleteButton_clicked()
{
    QString fPath = m_dir.filePath(ui_directoryBrowser->currentIndex());
    fPath.replace(" ",QString("\\ "));
    QString deleteString("rm -rf " + fPath);

    ::system(deleteString.toLocal8Bit());
}



