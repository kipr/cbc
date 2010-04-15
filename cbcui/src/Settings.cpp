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

#include "Settings.h"
#include "CbobData.h"
#include <QFile>
#include <QSettings>
#include <QMessageBox>
#include <QDir>

Settings::Settings(QWidget *parent) : Page(parent), m_brightness(parent)
{
    setupUi(this);

    QObject::connect(ui_recalibrateMotorsButton, SIGNAL(clicked()), this, SLOT(recalibrateMotors()));
    QObject::connect(ui_recalibrateAccelerometerButton, SIGNAL(clicked()), this, SLOT(recalibrateAccel()));
    QObject::connect(ui_resetPIDButton, SIGNAL(clicked()), this, SLOT(resetPID()));
    QObject::connect(ui_cameraDefaultButton, SIGNAL(clicked()), this, SLOT(setCameraDefault()));
    QObject::connect(ui_brightnessButton, SIGNAL(clicked()), &m_brightness, SLOT(raisePage()));

    if(!QFile::exists("/mnt/user/cbc_v2.config")){
        storePidCal();
        storeAccelCal();
        this->resetPID();
        ::system("sync");
        ::system("sync");
    }
    else {
        loadPidCal();
        loadAccelCal();
    }

    QSettings settings("/mnt/user/cbc_v2.config",QSettings::NativeFormat);
    ui_consoleShowBox->setChecked(settings.value("consoleShowOnRun", true).toBool());
}

Settings::~Settings()
{
}

void Settings::recalibrateMotors()
{
    QMessageBox msgBox;
    msgBox.setText("Do not move motors during calibration");
    msgBox.addButton(tr("Calibrate"), QMessageBox::ActionRole);
    msgBox.exec();

    CbobData::instance()->motorsRecalibrate();

    ::sleep(1);

    msgBox.setText("Calibration Done");
    msgBox.removeButton(msgBox.buttons().first());
    msgBox.addButton(tr("Ok"), QMessageBox::ActionRole);
    msgBox.exec();

    storePidCal();
}

void Settings::recalibrateAccel()
{
    QMessageBox msgBox;
    msgBox.setText("Place CBC on level surface");
    msgBox.addButton(tr("Calibrate"), QMessageBox::ActionRole);
    msgBox.exec();

    CbobData::instance()->accelerometerRecalibrate();

    ::sleep(1);

    msgBox.setText("Calibration Done");
    msgBox.removeButton(msgBox.buttons().first());
    msgBox.addButton(tr("Ok"), QMessageBox::ActionRole);
    msgBox.exec();

    storeAccelCal();
}

void Settings::resetPID()
{
    int i;
    for(i=0;i<4;i++) CbobData::instance()->defaultPIDgains(i);
}

void Settings::setCameraDefault()
{
#ifdef QT_ARCH_ARM
  QFile::remove("/mnt/user/vision/track_colors");
#else
  QFile::remove(QDir::homePath().toStdString() + "/track_colors");
#endif
}

void Settings::on_ui_consoleShowBox_clicked(bool checked)
{
    QSettings settings("/mnt/user/cbc_v2.config",QSettings::NativeFormat);

    settings.setValue("consoleShowOnRun",checked);
    settings.sync();
    ::system("sync");
    ::system("sync");
}

void Settings::loadAccelCal()
{
    QSettings settings("/mnt/user/cbc_v2.config",QSettings::NativeFormat);
    short data[3];

    for(int i = 0;i < 3;i++) {
        data[i] = settings.value("accelCal" + QString::number(i)).toInt();
    }

    CbobData::instance()->accelerometerSetCal(data);
}

void Settings::storeAccelCal()
{
    QSettings settings("/mnt/user/cbc_v2.config",QSettings::NativeFormat);
    short data[3];

    CbobData::instance()->accelerometerGetCal(data);

    for(int i = 0;i < 3;i++) {
        settings.setValue("accelCal" + QString::number(i), data[i]);
    }
}

void Settings::loadPidCal()
{
    QSettings settings("/mnt/user/cbc_v2.config",QSettings::NativeFormat);
    short data[4];

    for(int i = 0;i < 4;i++) {
        data[i] = settings.value("motorCal" + QString::number(i)).toInt();
    }

    CbobData::instance()->motorsSetCal(data);
}

void Settings::storePidCal()
{
    QSettings settings("/mnt/user/cbc_v2.config",QSettings::NativeFormat);
    short data[4];

    CbobData::instance()->motorsGetCal(data);

    for(int i = 0;i < 4;i++) {
        settings.setValue("motorCal"+QString::number(i), data[i]);
    }
}

