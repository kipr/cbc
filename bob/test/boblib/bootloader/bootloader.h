#ifndef __BOOTLOADER_H__
#define __BOOTLOADER_H__

#define PAGE_LEN 256
#define CMD_LEN 2
#define CRC_LEN 4

#define BOOT_BUFFER_SIZE (CMD_LEN + PAGE_LEN + CRC_LEN)

void Bootloader();

#endif
