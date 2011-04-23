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

#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#include "ui_Utilities.h"
#include "Page.h"

#include "CreateStatus.h"
#include "Serial/SerialPage.h"
#include "Wireless/Wireless.h"

class Utilities : public Page, private Ui::Utilities
{
    Q_OBJECT

public:
    Utilities(QWidget *parent = 0);
    ~Utilities();

private:
    CreateStatus    m_createStatus;
    SerialPage      m_serialPage;
    Wireless        m_wireless;
};

#endif
