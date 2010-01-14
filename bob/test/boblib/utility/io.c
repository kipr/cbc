#include <stdio.h>
#include <usb/usb.h>
#include <string.h>
#include <usart/usart.h>


signed int fputc(signed int c, FILE *pStream)
{
    if ((pStream == stdout) || (pStream == stderr)) {
		USART_Write(AT91C_BASE_US0, c, 0);
        return c;
    }
    else {

        return EOF;
    }
}

signed int fputs(const char *pStr, FILE *pStream)
{
	char *str = (char*)pStr;
    if(pStr) {
		while(*str) fputc(*str++, pStream);
      return strlen(pStr);
    }
    return 0;
}

#undef putchar

signed int putchar(signed int c)
{
  return fputc(c, stdout);
}

