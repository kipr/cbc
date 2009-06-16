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

#include "compat.h"
#include <stdio.h>

#ifndef WIN32
#include <sys/time.h>
#endif

#include <time.h>

#define TV_TO_FLOAT(x) ( (double)((x).tv_sec) + ((double)((x).tv_usec))/1000000.0 )

volatile int g_compat_seconds_initted = 0;
volatile struct timeval g_compat_seconds_start_time;

void fsleep(float ftime)
{
	long mtime = (long)(ftime * 1000.0);
	msleep(mtime);
}

void msleep(long mtime)
{
	struct timespec time;

	time.tv_sec = (mtime/1000);
	time.tv_nsec = (mtime%1000)*1000000;
	
	nanosleep(&time, 0);

}


float seconds()
{
	struct timeval now;
	
	if(!g_compat_seconds_initted) {
		gettimeofday(&g_compat_seconds_start_time,NULL);
		g_compat_seconds_initted = 1;
		return 0.0;
	}
	
	gettimeofday(&now, NULL);
	
	if(now.tv_usec < g_compat_seconds_start_time.tv_usec)
		return ((float)(now.tv_sec - g_compat_seconds_start_time.tv_sec) - ((float)(g_compat_seconds_start_time.tv_usec - now.tv_usec)/1000000.0));
	return ((float)(now.tv_sec - g_compat_seconds_start_time.tv_sec) + ((float)(now.tv_usec - g_compat_seconds_start_time.tv_usec)/1000000.0));
}


