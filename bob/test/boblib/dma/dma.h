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
 
#ifndef __DMA_H__
#define __DMA_H__

#include <bob.h>

#define DMA_BUFFER_SIZE 512

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int   uint32;

struct dma_dev {
	volatile AT91PS_PDC base;
	volatile uint8 rx_buf[2][DMA_BUFFER_SIZE];
	volatile uint8 tx_buf[2][DMA_BUFFER_SIZE];
	
	volatile int rx_pos;     // Our current receive buffer position
	volatile int rx_current; // Our current receive buffer (0 or 1)
	
	volatile int tx_pos;       // Our current transmit buffer position
	volatile int tx_current;   // Our current transmit buffer
	volatile int tx_switching; // Whether or not we're in the middle of switching buffers
};

void DMA_Init(struct dma_dev *channel, AT91PS_PDC base);
uint16  DMA_Read(struct dma_dev *channel, uint8 *data, uint16 count);
uint16  DMA_Write(struct dma_dev *channel, uint8 *data, uint16 count);
void DMA_Flush(struct dma_dev *channel);
void DMA_FlushInput(struct dma_dev *channel);
void DMA_FlushOutput(struct dma_dev *channel);

void DMA_WriteBlock(struct dma_dev *channel, uint8 *data, int count);

void DMA_Disable(struct dma_dev *channel);
void DMA_Enable(struct dma_dev *channel);

int DMA_TransferCount();


#endif
