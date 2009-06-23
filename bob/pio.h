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
 
#ifndef __PIO_H__
#define __PIO_H__

#include "BOBoard.h"

#define PIO_Enable(pin) (AT91C_BASE_PIOA->PIO_PER = 1U<<(pin))
#define PIO_Disable(pin) (AT91C_BASE_PIOA->PIO_PDR = 1U<<(pin))

#define PIO_EnableOutput(pin) (AT91C_BASE_PIOA->PIO_OER = 1U<<(pin))
#define PIO_DisableOutput(pin) (AT91C_BASE_PIOA->PIO_ODR = 1U<<(pin))

#define PIO_SetPeripheralA(pin) (AT91C_BASE_PIOA->PIO_ASR = 1U<<(pin))
#define PIO_SetPeripheralB(pin) (AT91C_BASE_PIOA->PIO_BSR = 1U<<(pin))

#define	PIO_EnablePullup(pin) (AT91C_BASE_PIOA->PIO_PPUER = 1U<<(pin))
#define PIO_DisablePullup(pin) (AT91C_BASE_PIOA->PIO_PPUDR = 1U<<(pin))

#endif
