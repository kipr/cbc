#include "chumby_spi.h"

volatile ChumbySpiCallback g_ChumbySpiCallback = 0;

static void ChumbySpiISR();

void ChumbySpiInit()
{
  PMC_EnablePeripheral(AT91C_ID_SPI1);
  ChumbySpiReset();
  
  CHUMBY_DMA->PDC_PTCR = AT91C_PDC_TXTEN | AT91C_PDC_RXTEN;
  
  AIC_ConfigureIT(AT91C_ID_SPI1, AT91C_AIC_PRIOR_LOWEST, ChumbySpiISR);
  AIC_EnableIT(AT91C_ID_SPI1);
}

void ChumbySpiReset()
{
  CHUMBY_SPI->SPI_CR = AT91C_SPI_SWRST;
  CHUMBY_SPI->SPI_CR = AT91C_SPI_SPIDIS;
  CHUMBY_SPI->SPI_MR = 0;
  CHUMBY_SPI->SPI_CSR[0] = AT91C_SPI_CPOL | AT91C_SPI_BITS_16;
  CHUMBY_SPI->SPI_CR = AT91C_SPI_SPIEN;
  CHUMBY_SPI->SPI_IDR = AT91C_SPI_ENDRX | AT91C_SPI_ENDTX;
}

void ChumbySpiWrite(void *data, int count)
{
  CHUMBY_SPI->SPI_IDR = AT91C_SPI_ENDRX | AT91C_SPI_ENDTX;
  
  CHUMBY_DMA->PDC_TPR = (unsigned int)data;
  CHUMBY_DMA->PDC_TCR = (unsigned int)count;
  
  CHUMBY_SPI->SPI_IER = AT91C_SPI_ENDTX;
}

void ChumbySpiRead (void *data, int count)
{
  CHUMBY_SPI->SPI_IDR = AT91C_SPI_ENDRX | AT91C_SPI_ENDTX;
  
  CHUMBY_DMA->PDC_RPR = (unsigned int)data;
  CHUMBY_DMA->PDC_RCR = (unsigned int)count;
  
  CHUMBY_SPI->SPI_IER = AT91C_SPI_ENDRX;
}

void ChumbySpiSetCallback(ChumbySpiCallback c)
{
  g_ChumbySpiCallback = c;
}

static void ChumbySpiISR()
{
  if(g_ChumbySpiCallback)
    ((ChumbySpiCallback)g_ChumbySpiCallback)();
}
