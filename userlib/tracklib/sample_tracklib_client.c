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
#include <unistd.h>  // for sleep
#include "track.h"

void print_blob(int channel, int blob)
{
  printf("  Blob(channel=%d, index=%d): size=%d, x=%d, y=%d\n",
         channel, blob, track_size(channel, blob),
         (int)track_x(channel, blob), (int)track_y(channel, blob));
  printf("    confidence=%d, bbox=(%d,%d)-(%d,%d)\n",
         (int)track_confidence(channel, blob),
         track_bbox_left(channel, blob), track_bbox_top(channel, blob),
         track_bbox_right(channel, blob), track_bbox_bottom(channel, blob));
  printf("    angle=%g, major_axis=%d, minor_axis=%d\n",
         track_angle(channel, blob),
         (int)track_major_axis(channel, blob), (int)track_minor_axis(channel, blob));
}

void print_channel(int channel, int max_blobs)
{
  int blob;
  printf("Channel %d:\n", channel);
  for (blob = 0; blob < max_blobs && blob < track_count(channel); blob++) {
    print_blob(channel, blob);
  }
}

int main(int argc, char **argv)
{
  while (1) {
    int channel;
    printf("is_new_data_available()=%d\n", track_is_new_data_available());
    track_update();
    printf("frame=%d\n", track_get_frame());
    for (channel = 0; channel < 4; channel++) {
      int max_blobs = 3;
      print_channel(channel, max_blobs);
    }
    sleep(1);
  }
}
