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

#include "process.h"

volatile struct __ptable_entry __process_table[MAX_PROCESSES];
pthread_mutex_t __process_mutex = PTHREAD_MUTEX_INITIALIZER;
volatile int __ptable_initted = 0;

void __init_ptable()
{
	int i;

	for(i = 0;i < MAX_PROCESSES;i++)
		__process_table[i].in_use = 0;

	__ptable_initted = 1;
}

int start_process(void (*func)())
{
	int i = -1;

	pthread_mutex_lock(&__process_mutex);

	if(!__ptable_initted)
		__init_ptable();

	while(++i < MAX_PROCESSES && __process_table[i].in_use);
	
	if(i == MAX_PROCESSES)
		return -1;

	__process_table[i].in_use = 1;

	if(pthread_create((pthread_t *)&(__process_table[i].thread), NULL, __run_process, (void *)func)) {
		__process_table[i].in_use = 0;
		return -1;
	}

	pthread_mutex_unlock(&__process_mutex);

	return i;
}

void kill_process(int pid)
{
	pthread_mutex_lock(&__process_mutex);

	if(__process_table[pid].in_use) {
		pthread_cancel(__process_table[pid].thread);
		__process_table[pid].in_use = 0;
	}

	pthread_mutex_unlock(&__process_mutex);
}

int is_process_running(int pid)
{
	pthread_mutex_lock(&__process_mutex);
	
	if(pthread_kill(__process_table[pid].thread, 0)) {
		pthread_mutex_unlock(&__process_mutex);
		return 0;
	}
	
	pthread_mutex_unlock(&__process_mutex);
	return 1;
}


void *__run_process(void *ptr)
{
	void (*func)() = ptr;
	func();
	return 0;
}
