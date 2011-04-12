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
#include "Keyboard/Keypad.h"

VisionSettings::VisionSettings(QWidget *parent, Camera *camera, RawView *rview) : Page(parent), m_camera(camera), m_rawview(rview)
{
    setupUi(this);
    m_ImageDisplay = new ImageDisplay();
    camImage->addWidget(m_ImageDisplay);

    ui_paramStack->setCurrentIndex(0);
}

VisionSettings::~VisionSettings()
{
}

void VisionSettings::show()
{
    this->loadSettings();
    Page::show();
}

void VisionSettings::hide()
{
    Page::hide();
}

void VisionSettings::loadSettings()
{
    int state;
    state = m_camera->getParameter(AUTO_WHITE_BALANCE);
    ui_autoWhiteBalCheckBox->setChecked(state);

    state = m_camera->getParameter(RED_BALANCE);
    ui_redBalLineEdit->setText(QString::number(state));
    ui_redBalSlider->setSliderPosition(state);

    state = m_camera->getParameter(BLUE_BALANCE);
    ui_blueBalLineEdit->setText(QString::number(state));
    ui_blueBalSlider->setSliderPosition(state);

    state = m_camera->getParameter(GAMMA)/256;
    ui_gammaLineEdit->setText(QString::number(state));
    ui_gammaSlider->setSliderPosition(state);

    state = m_camera->getParameter(EXPOSURE)/256;
    ui_exposureLineEdit->setText(QString::number(state));
    ui_exposureSlider->setSliderPosition(state);

    state = m_camera->getParameter(AUTO_EXPOSURE);
    ui_autoExposureCheckBox->setChecked(state);

    state = m_camera->getParameter(BRIGHTNESS)/256;
    ui_brightnessLineEdit->setText(QString::number(state));
    ui_brightnessSlider->setSliderPosition(state);

    state = m_camera->getParameter(CONTRAST)/256;
    ui_contrastLineEdit->setText(QString::number(state));
    ui_contrastSlider->setSliderPosition(state);

    state = m_camera->getParameter(SHARPNESS);
    ui_sharpnessLineEdit->setText(QString::number(state));
    ui_sharpnessSlider->setSliderPosition(state);
}

void VisionSettings::showEvent(QShowEvent *) {
    if(m_rawview) m_rawview->setImageDisplay(m_ImageDisplay);
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
    ui_redBalSlider->setEnabled(!state);
    ui_blueBalSlider->setEnabled(!state);
}
void VisionSettings::on_ui_redBalLineEdit_selectionChanged()
{
    Keypad *user_keypad = Keypad::instance();
    user_keypad->setRange(0,127);
    user_keypad->setType(0);

    ui_redBalLineEdit->setStyleSheet("QLineEdit#ui_redBalLineEdit{background-color:red}");
    user_keypad->exec();
    int value = user_keypad->getValue();

    m_camera->setParameter(RED_BALANCE,value);
    ui_redBalLineEdit->setStyleSheet("QLineEdit#ui_redBalLineEdit{background-color:white}");
    ui_redBalLineEdit->setText(QString::number(value));
    ui_redBalSlider->setSliderPosition(value);
}
void VisionSettings::on_ui_blueBalLineEdit_selectionChanged()
{
    Keypad *user_keypad = Keypad::instance();
    user_keypad->setRange(0,127);

    ui_blueBalLineEdit->setStyleSheet("QLineEdit#ui_blueBalLineEdit{background-color:red}");
    user_keypad->exec();
    int value = user_keypad->getValue();

    m_camera->setParameter(BLUE_BALANCE,value);
    ui_blueBalLineEdit->setStyleSheet("QLineEdit#ui_blueBalLineEdit{background-color:white");
    ui_blueBalLineEdit->setText(QString::number(value));
    ui_blueBalSlider->setSliderPosition(value);
}
void VisionSettings::on_ui_redBalSlider_valueChanged(int value)
{
    m_camera->setParameter(RED_BALANCE,value);
    ui_redBalLineEdit->setText(QString::number(value));
}
void VisionSettings::on_ui_blueBalSlider_valueChanged(int value)
{
    m_camera->setParameter(BLUE_BALANCE,value);
    ui_blueBalLineEdit->setText(QString::number(value));
}

