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

#ifndef __IROBOT_CREATE_H__
#define __IROBOT_CREATE_H__

#include <stdio.h>
#include "compat.h"
#include "cbcserial.h"

#define HIGH_BYTE(word) (((word)>>8)&0xFF)
#define LOW_BYTE(word)  ((word)&0xFF)

#define CREATE_WAITFORBUFFER(buffer,count,error) { if(create_read_block(buffer, count) < count) { printf("Create connection failed."); return error; }}

#define CREATE_BUSY while(g_create_busy)msleep(10L); g_create_busy = 1;
#define CREATE_FREE g_create_busy = 0;

#define twopi 6.28318531

// in seconds
#define COMM_TIMEOUT .5

extern int g_create_busy;
extern int g_create_connected;

//globals are updated by the functions below to give access to many key Create sensors
extern int gc_lbump, gc_rbump, gc_ldrop, gc_rdrop, gc_fdrop, gc_rcliff, gc_rfcliff, gc_lcliff, gc_lfcliff;
extern int gc_rcliff_amt, gc_rfcliff_amt, gc_lcliff_amt, gc_lfcliff_amt;
extern int gc_distance, gc_angle, gc_total_angle, gc_advance_button, gc_play_button;
extern int gc_wall, gc_wall_amt, gc_wall_hb, gc_IR;
extern int gc_vel, gc_radius, gc_rvel, gc_lvel;
extern int gc_overcurrents;
extern int gc_charge_state, gc_batt_voltage, gc_current_flow, gc_batt_temp, gc_batt_charge, gc_batt_capacity;
extern int gc_digital_in, gc_analog_in, gc_charge_source;
extern int gc_leds[3];
extern int gc_song_array[16][33]; 
extern int gc_mode;

int create_connect();
void create_disconnect() ;
void create_start();
void create_passive();
void create_safe();
void create_full() ;
void create_spot() ;
void create_cover();
void create_demo(int d) ;
void create_cover_dock();
int create_mode();
int create_sensor_update();
int create_wall();
int create_buttons() ;
int create_bumpdrop();
int create_cliffs() ;
int create_angle() ;
int create_distance();
int create_velocity();
int create_read_IR();
int create_overcurrents();
int create_battery_charge();
int create_cargo_bay_inputs();
void create_stop();
void create_drive (int speed, int radius);
void create_drive_straight (int speed);
void create_spin_CW (int speed);
void create_spin_CCW (int speed);
void create_drive_direct(int r_speed, int l_speed);
int create_spin_block(int speed, int angle);
int _create_get_raw_encoders(long *lenc, long *renc);
void create_advance_led(int on) ;
void create_play_led(int on) ;
void create_power_led(int color, int brightness) ;
void create_digital_output(int bits);
void create_pwm_low_side_drivers(int pwm2, int pwm1, int pwm0);
void create_low_side_drivers(int pwm2, int pwm1, int pwm0);
void create_load_song(int num);
void create_play_song(int num);
int create_read_block(char *data, int count);
void create_write_byte(char byte);
void create_clear_serial_buffer();

#endif
