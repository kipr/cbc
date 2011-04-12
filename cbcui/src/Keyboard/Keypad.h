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

#ifndef __KEYPAD_H__
#define __KEYPAD_H__

#include "ui_Keypad.h"
#include <QDialog>

class Keypad : public QDialog, private Ui::Keypad
{
        Q_OBJECT

protected:
    Keypad(QWidget *parent = 0);
    static Keypad *m_numKeypad;
    ~Keypad();

public:
    static Keypad *initialize(QWidget *parent = 0){
        if(!m_numKeypad)
            m_numKeypad = new Keypad(parent);
        return m_numKeypad;
    }

    static Keypad *instance(){
        return m_numKeypad;
    }
    static Keypad *destroy() {
        delete m_numKeypad;
        m_numKeypad = 0;
        return 0;
    }

public slots:
    void on_ui_oneButton_clicked(bool checked = false);
    void on_ui_twoButton_clicked(bool checked = false);
    void on_ui_threeButton_clicked(bool checked = false);
    void on_ui_fourButton_clicked(bool checked = false);
    void on_ui_fiveButton_clicked(bool checked = false);
    void on_ui_sixButton_clicked(bool checked = false);
    void on_ui_sevenButton_clicked(bool checked = false);
    void on_ui_eightButton_clicked(bool checked = false);
    void on_ui_nineButton_clicked(bool checked = false);
    void on_ui_zeroButton_clicked(bool checked = false);
    void on_ui_enterButton_clicked(bool checked = false);
    void on_ui_negateButton_clicked(bool checked = false);
    void on_ui_clearButton_clicked(bool checked = false);
    void show();
    int  getValue();
    QString getString();
    void setRange(int min,int max);
    void setType(int typ);

private:
    int keypadType;
    void refreshView();
    int  userValue;
    QString userString;
    int minVal;
    int maxVal;
};

#endif // KEYPAD_H
