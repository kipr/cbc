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

#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#include "font.h"
#include "fb.h"

int fb_x=0, fb_y=0;
int width=320;
int height=240;
int fb_initted=0;
int fb_fd;

#define PIXEL_SIZE 2

unsigned short *fb;
unsigned short *fb_shared;

void fb_init(int blanklines)
{
  if (!fb_initted) {
    fb_initted=1;
    fb_fd = open("/dev/fb", O_RDWR);
    if (fb_fd < 0) {
      perror("open /dev/fb");
      return;
    }
    fb_shared=(unsigned short*)mmap(0, width*height*PIXEL_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fb_fd, 0);
    if (fb_shared==(unsigned short*)-1) {
      perror("mmap");
      fb=NULL;
      return;
    }
    fb = fb_shared+blanklines*width;
    height -= blanklines;
    //printf("framebuffer at %x\n", (int)fb);
  }
}
    
void fb_cls()
{
  fb_init();
  memset(fb, 0xff, width*height*PIXEL_SIZE);
  fb_x=0;
  fb_y=0;
}

void fb_draw_char(int x, int y, int c)
{
  int row;
  for (row = 0; row < FONT_HEIGHT; row++) {
    unsigned char scanline = font[c-32][row];
    unsigned short *ptr = fb+(x+(y+row)*width);
    int col;
    for (col = 0; col < FONT_WIDTH; col++) {
      *ptr++ = ~-(scanline&1);
      scanline >>= 1;
    }
  }
}

void fb_scroll()
{
  memmove(fb, fb+width*FONT_HEIGHT,
          width*(height-FONT_HEIGHT)*PIXEL_SIZE);
  memset(fb+width*(height-FONT_HEIGHT), 0xff,
         width*FONT_HEIGHT*PIXEL_SIZE);
}

void fb_newline()
{
  fb_x=0;
  fb_y++;
  if (fb_y >= height/FONT_HEIGHT) {
    fb_scroll();
    fb_y--;
  }
}
    
void fb_putc(int c)
{
  fb_init();
  if (c == '\n') fb_newline();
  else if (c == '\f') fb_cls();
  else if (c >= ' ') {
    fb_draw_char(fb_x*FONT_WIDTH+1, fb_y*FONT_HEIGHT, c);
    fb_x++;
    if (fb_x+1 > width/FONT_WIDTH) fb_newline();
  }
}

void fb_puts(const char *buf)
{
  fb_init();
  if (!fb) return;
  while (*buf) fb_putc(*buf++);
}

void fb_getpos(int *x, int *y)
{
  *x = fb_x;
  *y = fb_y;
}

void fb_setpos(int x, int y)
{
  fb_x = x;
  fb_y = y;
  if (fb_x < 0 || fb_x+1 >= width/FONT_WIDTH) fb_x = 0;
  if (fb_y < 0 || fb_y >= height/FONT_HEIGHT) fb_y = 0;
}
