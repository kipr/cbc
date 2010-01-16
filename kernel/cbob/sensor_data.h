#ifndef __SENSOR_DATA_H__
#define __SENSOR_DATA_H__

struct sensor_data {
  short digitals;
  short analog[8];
  short accel_x;
  short accel_y;
  short accel_z;
};

#endif
