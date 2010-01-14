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
 
#include "dma.h"
#include <bob.h>
#include <string.h>

void DMA_Init(struct dma_dev *channel, AT91PS_PDC base)
{
	channel->base = base;
	
	base->PDC_PTCR = AT91C_PDC_TXTEN  | AT91C_PDC_RXTEN;
	
	DMA_Flush(channel);
}

void DMA_Disable(struct dma_dev *channel)
{
	DMA_Flush(channel);
	channel->base->PDC_PTCR = AT91C_PDC_TXTDIS | AT91C_PDC_RXTDIS;
}

void DMA_Enable(struct dma_dev *channel)
{
	DMA_Flush(channel);
	channel->base->PDC_PTCR = AT91C_PDC_TXTEN | AT91C_PDC_RXTEN;
}

void DMA_Flush(struct dma_dev *channel)
{
	DMA_FlushInput(channel);
	DMA_FlushOutput(channel);
}

void DMA_FlushInput(struct dma_dev *channel)
{
	channel->base->PDC_RNCR = 0;
	channel->base->PDC_RCR = 0;
	
	channel->rx_pos = 0;
	channel->rx_current = 0;
	
	channel->base->PDC_RPR = (uint32)channel->rx_buf[0];
	channel->base->PDC_RNPR = (uint32)channel->rx_buf[1];
	
	channel->base->PDC_RCR = DMA_BUFFER_SIZE;
	channel->base->PDC_RNCR = DMA_BUFFER_SIZE;
	
	memset((void*)channel->rx_buf[0], 0, DMA_BUFFER_SIZE);
	memset((void*)channel->rx_buf[1], 0, DMA_BUFFER_SIZE);
}

void DMA_FlushOutput(struct dma_dev *channel)
{
	channel->base->PDC_TCR = 0;
	channel->base->PDC_TNCR = 0;
	
	channel->tx_pos = 0;
	channel->tx_current = 0;
	channel->tx_switching = 0;
	
	channel->base->PDC_TPR = (unsigned int)channel->tx_buf[0];
	channel->base->PDC_TNPR = (unsigned int)channel->tx_buf[1];
	
	memset((void*)channel->tx_buf[0], 0, DMA_BUFFER_SIZE);
	memset((void*)channel->tx_buf[1], 0, DMA_BUFFER_SIZE);
}

uint16 DMA_Read(struct dma_dev *channel, uint8 *data, uint16 count)
{
	uint16 pdc_count, buffer_left, read_count;
	
	if(!count)
		return 0;
	
	pdc_count = DMA_BUFFER_SIZE - channel->base->PDC_RCR;
	
	if(pdc_count < channel->rx_pos) {
		//overflow state
		buffer_left = DMA_BUFFER_SIZE - channel->rx_pos;
		if(buffer_left <= count) {
			memcpy(data, (void*)(channel->rx_buf[channel->rx_current] + channel->rx_pos), buffer_left);
			
			channel->rx_pos = 0;
			channel->base->PDC_RNPR = (uint32)channel->rx_buf[channel->rx_current];
			channel->base->PDC_RNCR = DMA_BUFFER_SIZE;
			channel->rx_current ^= 1;
			
			return buffer_left;
		}
	}
	
	read_count = pdc_count - channel->rx_pos;
	
	if(count < read_count)
		read_count = count;
	
	memcpy(data, (void*)(channel->rx_buf[channel->rx_current] + channel->rx_pos), read_count);
	channel->rx_pos += read_count;
	return read_count;
}

uint16 DMA_Write(struct dma_dev *channel, uint8 *data, uint16 count)
{
	
	uint16 write_count, buffer_left, data_left;
	
	buffer_left = DMA_BUFFER_SIZE - channel->tx_pos;
	
	if(channel->tx_switching) {
		if(buffer_left < count)
			write_count = buffer_left;
		else
			write_count = count;
		
		memcpy((void*)&(channel->tx_buf[channel->tx_current][channel->tx_pos]), data, write_count);
		channel->tx_pos += write_count;
		
		if(channel->base->PDC_TNCR)
			channel->base->PDC_TNCR += write_count;
		else {
			channel->base->PDC_TCR += write_count;
			channel->tx_switching = 0;
			
			channel->base->PDC_TNPR = (uint32)(channel->tx_buf[channel->tx_current ^ 1]);
		}
		return write_count;
	}
	
	if(buffer_left < count) {
		memcpy((void*)&(channel->tx_buf[channel->tx_current][channel->tx_pos]), data, buffer_left);
		
		channel->base->PDC_TCR += buffer_left;
		channel->tx_pos = 0;
		channel->tx_current ^= 1;
		
		data_left = count - buffer_left;
		
		if(data_left > DMA_BUFFER_SIZE)
			data_left = DMA_BUFFER_SIZE;
		
		memcpy((void*)(channel->tx_buf[channel->tx_current]), data + buffer_left, data_left);
		channel->base->PDC_TNCR += data_left;
		channel->tx_pos += data_left;
		channel->tx_switching = 1;
		
		return buffer_left + data_left;
	}
	memcpy((void*)&(channel->tx_buf[channel->tx_current][channel->tx_pos]), data, count);
	
	channel->base->PDC_TCR += count;
	channel->tx_pos += count;
	
	return count;
}

void DMA_WriteBlock(struct dma_dev *channel, uint8 *data, int count)
{
	if(channel->base->PDC_TCR == 0) {
		memcpy((void*)(channel->tx_buf[channel->tx_current]), data, count);
		channel->base->PDC_TPR = (uint32)(channel->tx_buf[channel->tx_current]);
		channel->base->PDC_TCR = count;
		channel->tx_current ^= 1;
	}
	
	while(channel->base->PDC_TNCR) nop();
	
	memcpy((void*)(channel->tx_buf[channel->tx_current]), data, count);
	channel->base->PDC_TNPR = (uint32)channel->tx_buf[channel->tx_current];
	channel->base->PDC_TNCR = count;
	channel->tx_current ^= 1;
}

int DMA_TransferCount(struct dma_dev *dev)
{
	return dev->base->PDC_TCR;
}

