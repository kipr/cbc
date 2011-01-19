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

////////////////////////////////////////
//  copy dialog setup
///////////////
CopyDialog::CopyDialog(QString fromPath, QWidget *parent) : QDialog(parent), cpFromPath(fromPath)
{
    setWindowTitle("Copy to Location");
    setWindowFlags(Qt::Tool);
    this->setGeometry(0,0,320,250);

    cpDirModel.setRootPath(INTERNAL_USER_PATH);
    cpDirModel.setResolveSymlinks(true);
    cpDirModel.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    cpIndex = cpDirModel.index(INTERNAL_USER_PATH);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    cpTreeView = new QTreeView(this);
    cpTreeView->setModel(&cpDirModel);
    cpTreeView->setRootIndex(cpIndex);
    cpTreeView->setRootIsDecorated(true);
    cpTreeView->setCurrentIndex(cpIndex);
    cpTreeView->setFrameRect(QRect(0,0,238,250));
    mainLayout->addWidget(cpTreeView);

    QVBoxLayout *buttonLayout = new QVBoxLayout;

    buttonLayout->addStretch(0);

    copyButton = new QPushButton("Copy");
    copyButton->setMinimumWidth(80);
    copyButton->setMinimumHeight(30);
    copyButton->setStyleSheet("QPushButton{border:0px;background-image:url(:/actions/rivet80x30L.png);color:black;} QPushButton:pressed{border:0px;background-image:url(:/actions/rivet80x30D.png);color:white;}");
    buttonLayout->addWidget(copyButton);

    buttonLayout->addStretch(0);

    cancelButton = new QPushButton("Cancel");
    cancelButton->setMinimumWidth(80);
    cancelButton->setMinimumHeight(30);
    cancelButton->setStyleSheet("QPushButton{border:0px;background-image:url(:/actions/rivet80x30L.png);color:black;} QPushButton:pressed{border:0px;background-image:url(:/actions/rivet80x30D.png);color:white;}");
    buttonLayout->addWidget(cancelButton);

    buttonLayout->addStretch(0);

    mainLayout->addLayout(buttonLayout);

    connect(copyButton,SIGNAL(clicked()),this,SLOT(copyData()));
    connect(cancelButton,SIGNAL(clicked()),this,SLOT(reject()));

    this->setLayout(mainLayout);
}

void CopyDialog::copyData()
{
    cpIndex = cpTreeView->currentIndex();
    if(!cpDirModel.isDir(cpIndex)) return;

    QString copyString("cp -r ");
    QString cpToPath = cpDirModel.filePath(cpIndex);

    cpToPath.replace(" ",QString("\\ "));
    cpFromPath.replace(" ",QString("\\ "));

    copyString.append(cpFromPath);
    copyString.append(" ");
    copyString.append(cpToPath);

    ::system(copyString.toLocal8Bit());
    this->accept();
}

////////////////////////////////////////////////////////////////
//  File Manager
////////////////
FileManager::FileManager(QWidget *parent) : Page(parent), m_compiler(parent), m_mntState(false)
{
    setupUi(this);
    
    m_dir.setRootPath(DEFAULT_PATH);
    m_dir.setResolveSymlinks(true);
    ui_directoryBrowser->setModel(&m_dir);
    m_dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);

    ui_directoryBrowser->setRootIndex(m_dir.index(DEFAULT_PATH));
    m_index = m_dir.index(DEFAULT_PATH);

    //QObject::connect(&m_dir, SIGNAL(layoutChanged()), this, SLOT(updateGUI()));

    ui_actionButton->hide();
    ui_stopButton->hide();
    ui_copyButton->hide();
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
    this->updateGUI();
}

void FileManager::on_ui_directoryBrowser_entered(const QModelIndex &index)
{
    m_index = index;
    this->updateGUI();
}