void VisionSettings::on_ui_gammaLineEdit_selectionChanged()
{
    Keypad *user_keypad = Keypad::instance();
    user_keypad->setRange(0,255);
    user_keypad->setType(0);

    ui_gammaLineEdit->setStyleSheet("QLineEdit#ui_gammaLineEdit{background-color:red}");
    user_keypad->exec();
    int value = user_keypad->getValue();

    m_camera->setParameter(GAMMA,value*256);
    ui_gammaLineEdit->setStyleSheet("QLineEdit#ui_gammaLineEdit{background-color:white}");
    ui_gammaLineEdit->setText(QString::number(value));
    ui_gammaSlider->setSliderPosition(value);
}
void VisionSettings::on_ui_gammaSlider_valueChanged(int value)
{
    m_camera->setParameter(GAMMA,value*256);
    ui_gammaLineEdit->setText(QString::number(value));
}

void VisionSettings::on_ui_exposureLineEdit_selectionChanged()
{
    Keypad *user_keypad = Keypad::instance();
    user_keypad->setRange(0,255);
    user_keypad->setType(0);

    ui_exposureLineEdit->setStyleSheet("QLineEdit#ui_exposureLineEdit{background-color:red}");
    user_keypad->exec();
    int value = user_keypad->getValue();

    m_camera->setParameter(EXPOSURE,value*256);
    ui_exposureLineEdit->setStyleSheet("QLineEdit#ui_exposureLineEdit{background-color:white}");
    ui_exposureLineEdit->setText(QString::number(value));
    ui_exposureSlider->setSliderPosition(value);
}
void VisionSettings::on_ui_exposureSlider_valueChanged(int value)
{
    m_camera->setParameter(EXPOSURE,value*256);
    ui_exposureLineEdit->setText(QString::number(value));
}
void VisionSettings::on_ui_autoExposureCheckBox_stateChanged(int state)
{
    m_camera->setParameter(AUTO_EXPOSURE,state);
    ui_exposureLineEdit->setEnabled(!state);
    ui_exposureSlider->setEnabled(!state);
}


void VisionSettings::on_ui_brightnessLineEdit_selectionChanged()
{
    Keypad *user_keypad = Keypad::instance();
    user_keypad->setRange(0,255);
    user_keypad->setType(0);

    ui_brightnessLineEdit->setStyleSheet("QLineEdit#ui_brightnessLineEdit{background-color:red}");
    user_keypad->exec();
    int value = user_keypad->getValue();

    m_camera->setParameter(BRIGHTNESS,value*256);
    ui_brightnessLineEdit->setStyleSheet("QLineEdit#ui_brightnessLineEdit{background-color:white}");
    ui_brightnessLineEdit->setText(QString::number(value));
    ui_brightnessSlider->setSliderPosition(value);
}
void VisionSettings::on_ui_brightnessSlider_valueChanged(int value)
{
    m_camera->setParameter(BRIGHTNESS,value*256);
    ui_brightnessLineEdit->setText(QString::number(value));
}
void VisionSettings::on_ui_contrastLineEdit_selectionChanged()
{
    Keypad *user_keypad = Keypad::instance();
    user_keypad->setRange(0,255);
    user_keypad->setType(0);

    ui_contrastLineEdit->setStyleSheet("QLineEdit#ui_contrastLineEdit{background-color:red}");
    user_keypad->exec();
    int value = user_keypad->getValue();

    m_camera->setParameter(CONTRAST,value*256);
    ui_contrastLineEdit->setStyleSheet("QLineEdit#ui_contrastLineEdit{background-color:white}");
    ui_contrastLineEdit->setText(QString::number(value));
    ui_contrastSlider->setSliderPosition(value);
}
void VisionSettings::on_ui_contrastSlider_valueChanged(int value)
{
    m_camera->setParameter(CONTRAST,value*256);
    ui_contrastLineEdit->setText(QString::number(value));
}

void VisionSettings::on_ui_sharpnessLineEdit_selectionChanged()
{
    Keypad *user_keypad = Keypad::instance();
    user_keypad->setRange(0,63);
    user_keypad->setType(0);

    ui_sharpnessLineEdit->setStyleSheet("QLineEdit#ui_sharpnessLineEdit{background-color:red}");
    user_keypad->exec();
    int value = user_keypad->getValue();

    m_camera->setParameter(SHARPNESS,value);
    ui_sharpnessLineEdit->setStyleSheet("QLineEdit#ui_sharpnessLineEdit{background-color:white}");
    ui_sharpnessLineEdit->setText(QString::number(value));
    ui_sharpnessSlider->setSliderPosition(value);
}
void VisionSettings::on_ui_sharpnessSlider_valueChanged(int value)
{
    m_camera->setParameter(SHARPNESS,value);
    ui_sharpnessLineEdit->setText(QString::number(value));
}

void VisionSettings::on_ui_defaultButton_clicked()
{
    m_camera->setDefaultParams();
    this->loadSettings();
}
