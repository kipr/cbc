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
 
#include "util.h"

int strlen(const char *str)
{
	const char *s;
	
	for(s = str;*s;s++);
	
	return s - str;	
}

int strcmp(const char *s1, const char *s2)
{
	int i;
	
	if(strlen(s1) != strlen(s2))
		return 0;
		
	for(i = 0; s1[i];i++) {
		if(s1[i] != s2[i])
			return 0;
	}
	
	return 1;
}

void *memcpy(void *dest, void *src, size_t count)
{
	uint8 *pdest = (uint8 *)dest;
	uint8 *psrc = (uint8 *)src;
	
	while(count--)
		*pdest++ = *psrc++;
		
	return dest;
}

void *memset(void *dest, int c, size_t n)
{
	uint8 *pdest = (uint8 *)dest;
	int count = 0;
	
	while(count++ < n)
		pdest[count] = c;
	
	return dest;
}
