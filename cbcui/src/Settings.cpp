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
#include <QMessageBox>
#include <QDir>

Settings::Settings(QWidget *parent) :
        Page(parent),
        m_settings("/mnt/kiss/config/cbc_v2.config",QSettings::NativeFormat),
        m_brightness(parent),
        m_volume(parent)
{
    setupUi(this);

    QObject::connect(ui_recalibrateMotorsButton, SIGNAL(clicked()), this, SLOT(recalibrateMotors()));
    QObject::connect(ui_recalibrateAccelerometerButton, SIGNAL(clicked()), this, SLOT(recalibrateAccel()));
    QObject::connect(ui_recalibrateTouchscreenButton, SIGNAL(clicked()), this, SLOT(recalibrateTS()));
    QObject::connect(ui_resetPIDButton, SIGNAL(clicked()), this, SLOT(resetPID()));
    QObject::connect(ui_cameraDefaultButton, SIGNAL(clicked()), this, SLOT(setCameraDefault()));
    QObject::connect(ui_brightnessButton, SIGNAL(clicked()), &m_brightness, SLOT(raisePage()));
    QObject::connect(ui_volumeButton, SIGNAL(clicked()), &m_volume, SLOT(raisePage()));

    if(m_settings.contains("motorCal0")) loadPidCal();
    else storePidCal();
    if(m_settings.contains("accelCal0")) loadAccelCal();
    else storeAccelCal();
    m_settings.beginGroup("PIDgainsMotor0");
    if(!m_settings.contains("ProportionalMult")) resetPID();
    m_settings.endGroup();

    ::system("sync");
    ::system("sync");

    ui_consoleShowBox->setChecked(m_settings.value("consoleShowOnRun", true).toBool());
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

void Settings::recalibrateTS()
{
    QMessageBox msgBox;
    msgBox.setText("Calibrating the touch screen!");
    msgBox.addButton(tr("Calibrate"), QMessageBox::ActionRole);
    msgBox.exec();

    system("/mnt/kiss/qt/bin/ts_calibrate && /mnt/kiss/gui/startup.sh &");
    system("killall cbcui");
}

void Settings::resetPID()
{
    int i;
    for(i=0;i<4;i++) CbobData::instance()->defaultPIDgains(i);
}

void Settings::setCameraDefault()
{
#ifdef QT_ARCH_ARM
  QFile::remove("/mnt/kiss/config/track_colors");
#else
  QString str = QDir::homePath() + "/track_colors";
  QFile::remove(str);
#endif
}

void Settings::on_ui_consoleShowBox_clicked(bool checked)
{
    m_settings.setValue("consoleShowOnRun",checked);
    m_settings.sync();
    ::system("sync");
    ::system("sync");
}

void Settings::loadAccelCal()
{
    short data[3];

    for(int i = 0;i < 3;i++) {
        data[i] = m_settings.value("accelCal" + QString::number(i)).toInt();
    }

    CbobData::instance()->accelerometerSetCal(data);
}

void Settings::storeAccelCal()
{
    short data[3];

    CbobData::instance()->accelerometerGetCal(data);

    for(int i = 0;i < 3;i++) {
        m_settings.setValue("accelCal" + QString::number(i), data[i]);
    }
    m_settings.sync();
}

void Settings::loadPidCal()
{
    short data[4];

    for(int i = 0;i < 4;i++) {
        data[i] = m_settings.value("motorCal" + QString::number(i)).toInt();
    }

    CbobData::instance()->motorsSetCal(data);
}

void Settings::storePidCal()
{
    short data[4];

    CbobData::instance()->motorsGetCal(data);

    for(int i = 0;i < 4;i++) {
        m_settings.setValue("motorCal"+QString::number(i), data[i]);
    }
    m_settings.sync();
}

