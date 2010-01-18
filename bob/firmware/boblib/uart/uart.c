#include <bob.h>
#include <board.h>
#include <usart/usart.h>
#include <string.h>
#include "uart.h"
#include <chumby/chumby.h>
#include <stdio.h>
#include <timer/timer.h>

#include <pmc/pmc.h>
#include <pio/pio.h>

#include <dma/dma.h>

#include <usb/device/cdc-serial/CDCDSerialDriver.h>
#include <usb/device/cdc-serial/CDCDSerialDriverDescriptors.h>

#define USB_BUFFER_SIZE \
        BOARD_USB_ENDPOINTS_MAXPACKETSIZE(CDCDSerialDriverDescriptors_DATAIN)

// All of these are for the USB stuff
unsigned char g_Uart0Buffer[UART_BUFFER_SIZE];
unsigned char g_UartUsbReadBuffer[USB_BUFFER_SIZE];
unsigned char g_UartUsbWriteBuffer[USB_BUFFER_SIZE];
int  g_UartUsbWriteBufferBusy = 0;
volatile int g_Uart0WriteIndex = 0;
volatile int g_Uart0ReadIndex = 0;

unsigned char g_Uart1Buffer[UART_BUFFER_SIZE];
volatile int g_Uart1BufferIndex = 0;
volatile int g_Uart1BufferReadIndex = 0;

struct dma_dev g_Uart1DMA;

volatile int g_UartUsbLock = 0;
volatile int g_UartSigmask = 2;

static const Pin g_UartPins[] = {SERIAL_TXD,SERIAL_RXD};

static void UartUsbRead(void *pArg, 
												unsigned char status, 
												unsigned int received, 
												unsigned int remaining);

static void UartUsbWrite(void *pArg,
												 unsigned char status,
												 unsigned int transferred,
												 unsigned int remaining);
static void UartRefresh();
static void UartWiggle();

int UartInit()
{
	CDCDSerialDriver_Initialize();
	
	PIO_Configure(g_UartPins, PIO_LISTSIZE(g_UartPins));
	PMC_EnablePeripheral(AT91C_ID_US0);
	
	USART_Configure(AT91C_BASE_US0, AT91C_US_USMODE_NORMAL | AT91C_US_CLKS_CLOCK | 
																	AT91C_US_CHRL_8_BITS | AT91C_US_PAR_NONE | 
																	AT91C_US_NBSTOP_1_BIT, 57600, MCK);
	USART_SetReceiverEnabled(AT91C_BASE_US0, 1);
	USART_SetTransmitterEnabled(AT91C_BASE_US0, 1);
	
	DMA_Init(&g_Uart1DMA, AT91C_BASE_PDC_US0);
	
	SetPitCallback(UartRefresh, 2);
	
	ChumbySS1(0);
	
	return 1;
}

void UartStartRead()
{
	CDCDSerialDriver_Read(g_UartUsbReadBuffer, USB_BUFFER_SIZE, UartUsbRead, 0);
}

static void UartRefresh()
{
	if(g_Uart1BufferIndex + 64 >= UART_BUFFER_SIZE) {
		g_Uart1BufferIndex += DMA_Read(&g_Uart1DMA, &(g_Uart1Buffer[g_Uart1BufferIndex]), UART_BUFFER_SIZE-g_Uart1BufferIndex);
	}
	else g_Uart1BufferIndex += DMA_Read(&g_Uart1DMA, &(g_Uart1Buffer[g_Uart1BufferIndex]), 64);
	
	if(g_Uart1BufferIndex == UART_BUFFER_SIZE) {
		g_Uart1BufferIndex = 0;
	}
	
	UartWiggle();
}

void UartSetSigmask(int mask)
{
	g_UartSigmask = mask;
}

static void UartWiggle()
{
	int data_ready = 0;
	
	if(g_Uart0ReadIndex != g_Uart0WriteIndex)
		data_ready = 1;
		
	if(g_Uart1BufferIndex != g_Uart1BufferReadIndex)
		data_ready = 1;
		
	if(data_ready) {
		ChumbySS1(0);
		ChumbySS1(1);
		ChumbySS1(0);
	}
}

