#include "bootloader.h"

#include <bob.h>
#include <usb/usb.h>
#include <utility/crc32.h>
#include <memories/flash/flashd.h>
#include <efc/efc.h>
#include <string.h>
#include <dbgu/dbgu.h>
#include <utility/trace.h>
#include <pio/pio_it.h>

volatile int bootloaderBufferIndex = 0;
volatile unsigned int bootloaderFlashIndex = 0;
volatile int bootloaderIsFlashing = 1;
unsigned char bootloaderBuffer[BOOT_BUFFER_SIZE];

int BootloaderWriteFlash();
void BootloaderCopyFlash(const unsigned int page_count) RAMFUNC;

static void BootloaderDataCallback(char *data, int len)
{
  if(bootloaderBufferIndex == 0) {
    if(data[0] == 'B' && data[1] == 'L') {
      UsbWrite("LD", 2);
      return;
    }
    else if(data[0] == 'O' && data[1] == 'K') {
      UsbWrite("DC", 2);
      bootloaderIsFlashing = 0;
      return;
    }
    else if(data[0] == 'W' && data[1] == 'R') {
      // Just a placeholder, skips down to the rest of this function
    }
    else {
      UsbWrite("BC", 2);
      return;
    }
  }
  memcpy(&(bootloaderBuffer[bootloaderBufferIndex]), data, len);
  bootloaderBufferIndex += len;
  
  if(bootloaderBufferIndex >= BOOT_BUFFER_SIZE) {
    if(BootloaderWriteFlash()) {
      UsbWrite("OK", 2);
    }
    else UsbWrite("ER", 2);
  }
}

int BootloaderWriteFlash()
{
  unsigned long crc;
  unsigned long computed_crc;
  unsigned int address;
  
  memcpy(&crc, &(bootloaderBuffer[BOOT_BUFFER_SIZE-4]), 4);
  
  computed_crc = crc32(bootloaderBuffer, BOOT_BUFFER_SIZE-4);
  if(crc != crc32(bootloaderBuffer, BOOT_BUFFER_SIZE-4))
    return 0;

  address = AT91C_IFLASH + ((512+bootloaderFlashIndex)*AT91C_IFLASH_PAGE_SIZE);
  FLASHD_Write(address, bootloaderBuffer+2, AT91C_IFLASH_PAGE_SIZE);
  bootloaderFlashIndex += 1;
  bootloaderBufferIndex = 0;
  return 1;
}

void BootloaderCopyFlash(const unsigned int page_count)
{
  unsigned int *dest,*src;
  unsigned int page, i, status;
  
  dest = (unsigned int*)(0x100000);
  src = (unsigned int*)(0x120000);
  
  AT91C_BASE_EFC->EFC_FMR = (AT91C_BASE_EFC->EFC_FMR & ~AT91C_MC_FMCN) | (72 << 16);
  
  for(page = 0;page < page_count;page++) {
    for(i = 0;i < 64;i++)
      *dest++ = *src++;
    AT91C_BASE_EFC->EFC_FMR &= ~AT91C_MC_NEBP;
    AT91C_BASE_EFC->EFC_FMR &= ~AT91C_MC_EOP;
    AT91C_BASE_EFC->EFC_FCR = (0x5A << 24) | (page << 8) | AT91C_MC_FCMD_START_PROG;
    do{status=AT91C_BASE_EFC->EFC_FSR;}while((status&AT91C_MC_FRDY)==0);
  }
  // Jump to the new firmware
	//while(1) nop();
  ((void(*)())0x100000)(); 
}

void Bootloader()
{
	TRACE_CONFIGURE(DBGU_STANDARD, 115200, MCK);
	PIO_InitializeInterrupts(0);
	UsbInit();
  FLASHD_Initialize(MCK);
  UsbSetCallback(BootloaderDataCallback);
  UsbWaitConnect();
  while(bootloaderIsFlashing) nop();
  UsbDisconnect();
  BootloaderCopyFlash(bootloaderFlashIndex);
}
