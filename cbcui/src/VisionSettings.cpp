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

#include "VisionSettings.h"
#include "Keypad.h"
#include <QMessageBox>

VisionSettings::VisionSettings(QWidget *parent, Camera *camera, ColorTracker *tracker) : Page(parent), m_camera(camera), m_tracking(tracker)
{
    setupUi(this);
    m_ImageDisplay = new ImageDisplay();
    camImage->addWidget(m_ImageDisplay);
}

VisionSettings::~VisionSettings()
{
}

void VisionSettings::show(){
    //if(!this->loadSettings())

    Page::show();
}
void VisionSettings::hide(){
    this->saveSettings();
    Page::hide();
}

bool VisionSettings::loadSettings()
{

}

bool VisionSettings::saveSettings()
{

}

void VisionSettings::showEvent(QShowEvent *) {
    //if (m_tracking) m_tracking->setImageDisplay(m_ImageDisplay);
#ifdef QT_ARCH_ARM
  setWindowState(windowState() | Qt::WindowFullScreen);
#endif
}

void VisionSettings::on_ui_visionSettingRtButton_clicked()
{
    int index = ui_paramStack->currentIndex();
    if(index == ui_paramStack->count()-1) index = 0;
    else index++;
    ui_paramStack->setCurrentIndex(index);
}

void VisionSettings::on_ui_visionSettingLfButton_clicked()
{
    int index = ui_paramStack->currentIndex();
    if(index == 0) index = ui_paramStack->count()-1;
    else index--;
    ui_paramStack->setCurrentIndex(index);
}

void VisionSettings::on_ui_autoWhiteBalCheckBox_stateChanged(int state)
{
    m_camera->setParameter(AUTO_WHITE_BALANCE,state);
    ui_redBalLineEdit->setEnabled(!state);
    ui_blueBalLineEdit->setEnabled(!state);
}

void VisionSettings::on_ui_redBalLineEdit_selectionChanged()
{
    Keypad user_keypad(this);

    ui_redBalLineEdit->setStyleSheet("QLineEdit#ui_redBalLineEdit{background-color:red}");
    user_keypad.exec();
    int value = user_keypad.getValue();

    if(value < 0 || value > 127){
        QMessageBox::warning(this,
                             "Input Error",
                             "Value must be between 0 and 127\n",
                             QMessageBox::Ok,
                             QMessageBox::NoButton);
        value = 31;
    }
    m_camera->setParameter(RED_BALANCE,value);
    ui_redBalLineEdit->setStyleSheet("QLineEdit#ui_redBalLineEdit{background-color:white}");
    ui_redBalLineEdit->setText(QString::number(value));
}

void VisionSettings::on_ui_blueBalLineEdit_selectionChanged()
{
    Keypad user_keypad(this);

    ui_blueBalLineEdit->setStyleSheet("QLineEdit#ui_blueBalLineEdit{background-color:red}");
    user_keypad.exec();
    int value = user_keypad.getValue();

    if(value < 0 || value > 127){
        QMessageBox::warning(this,
                             "Input Error",
                             "Value must be between 0 and 127\n",
                             QMessageBox::Ok,
                             QMessageBox::NoButton);
        value = 31;
    }
    m_camera->setParameter(BLUE_BALANCE,value);
    ui_blueBalLineEdit->setStyleSheet("QLineEdit#ui_blueBalLineEdit{background-color:white");
    ui_blueBalLineEdit->setText(QString::number(value));
}

void VisionSettings::on_ui_gammaLineEdit_selectionChanged()
{
    Keypad user_keypad(this);

    ui_gammaLineEdit->setStyleSheet("QLineEdit#ui_gammaLineEdit{background-color:red}");
    user_keypad.exec();
    int value = user_keypad.getValue();

    if(value < 0 || value > 65280){
        QMessageBox::warning(this,
                             "Input Error",
                             "Value must be between 0 and 65280\n",
                             QMessageBox::Ok,
                             QMessageBox::NoButton);
        value = 13107;
    }
    m_camera->setParameter(GAMMA,value);
    ui_gammaLineEdit->setStyleSheet("QLineEdit#ui_gammaLineEdit{background-color:white}");
    ui_gammaLineEdit->setText(QString::number(value));
}

void VisionSettings::on_ui_exposureLineEdit_selectionChanged()
{
    Keypad user_keypad(this);

    ui_exposureLineEdit->setStyleSheet("QLineEdit#ui_exposureLineEdit{background-color:red}");
    user_keypad.exec();
    int value = user_keypad.getValue();

    if(value < 0 || value > 65280){
        QMessageBox::warning(this,
                             "Input Error",
                             "Value must be between 0 and 65280\n",
                             QMessageBox::Ok,
                             QMessageBox::NoButton);
        value = 100;
    }

    m_camera->setParameter(EXPOSURE,value);
    ui_exposureLineEdit->setStyleSheet("QLineEdit#ui_exposureLineEdit{background-color:white}");
    ui_exposureLineEdit->setText(QString::number(value));
}

void VisionSettings::on_ui_autoExposureCheckBox_stateChanged(int state)
{
    m_camera->setParameter(AUTO_EXPOSURE,state);
    ui_exposureLineEdit->setEnabled(!state);
}

void VisionSettings::on_ui_brightnessLineEdit_selectionChanged()
{
    Keypad user_keypad(this);

    ui_brightnessLineEdit->setStyleSheet("QLineEdit#ui_brightnessLineEdit{background-color:red}");
    user_keypad.exec();
    int value = user_keypad.getValue();

    if(value < 0 || value > 65280){
        QMessageBox::warning(this,
                             "Input Error",
                             "Value must be between 0 and 65280\n",
                             QMessageBox::Ok,
                             QMessageBox::NoButton);
        value = 32767;
    }
    m_camera->setParameter(BRIGHTNESS,value);
    ui_brightnessLineEdit->setStyleSheet("QLineEdit#ui_brightnessLineEdit{background-color:white}");
    ui_brightnessLineEdit->setText(QString::number(value));
}

void VisionSettings::on_ui_contrastLineEdit_selectionChanged()
{
    Keypad user_keypad(this);

    ui_contrastLineEdit->setStyleSheet("QLineEdit#ui_contrastLineEdit{background-color:red}");
    user_keypad.exec();
    int value = user_keypad.getValue();

    if(value < 0 || value > 65280){
        QMessageBox::warning(this,
                             "Input Error",
                             "Value must be between 0 and 65280\n",
                             QMessageBox::Ok,
                             QMessageBox::NoButton);
        value = 32767;
    }
    m_camera->setParameter(CONTRAST,value);
    ui_contrastLineEdit->setStyleSheet("QLineEdit#ui_contrastLineEdit{background-color:white}");
    ui_contrastLineEdit->setText(QString::number(value));
}

void VisionSettings::on_ui_sharpnessLineEdit_selectionChanged()
{
    Keypad user_keypad(this);

    ui_sharpnessLineEdit->setStyleSheet("QLineEdit#ui_sharpnessLineEdit{background-color:red}");
    user_keypad.exec();
    int value = user_keypad.getValue();

    if(value < 0 || value > 63){
        QMessageBox::warning(this,
                             "Input Error",
                             "Value must be between 0 and 63\n",
                             QMessageBox::Ok,
                             QMessageBox::NoButton);
        value = 31;
    }
    m_camera->setParameter(SHARPNESS,value);
    ui_sharpnessLineEdit->setStyleSheet("QLineEdit#ui_sharpnessLineEdit{background-color:white}");
    ui_sharpnessLineEdit->setText(QString::number(value));
}


