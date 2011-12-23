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
#include <QMessageBox>
#include <QDebug>

#define DEFAULT_PATH "/mnt/browser"
#define INTERNAL_USER_PATH "/mnt/browser/code"
#define USB_USER_PATH "/mnt/browser/usb"
#define COMPILERS_PATH "/mnt/kiss/usercode/compilers/"
#define STAGE_PATH "/mnt/browser/stage/"

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
FileManager::FileManager(QWidget *parent) : Page(parent), m_compiler(parent), m_compile(false)
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

    // Directory user interface setup
    if(m_dir.isDir(m_index)) {
        ui_actionButton->setText("Open");
        ui_actionButton->show();
	ui_stageButton->hide();
        ui_stopButton->hide();

        if(indexName == DEFAULT_PATH ||
           indexName == INTERNAL_USER_PATH ||
           indexName.startsWith(USB_USER_PATH) ||
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
	ui_stageButton->show();
        ui_deleteButton->show();

	const QStringList& validExts = QDir(COMPILERS_PATH).entryList(QDir::Files | QDir::NoDotAndDotDot);
	// qWarning() << validExts;

	m_compile = false;
	bool shouldCompile = false;
	foreach(const QString& ext, validExts) {
		if(filename.endsWith(QString(".") + ext)) {
			shouldCompile = true;
			break;
		}
	}
        if(shouldCompile) {
		ui_actionButton->setText("Compile");
		ui_actionButton->show();
		ui_stopButton->hide();
		m_compile = true;
        } else if(filename.endsWith(".wav",Qt::CaseInsensitive) || filename.endsWith(".mp3",Qt::CaseInsensitive)){
            ui_actionButton->setText("Play");
            ui_actionButton->show();
            ui_stopButton->show();
        }
        else if(filename == "CBC_interface")
        {
            ui_actionButton->setText("Install");
            ui_actionButton->show();
            ui_stopButton->hide();
        }
        else{
            ui_actionButton->hide();
            ui_stopButton->hide();
        }
    }
}

// moves the file browser to the default CBC home 
void FileManager::on_ui_homeButton_clicked()
{
    m_index = m_dir.index(DEFAULT_PATH);
    ui_directoryBrowser->setRootIndex(m_index);
    ui_directoryBrowser->setCurrentIndex(m_index);
    m_dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);

    updateGUI();
}

bool FileManager::isUSBMounted()
{
    QFileInfo info(USB_USER_PATH);

    return info.exists();
}

// mounts and unmounts the external usb stick
void FileManager::on_ui_mountButton_clicked()
{
    QProcess mount;

    if(!this->isUSBMounted())
    {
        mount.start(QString("/mnt/kiss/usercode/mount-usb ").append(USB_USER_PATH));
        mount.waitForFinished();

        if(mount.exitCode()) return;    // don't change gui if mount fails

        m_index = m_dir.index(USB_USER_PATH);
        ui_mountButton->setText("Unmnt. USB");
    }
    else{
        if(m_dir.filePath(m_index).startsWith(USB_USER_PATH))
        {
            m_index = m_dir.index(DEFAULT_PATH);
            ui_directoryBrowser->setRootIndex(m_index);
            ui_directoryBrowser->setCurrentIndex(m_index);
            m_dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
        }
        mount.start(QString("/mnt/kiss/usercode/umount-usb ").append(USB_USER_PATH));
        mount.waitForFinished();

        if(mount.exitCode()) return;    // don't change gui if unmount fails
        ui_mountButton->setText("Mount USB");

        QProcess::startDetached("btplay /mnt/kiss/sounds/disconnected.wav");
    }

    this->updateGUI();
}