int UartWrite(int uart, char *data, int len)
{
	if(uart == 0) {
		if(g_UartUsbWriteBufferBusy)
			return 0;
		if(len > USB_BUFFER_SIZE)
			len = USB_BUFFER_SIZE;
		memcpy(g_UartUsbWriteBuffer, data, len);
		CDCDSerialDriver_Write(g_UartUsbWriteBuffer, len, UartUsbWrite, 0);
		return len;
	}
	else {
		return DMA_Write(&g_Uart1DMA, (void*)data, len);
	}
}

#define BUFF_COUNT(readIndex,writeIndex,size) \
				    (readIndex <= writeIndex ? writeIndex-readIndex : size-readIndex+writeIndex+1)

int UartRead(int uart, char *data, int len)
{
	if(uart == 0) {
		if(g_Uart0ReadIndex == g_Uart0WriteIndex) {
			return 0;
		}
		
		if(g_Uart0WriteIndex < g_Uart0ReadIndex) {
			if(UART_BUFFER_SIZE - g_Uart0ReadIndex < len)
				len = UART_BUFFER_SIZE - g_Uart0ReadIndex;
		} 
		else {
			if(g_Uart0WriteIndex - g_Uart0ReadIndex < len)
				len = g_Uart0WriteIndex - g_Uart0ReadIndex;
		}
		memcpy(data, &(g_Uart0Buffer[g_Uart0ReadIndex]), len);
		g_Uart0ReadIndex += len;
		if(g_Uart0ReadIndex >= UART_BUFFER_SIZE) g_Uart0ReadIndex = 0;
		return len;
	}
	else if(uart == 1){
		if(g_Uart1BufferReadIndex == g_Uart1BufferIndex)
			return 0;
		if(g_Uart1BufferIndex < g_Uart1BufferReadIndex) {
			if(UART_BUFFER_SIZE - g_Uart1BufferReadIndex < len)
				len = UART_BUFFER_SIZE - g_Uart1BufferReadIndex;
		}
		else {
			if(g_Uart1BufferIndex - g_Uart1BufferReadIndex < len)
				len = g_Uart1BufferIndex - g_Uart1BufferReadIndex;
		}
		
		memcpy(data, &(g_Uart1Buffer[g_Uart1BufferReadIndex]), len);
		g_Uart1BufferReadIndex += len;
		if(g_Uart1BufferReadIndex >= UART_BUFFER_SIZE) g_Uart1BufferReadIndex = 0;
		return len;
	}
	return 0;
}

int UartReset(int uart)
{
	if(uart == 0) {
		g_Uart0WriteIndex = 0;
		g_Uart0ReadIndex = 0;
		return 1;
	}
	else {
		g_Uart1BufferIndex = 0;
		g_Uart1BufferReadIndex = 0;
		return 0;
	}
}

static void UartUsbRead(void *pArg, 
												unsigned char status, 
												unsigned int received, 
												unsigned int remaining)
{
	int copied = 0;
	
	
	if(received + g_Uart0WriteIndex >= UART_BUFFER_SIZE) {
		copied = UART_BUFFER_SIZE - g_Uart0WriteIndex;
		memcpy(&(g_Uart0Buffer[g_Uart0WriteIndex]), g_UartUsbReadBuffer, copied);
		g_Uart0WriteIndex = 0;
	}
	
	memcpy(&(g_Uart0Buffer[g_Uart0WriteIndex]), &(g_UartUsbReadBuffer[copied]), received - copied);
	g_Uart0WriteIndex += received - copied;

	CDCDSerialDriver_Read(g_UartUsbReadBuffer, USB_BUFFER_SIZE, UartUsbRead, 0);
}

static void UartUsbWrite(void *pArg,
												 unsigned char status,
												 unsigned int transferred,
												 unsigned int remaining)
{
	g_UartUsbWriteBufferBusy = 0;
}


