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

#ifndef INCLUDE_TrackingResults_h
#define INCLUDE_TrackingResults_h

typedef struct BlobResultsStr {
  // area, in pixels
  int area;
  // centroid location
  float x;
  float y;
  // confidence, 0-100
  float confidence;
  int bbox_left, bbox_top, bbox_right, bbox_bottom;
  
  // angle in radians of the major axis of the blob.
  // Zero is horizontal and when the left end is higher than the right end the angle will be positive.
  // The range is -PI/2 to +PI/2.
  float angle;
  // sizes, in pixels, of major and minor axes
  float major_axis;
  float minor_axis;
} BlobResults;

#define CHANNEL_MAX_BLOBS 10
typedef struct ChannelResultsStr {
  int n_blobs;
  int hsv_model[4];
  int new_model;
  BlobResults blobs[CHANNEL_MAX_BLOBS];
} ChannelResults;

#define TRACKING_MAX_CHANNELS 4
typedef struct TrackingResultsStr {
  int frame_number;
  int frame_time;
  int previous_frame_time;
  int n_channels;
  ChannelResults channels[TRACKING_MAX_CHANNELS];
} TrackingResults;

#endif
