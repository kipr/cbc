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

VisionTracking::VisionTracking(QWidget *parent, ColorTracker *colorTracker) : Page(parent), m_ColorTracker(colorTracker)
{
  setupUi(this);
  
  m_ImageDisplay = new ImageDisplay();
  image->addWidget(m_ImageDisplay);

  m_HSVRangeDisplay = new HSVRangeDisplay();
  hsv->addWidget(m_HSVRangeDisplay);

  displayModel(0);

  m_modelGroup.addButton(Model0Button);
  m_modelGroup.addButton(Model1Button);
  m_modelGroup.addButton(Model2Button);
  m_modelGroup.addButton(Model3Button);
  Model0Button->click();

  m_modeGroup.addButton(RawButton);
  m_modeGroup.addButton(MatchButton);
  m_modeGroup.addButton(TrackButton);
  TrackButton->click();

  m_tlbrGroup.addButton(TopLeftButton);
  m_tlbrGroup.addButton(BottomRightButton);
  TopLeftButton->click();

  if (!loadModels()) {
    loadDefaultModels();
  }
}

VisionTracking::~VisionTracking()
{
}

void VisionTracking::closeEvent(QCloseEvent */*event*/) {
  saveModels();
}

void VisionTracking::setModel(const HSVRange &model) {
  m_ColorTracker->setModel(m_ColorTracker->getDisplayModel(), model);
  updateModel();
}

void VisionTracking::updateModel()
{
  m_HSVRangeDisplay->setRange(getModel());
}

void VisionTracking::displayModel(int ch) {
  m_ColorTracker->setDisplayModel(ch);
  updateModel();
}

std::string VisionTracking::modelSaveFile() const {
#ifdef QT_ARCH_ARM  
  return "/mnt/user/vision/track_colors";
#else
  return QDir::homePath().toStdString() + "/track_colors";
#endif
}

bool VisionTracking::loadModels()
{
  bool ret=m_ColorTracker->loadModels(modelSaveFile().c_str());
  updateModel();
  return ret;
}

bool VisionTracking::saveModels()
{
  return m_ColorTracker->saveModels(modelSaveFile().c_str());
}

void VisionTracking::loadDefaultModels()
{
  HSVRange redTraining(HSV(330, 127, 127), HSV(30, 255, 255));
  m_ColorTracker->setModel(0, redTraining);
  
  HSVRange yellowTraining(HSV(30, 127, 127), HSV(90, 255, 255));
  m_ColorTracker->setModel(1, yellowTraining);

  HSVRange greenTraining(HSV(90, 127, 127), HSV(150, 255, 255));
  m_ColorTracker->setModel(2, greenTraining);

  HSVRange blueTraining(HSV(210, 127, 127), HSV(270, 255, 255));
  m_ColorTracker->setModel(3, blueTraining);

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
