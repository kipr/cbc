#include "usb.h"

#include <bob.h>
#include <board.h>
#include <usb/device/cdc-serial/CDCDSerialDriver.h>
#include <usb/device/cdc-serial/CDCDSerialDriverDescriptors.h>

#define USB_BUFFER_SIZE \
        BOARD_USB_ENDPOINTS_MAXPACKETSIZE(CDCDSerialDriverDescriptors_DATAIN)

volatile static DataCallback g_UsbDataCallback = 0;
static char g_UsbBuffer[USB_BUFFER_SIZE];

static void UsbDataCallback(unsigned int unused,
                             unsigned char status,
                             unsigned int received,
                             unsigned int remaining);

void UsbInit()
{
  CDCDSerialDriver_Initialize();
  USBD_Connect();
  UsbRead();
}

void UsbSetCallback(DataCallback c)
{
  g_UsbDataCallback = c;
}

unsigned char UsbWrite(char *data, int len)
{
  if(USBD_GetState() == USBD_STATE_CONFIGURED)
    return CDCDSerialDriver_Write(data, len, 0, 0);
  else
    return 0;
}

void UsbRead()
{
  CDCDSerialDriver_Read(g_UsbBuffer, USB_BUFFER_SIZE, (TransferCallback)UsbDataCallback, 0);
}

void UsbWaitConnect()
{
  while(USBD_GetState() != USBD_STATE_CONFIGURED) nop();
  UsbRead();
}

void UsbDisconnect()
{
  USBD_Disconnect();
}

static void UsbDataCallback(unsigned int unused,
                             unsigned char status,
                             unsigned int received,
                             unsigned int remaining)
{
  if(g_UsbDataCallback) {
    ((DataCallback)g_UsbDataCallback)(g_UsbBuffer, received);
  }
  UsbRead();
}

void USBDCallbacks_Suspended(void)
{
}

void USBDCallbacks_Resumed(void)
{
}
