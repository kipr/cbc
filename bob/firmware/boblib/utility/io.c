#include <stdio.h>
#include <usb/usb.h>
#include <string.h>

/*
signed int fputc(signed int c, FILE *pStream)
{
    if ((pStream == stdout) || (pStream == stderr)) {
        UsbWrite((void*)&c, 1);
        return c;
    }
    else {

        return EOF;
    }
}

signed int fputs(const char *pStr, FILE *pStream)
{
    if(pStr) {
      UsbWrite((void*)pStr, strlen(pStr));
      return strlen(pStr);
    }
    return 0;
}

#undef putchar

signed int putchar(signed int c)
{
  return fputc(c, stdout);
}
*/
