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
#include "Keypad.h"
#include <QMessageBox>
#include <QProcess>

Keypad::Keypad(QWidget *parent, int min, int max)
    : QDialog(parent),minVal(min),maxVal(max)
{
    setupUi(this);
    this->move(100,25);

#ifdef QT_ARCH_ARM
    setWindowState(windowState() | Qt::WindowActive);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    setWindowModality(Qt::ApplicationModal);
#endif
    userValue = 0;

}

Keypad::~Keypad()
{
}

void Keypad::show()
{
    ui_outputLine->clear();
    userValue = 0;
    this->raise();
}
void Keypad::on_ui_oneButton_clicked(bool)
{
    ui_outputLine->insert(QString::number(1));
}
void Keypad::on_ui_twoButton_clicked(bool)
{
    ui_outputLine->insert(QString::number(2));
}
void Keypad::on_ui_threeButton_clicked(bool)
{
    ui_outputLine->insert(QString::number(3));
}
void Keypad::on_ui_fourButton_clicked(bool)
{
    ui_outputLine->insert(QString::number(4));
}
void Keypad::on_ui_fiveButton_clicked(bool)
{
    ui_outputLine->insert(QString::number(5));
}
void Keypad::on_ui_sixButton_clicked(bool)
{
    ui_outputLine->insert(QString::number(6));
}
void Keypad::on_ui_sevenButton_clicked(bool)
{
    ui_outputLine->insert(QString::number(7));
}
void Keypad::on_ui_eightButton_clicked(bool)
{
    ui_outputLine->insert(QString::number(8));
}
void Keypad::on_ui_nineButton_clicked(bool)
{
    ui_outputLine->insert(QString::number(9));
}
void Keypad::on_ui_zeroButton_clicked(bool)
{
    if(ui_outputLine->text() == "" || ui_outputLine->text().toInt() == 0) return;
    ui_outputLine->insert(QString::number(0));
}
void Keypad::on_ui_negateButton_clicked(bool)
{
    QString value = ui_outputLine->text();
    userValue = value.toInt();
    userValue = -userValue;
    this->refreshView();
}
void Keypad::on_ui_clearButton_clicked(bool)
{
    ui_outputLine->clear();
    userValue = 0;
}
void Keypad::on_ui_enterButton_clicked(bool)
{
    QString value = ui_outputLine->text();
    userValue = value.toInt();
    if(userValue < minVal || userValue > maxVal){
        QProcess::startDetached("aplay /mnt/kiss/sounds/quack.wav");
        QMessageBox::warning(this,
                             "Input Error",
                             QString("Value must be between %1 and %2\n").arg(minVal).arg(maxVal),
                             QMessageBox::Ok,
                             QMessageBox::NoButton);
        ui_outputLine->setText("");
    }
    else this->close();
}
void Keypad::refreshView()
{
    ui_outputLine->setText(QString::number(userValue));
}

int Keypad::getValue()
{
    return userValue;
}

void Keypad::setRange(int min, int max)
{
    minVal = min;
    maxVal = max;
}
