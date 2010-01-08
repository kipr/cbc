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

#include <usb/device/cdc-serial/CDCDSerialDriver.h>
#include <usb/device/cdc-serial/CDCDSerialDriverDescriptors.h>

#define USB_BUFFER_SIZE \
        BOARD_USB_ENDPOINTS_MAXPACKETSIZE(CDCDSerialDriverDescriptors_DATAIN)

char g_Uart0Buffer[UART_BUFFER_SIZE];
char g_Uart1Buffer[UART_BUFFER_SIZE];

char g_Uart1WriteBuffer[3][UART_BUFFER_SIZE];
int g_Uart1WriteBufferIndex = 0;

char g_UartUsbReadBuffer[USB_BUFFER_SIZE];
char g_UartUsbWriteBuffer[USB_BUFFER_SIZE];
int  g_UartUsbWriteBufferBusy = 0;

volatile int g_Uart0WriteIndex = 0;
volatile int g_Uart0ReadIndex = 0;

volatile int g_Uart1ReadIndex = 0;
volatile int g_Uart1WriteIndex = 0;

volatile int g_UartUsbLock = 0;

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

int UartInit()
{
	CDCDSerialDriver_Initialize();
  USBD_Connect();
	
	UartStartRead();
	
	PMC_EnablePeripheral(AT91C_ID_US0);
	PIO_Configure(g_UartPins, PIO_LISTSIZE(g_UartPins));
	USART_Configure(AT91C_BASE_US0, AT91C_US_CHRL_8_BITS, 57600, MCK);
	AT91C_BASE_PDC_US0->PDC_PTCR = AT91C_PDC_RXTEN;
	USART_SetReceiverEnabled(AT91C_BASE_US0, 1);
	USART_SetTransmitterEnabled(AT91C_BASE_US0, 1);
	
	AT91C_BASE_PDC_US0->PDC_RPR  = (unsigned int)g_Uart1Buffer;
	AT91C_BASE_PDC_US0->PDC_RNPR = (unsigned int)g_Uart1Buffer;
	AT91C_BASE_PDC_US0->PDC_RCR  = UART_BUFFER_SIZE;
	AT91C_BASE_PDC_US0->PDC_RNCR = UART_BUFFER_SIZE;
	
	SetPitCallback(UartRefresh, 2);
	
	return 1;
}

void UartStartRead()
{
	CDCDSerialDriver_Read(g_UartUsbReadBuffer, USB_BUFFER_SIZE, UartUsbRead, 0);
}

static void UartRefresh()
{
	g_Uart1WriteIndex = UART_BUFFER_SIZE - AT91C_BASE_PDC_US0->PDC_RCR;
	
	if(!AT91C_BASE_PDC_US0->PDC_RNCR) {
		AT91C_BASE_PDC_US0->PDC_RNPR = (unsigned int)g_Uart1Buffer;
		AT91C_BASE_PDC_US0->PDC_RNCR = UART_BUFFER_SIZE;
	}
	
	if(g_Uart1WriteIndex != g_Uart1ReadIndex) {
		ChumbyBend(1);
	}
}

int UartWrite(int uart, char *data, int len)
{
	int i = 0;
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
		while(i < len) {
			//printf("Writing %d\r\n", data[i]);
			//USART_Write(AT91C_BASE_US0, data[i++], 0);
		}
		/*if(AT91C_BASE_PDC_US0->PDC_TCR != 0) {
			if(AT91C_BASE_PDC_US0->PDC_TCR != 0)
				return 0;
			else
				g_Uart1WriteBufferState ^= 1;
		}
		
		memcpy(g_Uart1WriteBuffer[g_Uart1WriteBufferState], data, len);
		AT91C_BASE_PDC_US0->PDC_TNPR = (unsigned int)g_Uart1WriteBuffer[g_Uart1WriteBufferState];
		AT91C_BASE_PDC_US0->PDC_TNCR = len;
		
		printf("Wrote %d bytes\r\n", len);*/
		return len;
	}
}

#define BUFF_COUNT(readIndex,writeIndex,size) \
				    (readIndex <= writeIndex ? writeIndex-readIndex : size-readIndex+writeIndex+1)

int UartRead(int uart, char *data, int len)
{
	if(g_Uart0ReadIndex == g_Uart0WriteIndex && g_Uart1ReadIndex == g_Uart1WriteIndex) {
			ChumbyBend(0);
			return 0;
	}
	
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
	else if(uart == 1) {
		if(g_Uart1ReadIndex == g_Uart1WriteIndex) {
			return 0;
		}
		
		if(g_Uart1WriteIndex < g_Uart1ReadIndex) {
			if(UART_BUFFER_SIZE - g_Uart1ReadIndex < len)
				len = UART_BUFFER_SIZE - g_Uart1ReadIndex;
		}
		else {
			if(g_Uart1WriteIndex - g_Uart1ReadIndex < len)
				len = g_Uart1WriteIndex - g_Uart1ReadIndex;
		}
		
		memcpy(data, &(g_Uart1Buffer[g_Uart1ReadIndex]), len);
		g_Uart1ReadIndex += len;
		if(g_Uart1ReadIndex >= UART_BUFFER_SIZE) g_Uart1ReadIndex = 0;
		
		printf("Read %d bytes\r\n", len);
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
	
	ChumbyBend(1);

	CDCDSerialDriver_Read(g_UartUsbReadBuffer, USB_BUFFER_SIZE, UartUsbRead, 0);
}

static void UartUsbWrite(void *pArg,
												 unsigned char status,
												 unsigned int transferred,
												 unsigned int remaining)
{
	g_UartUsbWriteBufferBusy = 0;
}


