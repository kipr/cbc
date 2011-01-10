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

#include "VisionTracking.h"

VisionTracking::VisionTracking(QWidget *parent, ColorTracker *colorTracker)
    : Page(parent),
    m_ColorTracker(colorTracker)
{
    setupUi(this);

    m_ImageDisplay = new ImageDisplay();
    image->addWidget(m_ImageDisplay);

    m_HSVRangeDisplay = new HSVRangeDisplay();
    hsv->addWidget(m_HSVRangeDisplay);

    //qWarning("setting display model 0");
    displayModel(0);

    // qWarning("setting up model buttons");

    Model0Button->setCheckable(true);
    Model1Button->setCheckable(true);
    Model2Button->setCheckable(true);
    Model3Button->setCheckable(true);

    m_modelGroup.addButton(Model0Button);
    m_modelGroup.addButton(Model1Button);
    m_modelGroup.addButton(Model2Button);
    m_modelGroup.addButton(Model3Button);
    Model0Button->click();

    //qWarning("Setting up track button");
    RawButton->setCheckable(true);
    MatchButton->setCheckable(true);
    TrackButton->setCheckable(true);
    m_modeGroup.addButton(RawButton);
    m_modeGroup.addButton(MatchButton);
    m_modeGroup.addButton(TrackButton);
    TrackButton->click();


    //qWarning("setting up other buttons");
    TopLeftButton->setCheckable(true);
    BottomRightButton->setCheckable(true);
    m_tlbrGroup.addButton(TopLeftButton);
    m_tlbrGroup.addButton(BottomRightButton);
    TopLeftButton->click();

    HSVLabel->setText(" ");
}

VisionTracking::~VisionTracking()
{
}

void VisionTracking::show()
{
    m_ColorTracker->loadModels();
    this->updateModel();
    Page::show();
}
void VisionTracking::hide()
{
    m_ColorTracker->saveModels();
    Page::hide();
}

void VisionTracking::setModel(const HSVRange &model)
{
    m_ColorTracker->setModel(m_ColorTracker->getDisplayModel(), model);
    updateModel();
}

void VisionTracking::updateModel()
{
    HSVRange model = getModel();
    m_HSVRangeDisplay->setRange(model);
}

void VisionTracking::updateModelLabel()
{
    HSVRange model = getModel();
    HSVLabel->setText(QString("(%1,%2,\n  %3,%4)").arg(model.h.min).arg(model.h.max).arg(model.s.min).arg(model.v.min));
}

void VisionTracking::displayModel(int ch)
{
    m_ColorTracker->setDisplayModel(ch);
    updateModel();
}

template <class T>
        static void tweak(T &val, int delta, int min, int max)
{
    int newval = val + delta;
    if (newval < min) newval = min;
    if (newval > max) newval = max;
    val = (T) newval;
}

void VisionTracking::moveHorizontally(int dx) {
    HSVRange model(getModel());
    if (m_tl) tweak(model.s.min,  dx, 0, 255);
    else      tweak(model.v.min, -dx, 0, 255);
    setModel(model);
}

template <class T>
        static void mod360_tweak(T &val, int delta)
{
    int newval = val + 360 + delta;
    val = (T) (newval % 360);
}

void VisionTracking::moveVertically(int dy) {
    HSVRange model(getModel());
    HSVRange oldModel(model);
    int min_hue_range = 5;
    int max_hue_range = 120;

    mod360_tweak(m_tl ? model.h.min : model.h.max, dy);
    int new_range = (model.h.max + 360 - model.h.min) % 360;
    if (new_range < min_hue_range || new_range > max_hue_range) {
        // If we made the hue range too much or too little, revert and move
        // both bounds
        model = oldModel;
        mod360_tweak(model.h.min, dy);
        mod360_tweak(model.h.max, dy);
    }
    setModel(model);
}

void VisionTracking::showEvent(QShowEvent *) {
    if (m_ColorTracker) m_ColorTracker->setImageDisplay(m_ImageDisplay);
#ifdef QT_ARCH_ARM
    setWindowState(windowState() | Qt::WindowFullScreen);
#endif
}

void VisionTracking::on_Model0Button_clicked() {
    displayModel(0);
    Model1Button->setChecked(false);
    Model2Button->setChecked(false);
    Model3Button->setChecked(false);
}
void VisionTracking::on_Model1Button_clicked() {
    displayModel(1);
    Model0Button->setChecked(false);
    Model2Button->setChecked(false);
    Model3Button->setChecked(false);
}
void VisionTracking::on_Model2Button_clicked() {
    displayModel(2);
    Model0Button->setChecked(false);
    Model1Button->setChecked(false);
    Model3Button->setChecked(false);
}
void VisionTracking::on_Model3Button_clicked() {
    displayModel(3);
    Model0Button->setChecked(false);
    Model1Button->setChecked(false);
    Model2Button->setChecked(false);
}

void VisionTracking::on_RawButton_clicked() {
    m_ColorTracker->setDisplayMode(ColorTracker::DisplayRaw);
    MatchButton->setChecked(false);
    TrackButton->setChecked(false);
}
void VisionTracking::on_MatchButton_clicked() {
    m_ColorTracker->setDisplayMode(ColorTracker::DisplayMatches);
    RawButton->setChecked(false);
    TrackButton->setChecked(false);
}
void VisionTracking::on_TrackButton_clicked() {
    m_ColorTracker->setDisplayMode(ColorTracker::DisplayBlobs);
    RawButton->setChecked(false);
    MatchButton->setChecked(false);
}

void VisionTracking::on_TopLeftButton_clicked() {
    m_tl = true;
    BottomRightButton->setChecked(false);
}
void VisionTracking::on_BottomRightButton_clicked() {
    m_tl = false;
    TopLeftButton->setChecked(false);
}