void FileManager::updateGUI()
{
    QString indexName = m_dir.filePath(m_index);
    ui_directoryBrowser->setCurrentIndex(m_index);

    if(m_mntState)
        ui_mountButton->setText("Unmnt. USB");
    else
        ui_mountButton->setText("Mount USB");

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
            // ui_copyButton->hide();   // uncomment to allow copy
            ui_deleteButton->hide();
        }
        else{
            // ui_copyButton->show();   // uncomment to allow copy
            ui_deleteButton->show();
       }
    }
    // File user interface setup
    else {
        QString filename = m_dir.fileName(m_index);
        // ui_copyButton->show();   // uncomment to allow copy
        ui_deleteButton->show();

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

void FileManager::on_ui_homeButton_clicked()
{
    m_index = m_dir.index(DEFAULT_PATH);
    ui_directoryBrowser->setRootIndex(m_index);
    ui_directoryBrowser->setCurrentIndex(m_index);
    m_dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);

    updateGUI();
}

void FileManager::on_ui_mountButton_clicked()
{
    QProcess mount;

    if(!m_mntState){
        mount.start("/mnt/kiss/usercode/mount-usb");
        mount.waitForFinished();

        if(mount.exitCode()) return;    // don't change gui if mount fails

        m_index = m_dir.index(USB_USER_PATH);
        m_mntState = true;
    }
    else{
        mount.start("/mnt/kiss/usercode/umount-usb");
        mount.waitForFinished();

        if(mount.exitCode()) return;    // don't change gui if unmount fails

        if(m_dir.filePath(m_index).startsWith(USB_USER_PATH)){
            m_index = m_dir.index(DEFAULT_PATH);
            ui_directoryBrowser->setRootIndex(m_index);
            ui_directoryBrowser->setCurrentIndex(m_index);
            m_dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
        }

        QProcess::startDetached("btplay /mnt/kiss/sounds/disconnected.wav");
        m_mntState = false;
    }

    this->updateGUI();
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
        m_index = m_dir.index(path);
    }
    else{
        QFileInfo fileInfo = m_dir.fileInfo(m_index);
        //QString filePath = m_dir.filePath(m_index);
        if(fileInfo.suffix() == "c"){

            // if the file to compile is on the external usb, copy it to the internal storage
            if(fileInfo.absolutePath().startsWith(USB_USER_PATH)){

                QString toPath(INTERNAL_USER_PATH);
                toPath.append("/");

                // create a directory with the name of the file
                m_dir.mkdir(m_dir.index(INTERNAL_USER_PATH),fileInfo.baseName());

                toPath.append(fileInfo.baseName());
                toPath.append("/");
                toPath.replace(" ",QString("\\ "));

                QString copyString("cp -r ");
                copyString.append(fileInfo.absoluteFilePath().replace(" ",QString("\\ ")));
                copyString.append(" ");
                copyString.append(toPath);

                ::system(copyString.toLocal8Bit());
            }

            QProcess::startDetached("aplay /mnt/kiss/sounds/compiling.wav");
            m_compiler.compileFile(fileInfo.absoluteFilePath());
        }
        else if(fileInfo.fileName().endsWith(".wav",Qt::CaseInsensitive) || fileInfo.fileName().endsWith(".mp3",Qt::CaseInsensitive)){
            QString soundFile = fileInfo.absoluteFilePath();
            soundFile.replace(" ","\\ ");
            soundFile.prepend("btplay ");
            qWarning("%s",qPrintable(soundFile));
            QProcess::startDetached(soundFile.toLocal8Bit());
        }
    }
    this->updateGUI();
}

void FileManager::on_ui_stopButton_clicked()
{
    ::system("echo stop > /tmp/.btplay-cmdin");
}

void FileManager::on_ui_copyButton_clicked()
{
    QString fromPath = m_dir.filePath(ui_directoryBrowser->currentIndex());

    CopyDialog cpDialog(fromPath,this);
    cpDialog.exec();
}

void FileManager::on_ui_deleteButton_clicked()
{
    QString fPath = m_dir.filePath(ui_directoryBrowser->currentIndex());
    fPath.replace(" ",QString("\\ "));
    QString deleteString("rm -rf " + fPath);

    ::system(deleteString.toLocal8Bit());
}
