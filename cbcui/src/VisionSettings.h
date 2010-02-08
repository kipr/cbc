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

#ifndef __VISION_SETTINGS_H__
#define __VISION_SETTINGS_H__

#include "ui_VisionSettings.h"
#include "Page.h"
#include <QSettings>

#include "vision/ColorTracker.h"
#include "vision/Camera.h"
#include "vision/ImageDisplay.h"

class VisionSettings : public Page, private Ui::VisionSettings
{
    Q_OBJECT

public:
    VisionSettings(QWidget *parent = 0, Camera *camera = 0, ColorTracker *tracker = 0);
    ~VisionSettings();

    ImageDisplay *m_ImageDisplay;
    virtual void showEvent(QShowEvent *event);
    bool saveSettings();
    bool loadSettings();

protected:
    Camera  *m_camera;
    ColorTracker *m_tracking;

public slots:
    void on_ui_visionSettingRtButton_clicked();
    void on_ui_visionSettingLfButton_clicked();
    void show();
    void hide();


    void on_ui_autoWhiteBalCheckBox_stateChanged(int state);
    void on_ui_redBalLineEdit_selectionChanged();
    void on_ui_blueBalLineEdit_selectionChanged();
    void on_ui_gammaLineEdit_selectionChanged();

    void on_ui_exposureLineEdit_selectionChanged();
    void on_ui_autoExposureCheckBox_stateChanged(int state);

    void on_ui_brightnessLineEdit_selectionChanged();
    void on_ui_contrastLineEdit_selectionChanged();
    void on_ui_sharpnessLineEdit_selectionChanged();

};

#endif
