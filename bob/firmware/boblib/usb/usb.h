#ifndef __USB_H__
#define __USB_H__
        
typedef void (*DataCallback)(char *data, int len);

void UsbInit();
void UsbSetCallback(DataCallback c);
unsigned char UsbWrite(char *data, int len);
void UsbRead();
void UsbWaitConnect();
void UsbDisconnect();

#endif
