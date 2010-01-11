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

#include "Page.h"

Page *Page::m_currentPage = 0;

Page::Page(QWidget *parent) : QWidget(parent), m_lastPage(this)
{
  hide();
}

Page::~Page()
{
}

void Page::raiseLastPage()
{
  m_currentPage->hide();
  m_currentPage = m_currentPage->lastPage();
  m_currentPage->show();
}

void Page::raisePage()
{
   if(this == m_currentPage) return;
   
   if(m_currentPage) {
    m_lastPage = m_currentPage;
    m_lastPage->hide();
  }
   m_currentPage = this;
   this->show();
}
    
Page *Page::currentPage()
{
   return m_currentPage;
}

Page *Page::lastPage()
{
  return m_lastPage;
}

void Page::show()
{
   QWidget::show();
}

void Page::hide()
{
   QWidget::hide();
}

