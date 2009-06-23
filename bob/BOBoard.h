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
 
 /*
 *  BOBoard.h
 *
 * Setup header file for ARM AT91SAM7S64 on the BOBoard
 * 
 */

#include "AT91SAM7S64.h"
#ifndef __BOB_H__
#define __BOB_H__

#define FIRMWARE_VER	"CBC ver 0.4"

#define nop()  __asm__ __volatile__("nop")

#define EXT_OCC		20000000
#define MCK			50000000


#define USB_SERIAL_BAUD		38400
#define CREATE_SERIAL_BAUD	57600

// ARM SPI pins
#define SPI_CS		(1<<11)
#define SPI_MISO	(1<<12)
#define SPI_MOSI	(1<<13)
#define SPI_SCK		(1<<14)

// ARM TWI pins
#define TWI_SDA		(1<<3)
#define TWI_SCL		(1<<4)

// ARM DIGITAL ports pin locations
#define DIGITAL0	(1<<30)
#define DIGITAL1	(1<<29)
#define DIGITAL2	(1<<28)
#define	DIGITAL3	(1<<27)
#define DIGITAL4	(1<<2)
#define DIGITAL5	(1<<1)
#define DIGITAL6	(1<<0)
#define DIGITAL7	(1<<25)

// ARM ANALOG ports pin locations
// Remapping analog pins due to trace layout on the board
#define ANALOG0		iADC->ADC_CDR4 /* (1<<20) */
#define ANALOG1		iADC->ADC_CDR5 /* (1<<19) */
#define ANALOG2		iADC->ADC_CDR6 /* (1<<18) */
#define ANALOG3		iADC->ADC_CDR7 /* (1<<17) */
#define ANALOG4		iADC->ADC_CDR0 /* (1<<17) */
#define ANALOG5		iADC->ADC_CDR1 /* (1<<18) */
#define ANALOG6		iADC->ADC_CDR2 /* (1<<19) */
#define ANALOG7		iADC->ADC_CDR3 /* (1<<20) */

// RESET IO pin location
#define RST_IOPIN	(1<<9)
// BUTTON pin
#define BUTTON		(1<<23)
// CHUMBY bend sensor output
#define BEND_IOPIN	(1<<10)

//////////////////////////////////////////////
// ACCELEROMETER SETUP PINS
/* ACC scale select pins
 *	GSEL2		GSEL1		range
 *	 0			 0			+/- 1.5g
 *	 0			 1			+/- 2g
 *	 1			 0			+/- 4g
 *	 1			 1			+/- 6g
 */
#define ACC_GSEL1	(1<<15)
#define ACC_GSEL2	(1<<24)


/////////////////////////////////////////////
// PWM SETUP
// RESET pin
#define PWM_RESET	(1<<16)
// TWI address for PWM PCA9635
#define PWM_ADDR (0x0f)

/////////////////////////////////////////////
// iADC	internal ADC setup
#define ADCCLOCK	1000000	// clock in MHz
#define ADCSTARTUP	11		// 0.0001s startup time
#define ADCSAMPHOLD	9		// 0.00001s sample and hold time

/////////////////////////////////////////////
// EADC external ADC setup
// TWI address for EADC MAX1239
#define EADC_ADDR (0x35)		// external address
#define EADC_SETUP (0x83)		// setup byte
#define EADC_MOTOR_CONFIG (0xE)	// configuration for differential readings of motor ports up to port 7
#define EADC_ACCV_CONFIG (0x57)	// configuration for single ended readings for batt voltage port 8 and accelerometer port 9,10,11, starts reading from port AIN6
#define BEMF3_BLUE	(0x0)
#define BEMF3_RED	(0x1)
#define BEMF2_BLUE  (0x3)
#define BEMF2_RED	(0X2)
#define BEMF1_BLUE	(0x4)
#define BEMF1_RED	(0x5)
#define BEMF0_BLUE	(0x7)
#define BEMF0_RED	(0x6)

#define VOLT		(0x8)
#define ACCX		(0x9)
#define ACCY		(0xA)
#define ACCZ		(0xB)

////////////////////////////////////////////
// Back EMF setup
////////////////////////////////////////////
#define INDUCTION_SPIKE_DELAY		1400 //660 microsecond delay time to allow back EMF voltage to settle
#define MAX_PID_ERR		1600

#include "bob_struct.h"

#endif

