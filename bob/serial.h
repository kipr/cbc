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
 
#ifndef __SERIAL_H__
#define __SERIAL_H__

#include "BOBoard.h"
#include "util.h"

#define BAUD_38400 38400

#define UART0_RXPIN 5
#define UART0_TXPIN 6
#define UART0_PMC_ID 6

#define UART1_RXPIN 21
#define UART1_TXPIN 22
#define UART1_PMC_ID 7

#define SERIAL_BUFFER_SIZE 128

void UART_Init(int uart_id, int baud);
void UART_SetBaud(int uart_id, int baud);

void UART_Disable(int uart_id);

int UART_Read(int uart_id, uint8 *data, int count);
int UART_Write(int uart_id, uint8 *data, int count);

uint8 UART_ReadByte(int uart_id);
void UART_WriteByte(int uart_id, char byte);

void UART_WriteString(int uart_id, char *str);

void UART_Flush(int uart_id);
void UART_FlushInput(int uart_id);
void UART_FlushOutput(int uart_id);

struct dma_dev *UART_DMADevice(int uart_id);
#endif
