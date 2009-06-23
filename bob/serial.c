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
 
#include "serial.h"
#include "dma.h"
#include "pio.h"
#include "pmc.h"

volatile AT91PS_USART pUSART[2];
volatile struct dma_dev uart_dma[2];

void UART_Init(int uart_id, int baud)
{
	if(uart_id == 0) {
		pUSART[uart_id] = AT91C_BASE_US0;
		PIO_Disable(UART0_RXPIN);			// setup pins to be used for UART peripherals
		PIO_Disable(UART0_TXPIN);
		PIO_DisablePullup(UART0_RXPIN);			// disable pullup resistors
		PIO_DisablePullup(UART0_TXPIN);
		PIO_SetPeripheralA(UART0_RXPIN);
		PIO_SetPeripheralA(UART0_TXPIN);
		PMC_Enable(UART0_PMC_ID);			// Turn on UART clock
		
		// RESET RX and TX then disable RX and TX
		pUSART[uart_id]->US_CR =AT91C_US_RSTRX |
								AT91C_US_RSTTX |
								AT91C_US_RXDIS |
								AT91C_US_TXDIS;

		// settings: Normal mode, clock = MCK, 8N1, baud rate
		UART_SetBaud(uart_id, baud);
		
		// enable RX and TX lines
		pUSART[uart_id]->US_CR = AT91C_US_TXEN | AT91C_US_RXEN;
		
		DMA_Init((struct dma_dev *)&(uart_dma[uart_id]), AT91C_BASE_PDC_US0);
	}
	if(uart_id == 1) {
		pUSART[uart_id] = AT91C_BASE_US1;
		PIO_Disable(UART1_RXPIN);			// setup pins to be used for UART peripherals
		PIO_Disable(UART1_TXPIN);
		PIO_DisablePullup(UART1_RXPIN);			// disable pullup resistors
		PIO_DisablePullup(UART1_TXPIN);
		PIO_SetPeripheralA(UART1_RXPIN);
		PIO_SetPeripheralA(UART1_TXPIN);
		PMC_Enable(UART1_PMC_ID);		// Turn on UART clock
		
		// RESET RX and TX then disable RX and TX
		pUSART[uart_id]->US_CR =AT91C_US_RSTRX |
								AT91C_US_RSTTX |
								AT91C_US_RXDIS |
								AT91C_US_TXDIS;
		
		// settings: Normal mode, clock = MCK, 8N1, baud rate
		UART_SetBaud(uart_id, baud);
		
		// enable RX and TX lines
		pUSART[uart_id]->US_CR = AT91C_US_TXEN | AT91C_US_RXEN;
		
		DMA_Init((struct dma_dev *)&(uart_dma[uart_id]), AT91C_BASE_PDC_US1);
	}
}

void UART_SetBaud(int uart_id, int baud)
{
	pUSART[uart_id]->US_MR =AT91C_US_USMODE_NORMAL |
							AT91C_US_CLKS_CLOCK |
							AT91C_US_CHRL_8_BITS |
							AT91C_US_PAR_NONE |
							AT91C_US_NBSTOP_1_BIT;
	
	pUSART[uart_id]->US_BRGR = MCK/16/baud;
	
	pUSART[uart_id]->US_TTGR = 0;
}

int UART_Write(int uart_id, uint8 *data, int count)
{
	return DMA_Write((struct dma_dev *)&(uart_dma[uart_id]), data, count);
}

int UART_Read(int uart_id, uint8 *data, int count)
{
	return DMA_Read((struct dma_dev *)&(uart_dma[uart_id]), data, count);
}

void UART_WriteByte(int uart_id, char c)
{
	while(!UART_Write(uart_id, &c, 1));
}

uint8 UART_ReadByte(int uart_id)
{
	uint8 c;
	
	while(!UART_Read(uart_id, &c, 1));
	
	return c;
}

void UART_WriteString(int uart_id, char *str)
{
	UART_Write(uart_id, str, strlen((const char *)str));
}

void UART_Flush(int uart_id)
{
	DMA_Flush((struct dma_dev *)&(uart_dma[uart_id]));
}

void UART_FlushInput(int uart_id)
{
	DMA_FlushInput((struct dma_dev *)&(uart_dma[uart_id]));
}

void UART_FlushOutput(int uart_id)
{
	DMA_FlushOutput((struct dma_dev *)&(uart_dma[uart_id]));
}

struct dma_dev *UART_DMADevice(int uart_id)
{
	return (struct dma_dev *)&(uart_dma[uart_id]);
}

void UART_Disable(int uart_id)
{
	DMA_Disable((struct dma_dev *)&uart_dma[uart_id]);
	
	pUSART[uart_id]->US_CR = AT91C_US_TXDIS | AT91C_US_TXDIS;
	
	if(uart_id) PMC_Disable(UART1_PMC_ID);
	else PMC_Disable(UART0_PMC_ID);
}
