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

#include "cbcserial.h"

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/types.h>
#include <stdio.h>

int g_cbc_serial_tx_fd = -1;
int g_cbc_serial_rx_fd = -1;

void serial_init()
{
  g_cbc_serial_tx_fd = open(CREATE_UART_TX, O_WRONLY | O_NONBLOCK);
  g_cbc_serial_rx_fd = open(CREATE_UART_RX, O_RDONLY | O_NONBLOCK);
  
  atexit(serial_quit);
}

void serial_quit()
{
  if(g_cbc_serial_tx_fd > 0) {
    close(g_cbc_serial_tx_fd);
    g_cbc_serial_tx_fd = -1;
  }
  if(g_cbc_serial_rx_fd > 0) {
    close(g_cbc_serial_rx_fd);
    g_cbc_serial_rx_fd = -1;
  }
}

int serial_read(char *data, int count)
{
  int ret;
  if(g_cbc_serial_rx_fd > 0) {
    if((ret = read(g_cbc_serial_rx_fd, data, count)) > 0)
      return ret;
  }
  return 0;
}

int serial_write(char *data, int count)
{
  int ret;
  if(g_cbc_serial_tx_fd > 0) {
    if((ret = write(g_cbc_serial_tx_fd, data, count)) > 0)
      return ret;
  }
  return 0;
}


