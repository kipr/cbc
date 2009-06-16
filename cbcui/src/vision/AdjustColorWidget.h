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

#ifndef INCLUDE_AdjustColorWidget_h
#define INCLUDE_AdjustColorWidget_h

// Qt includes
#include <QWidget>
#include <QButtonGroup>

// Local includes
#include <ColorTracker.h>

// UI
#include "ui_AdjustColorWidget.h"

class ImageDisplay;
class HSVRangeDisplay;

class AdjustColorWidget : public QWidget {
  Q_OBJECT
  
public:
  AdjustColorWidget(QWidget *parent, ColorTracker *colorTracker);
  ImageDisplay *m_ImageDisplay;
  HSVRangeDisplay *m_HSVRangeDisplay;
  virtual void showEvent(QShowEvent *event);
  bool saveModels();
  bool loadModels();
  void loadDefaultModels();
  virtual void closeEvent(QCloseEvent *event);

protected:
  Ui::AdjustColorWidget ui;
  ColorTracker *m_ColorTracker;
  QButtonGroup m_modelGroup;
  QButtonGroup m_modeGroup;
  QButtonGroup m_tlbrGroup;
  bool m_tl;
  
  HSVRange getModel() { return m_ColorTracker->getModel(m_ColorTracker->getDisplayModel()); }
  void setModel(const HSVRange &model);
  void updateModel();
  void displayModel(int ch);
  void moveHorizontally(int dx);
  void moveVertically(int dy);
  std::string modelSaveFile() const;
                          
public slots:
  // Select model
  void on_Model0Button_clicked() { displayModel(0); }
  void on_Model1Button_clicked() { displayModel(1); }
  void on_Model2Button_clicked() { displayModel(2); }
  void on_Model3Button_clicked() { displayModel(3); }

  // Select Raw/Match/Track display
  void on_RawButton_clicked()    { m_ColorTracker->setDisplayMode(ColorTracker::DisplayRaw);     }
  void on_MatchButton_clicked()  { m_ColorTracker->setDisplayMode(ColorTracker::DisplayMatches); }
  void on_TrackButton_clicked()  { m_ColorTracker->setDisplayMode(ColorTracker::DisplayBlobs);   }

  // Select TL/BR
  void on_TopLeftButton_clicked()     { m_tl = true;  }
  void on_BottomRightButton_clicked() { m_tl = false; }

  // Up/down/left/right
  void on_UpButton_clicked()    { moveVertically(-1);   }
  void on_DownButton_clicked()  { moveVertically( 1);   }
  void on_LeftButton_clicked()  { moveHorizontally(-1); }
  void on_RightButton_clicked() { moveHorizontally( 1); }
};

  
  
#endif
  
