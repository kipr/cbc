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

// This implements the 2011 version of the Create library.
// This is NOT backwards compatible with 2010 and previous versions.

#ifndef __IROBOT_CREATE_H__
#define __IROBOT_CREATE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "compat.h"
//#include "cbcserial.h"

#define HIGH_BYTE(word) (((word)>>8)&0xFF)
#define LOW_BYTE(word)  ((word)&0xFF)

#define CREATE_WAITFORBUFFER(buffer,count,error) { if(create_read_block(buffer, count) < count) { printf("Create connection failed.");stateOfCreate.createBusy=0; return error; }}

#define CREATE_BUSY while(stateOfCreate.createBusy)msleep(10); serial_flush(); stateOfCreate.createBusy = 1;
#define CREATE_FREE stateOfCreate.createBusy = 0;

#define twopi 6.28318531

// in seconds
#define COMM_TIMEOUT .5

// structures used to track and cache Create data
struct _sensor_packet{
        float lastUpdate; //cpu clock time
        unsigned int data; // 4 bytes or less of data
};

struct _create_state{
        int createConnected;
        int createBusy;
        int normalizedAngle;
        int totalAngle;
        int accumulatedDistance;//sum of the increment distances
        int driveDirect;//1 if active command of each wheel
        int drive;//1 if speed and radius command is active
        int lspeed;//command last sent
        int rspeed;
        int radius;
        int speed;
        int leds[3];//The state of the create LEDs
        struct _sensor_packet bumpsWheelDrops;//packet 7, b4 caster, b3 lw, b2 rw, b1 lb, b0 rb
        struct _sensor_packet wall;//packet 8, 0 no wall, 1 wall
        struct _sensor_packet lcliff;//packet 9 left cliff sensor 0 no cliff, 1 cliff
        struct _sensor_packet lfcliff;//packet 10 left front cliff sensor 0 no cliff, 1 cliff
        struct _sensor_packet rfcliff;//packet 11 right front cliff sensor 0 no cliff, 1 cliff
        struct _sensor_packet rcliff;//packet 12 right cliff sensor 0 no cliff, 1 cliff
        struct _sensor_packet vWall;//packet 13 0 no wall seen, 1 there is a wall
        struct _sensor_packet LSDandWheelOvercurrents ;//packet 14;b4 LW, b3 RW, B2 LD2, b1 LD0, b0 LD1
        // packets 15-16 are unused
        struct _sensor_packet infrared;//packet 17; byte received from IR remote. 255 means no byte seen
        struct _sensor_packet buttons;//packet 18; b2 advance button is pressed, b0 play button
        struct _sensor_packet distance; //packet 19; 2 byte signed number in mm since last request
        struct _sensor_packet angle;//packet 20; in degrees since last request - 2 byte signed num
        struct _sensor_packet chargingState;//packet 21; 0=not charging; 1-reconditioning chargeing; 2 full charging; 3 trickle charging; 4 waiting; 5 charge fault
        struct _sensor_packet voltage;//packet 22; unsigned 2 byte in mV
        struct _sensor_packet current;// packet 23; signed 2 byte in mA
        struct _sensor_packet batteryTemp;//packet 24; temp in degrees Celcius (1 byte signed)
        struct _sensor_packet batteryCharge;//packet 25; 2byte no sign in mAh
        struct _sensor_packet batteryCapacity;//packet 26; estimated charge capacity of battery in mAh
        struct _sensor_packet wallSignal;//packet 27; analog value of wall sensor 0-4095
        struct _sensor_packet lcliffSignal;//packet 28 left cliff sensor 0-4095
        struct _sensor_packet lfcliffSignal;//packet 29 left front cliff sensor 0-4095
        struct _sensor_packet rfcliffSignal;//packet 30 right front cliff sensor 0-4095
        struct _sensor_packet rcliffSignal;//packet 31 right cliff sensor 0-4095
        struct _sensor_packet cargoBayDI;//packet 32; b4 baud rate change pin 15, b3 DI3 pin 6, b2 DI2-pin 18, b1 DI1 pin 5, b0 DI0 pin 17
        struct _sensor_packet cargoBayAI;//packet 33; 0-1024
        struct _sensor_packet chargingSource;//packet 34; b1 home base, b0 internal charger
        struct _sensor_packet OIMode;//packet 35;0=off,1=passive,2=safe,3=full
        struct _sensor_packet songNumber;//packet 36; currently selected song number
        struct _sensor_packet songPlaying;//packet 37;1=song is playing, 0 song not playing
        struct _sensor_packet numStreamPackets;//packet-38, number of streamed packets, 0-43, is returned
        struct _sensor_packet requestedVelocity;//packet 39; -500 to 500
        struct _sensor_packet requestedRadius;//packet 40; -32768 to 32767
        struct _sensor_packet requestedRVelocity;//packet 41; -500 to 500
        struct _sensor_packet requestedLVelocity;//packet 42; left wheel v -500 to 500
} ;

// This global is used by the user to store songs to be played on the Create
// The functions create_load_song and create_play_song are used to get the data
// from this global to the Create.  The user fills the global on their own.
extern int gc_song_array[16][33];

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
int get_create_mode(float lag);
int get_create_lbump(float lag);
int get_create_rbump(float lag);
int get_create_lwdrop(float lag);
int get_create_cwdrop(float lag);
int get_create_rwdrop(float lag);
int get_create_wall(float lag);
int get_create_lcliff(float lag);
int get_create_lfcliff(float lag);
int get_create_rfcliff(float lag);
int get_create_rcliff(float lag);
int get_create_vwall(float lag);
int get_create_overcurrents(float lag);
int get_create_infrared(float lag);
int get_create_advance_button(float lag);
int get_create_play_button(float lag);
int get_create_normalized_angle(float lag);
void set_create_normalized_angle(int angle);
int get_create_total_angle(float lag);
void set_create_total_angle(int angle);
int get_create_distance(float lag);
void set_create_distance(int dist);
int get_create_battery_charging_state(float lag);
int get_create_battery_voltage(float lag);
int get_create_battery_current(float lag);
int get_create_battery_temp(float lag);
int get_create_battery_charge(float lag);
int get_create_battery_capacity(float lag);
int get_create_wall_amt(float lag);
int get_create_lcliff_amt(float lag);
int get_create_lfcliff_amt(float lag);
int get_create_rfcliff_amt(float lag);
int get_create_rcliff_amt(float lag);
int get_create_bay_DI(float lag);
int get_create_bay_AI(float lag);
int get_create_song_number(float lag);
int get_create_song_playing(float lag);
int get_create_number_of_stream_packets(float lag);
int get_create_requested_velocity(float lag);
int get_create_requested_radius(float lag);
int get_create_requested_right_velocity(float lag);
int get_create_requested_left_velocity(float lag);
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

#ifdef __cplusplus
}
#endif

#endif
