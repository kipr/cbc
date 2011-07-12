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

#ifndef __CBCSERIAL_H__
#define __CBCSERIAL_H__

#ifdef __cplusplus
extern "C" {
#endif

#define CREATE_UART "/dev/uart1"

void serial_init();
void serial_quit();

int serial_read(char *data, int count);
int serial_write(char *data, int count);

char serial_read_byte();
void serial_wryte_byte(char byte);

void serial_flush_output();
void serial_flush_input();
void serial_flush();

#ifdef __cplusplus
}
#endif

#endif
