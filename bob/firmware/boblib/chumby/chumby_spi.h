#ifndef __CHUMBY_SPI_H__
#define __CHUMBY_SPI_H__

#include <at91sam7a3/AT91SAM7A3.h>
#include <bob.h>
#include <aic/aic.h>
#include <pmc/pmc.h>

#define CHUMBY_SPI AT91C_BASE_SPI1
#define CHUMBY_DMA AT91C_BASE_PDC_SPI1

typedef void (*ChumbySpiCallback)();

void ChumbySpiInit();
void ChumbySpiReset();

void ChumbySpiWrite(void *data, int len);
void ChumbySpiRead (void *data, int len);

void ChumbySpiSetCallback(ChumbySpiCallback c);

#endif