// determines what to do with the currently highlighted index when the action button is pressed
void FileManager::on_ui_actionButton_clicked()
{
    if(m_dir.isDir(m_index)){	// selected index is a directory
        QString path = m_dir.fileInfo(m_index).canonicalFilePath();

        if(path == DEFAULT_PATH)
            m_dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
        else
            m_dir.setFilter(QDir::AllEntries);

        ui_directoryBrowser->setRootIndex(m_dir.index(path));
        m_index = m_dir.index(path);
    }
    else{	// selected index is a file
        QFileInfo fileInfo = m_dir.fileInfo(m_index);
        //QString filePath = m_dir.filePath(m_index);
		
	const QStringList& validExts = QDir(COMPILERS_PATH).entryList(QDir::Files | QDir::NoDotAndDotDot);		
		// file is a c program
        if(validExts.contains(fileInfo.suffix())){

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
		// file is a sound .wav or .mp3
        else if(fileInfo.fileName().endsWith(".wav",Qt::CaseInsensitive) || fileInfo.fileName().endsWith(".mp3",Qt::CaseInsensitive)){
            QString soundFile = fileInfo.absoluteFilePath();
            soundFile.replace(" ","\\ ");
            soundFile.prepend("btplay ");
            qWarning("%s",qPrintable(soundFile));
            QProcess::startDetached(soundFile.toLocal8Bit());
        }
// file is a CBC_interface update file
        else if(fileInfo.fileName() == "CBC_interface"){

            int ret = QMessageBox::warning(this,
                                           "Update CBC Interface",
                                           "This will take a few minutes!\nDo not unplug USB key.",
                                           QMessageBox::Cancel | QMessageBox::Ok,
                                           QMessageBox::Cancel);

            if(ret == QMessageBox::Ok){
                QFile upgradeFile("/psp/kiss_upgrade");
                if(upgradeFile.open(QIODevice::WriteOnly | QIODevice::Text))
                {
                    QProcess updateFeatures;
                    // run the CBC Interface file to get all new features it includes
                    updateFeatures.start(fileInfo.absoluteFilePath().append(" --features"));
                    updateFeatures.waitForFinished();
                    // save the features names
                    QString featureString(updateFeatures.readAllStandardOutput());

                    // prepend the location of the interface file to the update arguments
                    featureString.prepend(fileInfo.absoluteFilePath().remove("/browser"));
                    upgradeFile.write(featureString.toLocal8Bit());
                    upgradeFile.close();
                    ::system("reboot"); // reboot the cbc to install
                }
            }
        }
    }
    this->updateGUI();
}

void FileManager::on_ui_stageButton_clicked()
{
	QFileInfo info(m_dir.filePath(ui_directoryBrowser->currentIndex()));
	if(!QDir(STAGE_PATH).exists()) QDir(DEFAULT_PATH).mkdir("stage");
	if(QFile::copy(info.filePath(), QString(STAGE_PATH) + info.fileName()))
		QMessageBox::information(this, "Stage", "File is ready for transfer.");
	else QMessageBox::warning(this, "Stage", "File copy failed.");
}

// stop playing sound file
void FileManager::on_ui_stopButton_clicked()
{
    ::system("echo stop > /tmp/.btplay-cmdin");
}

// copy button opens the copy dialog defined at the top of this file
// allows the user to copy the currently selected index to the usb stick
void FileManager::on_ui_copyButton_clicked()
{
    QString fromPath = m_dir.filePath(ui_directoryBrowser->currentIndex());

    CopyDialog cpDialog(fromPath,this);
    cpDialog.exec();
}

// deletes the currently selected index item, assuming it is not being used ie. program file
void FileManager::on_ui_deleteButton_clicked()
{
    int ret = QMessageBox::warning(this,
                                   "Delete",
                                   "Do you want to Delete the file?",
                                   QMessageBox::Cancel | QMessageBox::Ok,
                                   QMessageBox::Cancel);
    if(ret == QMessageBox::Ok){
        QString fPath = m_dir.filePath(ui_directoryBrowser->currentIndex());
        m_index = ui_directoryBrowser->indexAt(QPoint(1,1));
        fPath.replace(" ",QString("\\ "));
        QString deleteString("rm -rf " + fPath);

        ::system(deleteString.toLocal8Bit());
    }
}
