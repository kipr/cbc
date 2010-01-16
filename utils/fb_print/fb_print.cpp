/**************************************************************************
 *  Copyright 2008,2009 KISS Institute for Practical Robotics             *
 *                                                                        *
 *  This file is part of CBC Firmware.                                    *
 *                                                                        *
 *  CBC Firmware is free software: you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation, either version 2 of the License, or     *
 *  (at your option) any later version.                                   *
 *                                                                        *
 *  CBC Firmware is distributed in the hope that it will be useful,       *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *  GNU General Public License for more details.                          *
 *                                                                        *
 *  You should have received a copy of the GNU General Public License     *
 *  along with this copy of CBC Firmware.  Check the LICENSE file         *
 *  in the project root.  If not, see <http://www.gnu.org/licenses/>.     *
 **************************************************************************/

#include <stdio.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#include "fb.h"

bool read_coords(int &x, int &y)
{
  FILE *in= fopen("/tmp/fb_coords","r");
  if (!in) return false;
  x=y=0;
  fscanf(in, "%d %d", &x, &y);
  fclose(in);
  return true;
}

void write_coords(int x, int y)
{
  FILE *out = fopen("/tmp/fb_coords","w");
  fprintf(out, "%d %d\n", x, y);
  fclose(out);
}

int main(int argc, char **argv)
{
  int x,y;
  if (!read_coords(x,y)) {
    x=0; y=0; fb_cls();
  }
  int skiptop=0;
  bool echo=false;
  for (int i= 1; i < argc; i++) {
    char *arg = argv[i];
    if (!::strcmp(arg, "--echo")) echo = true;
    else if (!strcmp(arg, "--skiptop")) {
      skiptop = ::atoi(argv[++i]);
    }
  }
  fb_init(skiptop);
  fb_setpos(x,y);
  while (1) {
    int c = getchar();
    if (c == EOF) break;
    fb_putc(c);
    if (echo) {
      putchar(c);
      fflush(stdout);
    }
  }
  fb_getpos(&x,&y);
  write_coords(x,y);
  return 0;
}

