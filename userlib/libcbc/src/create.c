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

#include "create.h"

// create the Create's state and initialize all data to 0 and all last updates to a billion seconds in the past.
struct _create_state stateOfCreate={0,0,0,0,0,0,0,0,0,0,0,{0,0,0},{-1.E9,0},{-1.E9,0},{-1.E9,0},{-1.E9,0},{-1.E9,0},{-1.E9,0},{-1.E9,0},{-1.E9,0},{-1.E9,0},{-1.E9,0},{-1.E9,0},{-1.E9,0},{-1.E9,0},{-1.E9,0},{-1.E9,0},{-1.E9,0},{-1.E9,0},{-1.E9,0},{-1.E9,0},{-1.E9,0},{-1.E9,0},{-1.E9,0},{-1.E9,0},{-1.E9,0},{-1.E9,0},{-1.E9,0},{-1.E9,0},{-1.E9,0},{-1.E9,0},{-1.E9,0},{-1.E9,0},{-1.E9,0},{-1.E9,0},{-1.E9,0}};//is the one and only instance of this structure
// This global is used by the user to store songs to be played on the Create
// The functions create_load_song and create_play_song are used to get the data
// from this global to the Create.  The user fills the global on their own.
int gc_song_array[16][33];


// void beep() {} // Does nothing for now

// Create must be turned on and connected.  This routine establishes serial link
// Note that this routine should be called once, and must have been called before
// any of the other create functions will work.
// If Create is connected, power light goes yellow and play led turns on
int create_connect()
{
        serial_init();
        create_start();
        create_safe(); //allows create to be commanded and turns LED orange
        if (get_create_mode(0.0)!=2) {       // if not safe mode, connection not established
                printf("Create connect failed, mode is %d\n", stateOfCreate.OIMode.data);
                stateOfCreate.createConnected = 0;
                beep(); msleep(100); beep();
                return(-1);//connection failed
        }
        create_advance_led(1); // turn on advance LED
        stateOfCreate.createConnected = 1;

        atexit(create_disconnect);

        return(0);//successful connection
}

// returns the serial connections on XBC to normal communications over the USB port.
// Turns of play LED and returns power light to green
void create_disconnect() {
        create_play_led(0);
        create_power_led(0,255);
        create_stop();
        stateOfCreate.createConnected=0;
        serial_quit();
}

// if serial communication times out (ie., is lost), attempts to restart it
void create_reconnect()
{
        printf("Create communication lost;\nattempting reconnection.\n");
        create_disconnect();    // reset environment
        create_connect();

}

// Create open interface mode is off=0 (Create powered), passive=1, safe=2, full=3
// From "safe" mode, wheel drop or cliff detect halts motors and puts Create in passive mode
int get_create_mode(float lag)
{
        char buffer[1];
        char *bptr = buffer;
        if(seconds()-stateOfCreate.OIMode.lastUpdate > lag){
                CREATE_BUSY;
                create_write_byte(142);
                create_write_byte(35);
                CREATE_WAITFORBUFFER(buffer,1,100035)
                stateOfCreate.OIMode.data=buffer[0];//update data
                stateOfCreate.OIMode.lastUpdate=seconds();//update update time
                CREATE_FREE;
        }
        return(stateOfCreate.OIMode.data);
}



// See the create open interface document for more information on these functions
void create_start()
{
        CREATE_BUSY;
        create_write_byte(128);
        CREATE_FREE;
}

void create_passive()
{
        CREATE_BUSY;
        create_write_byte(128);
        CREATE_FREE;
}

void create_safe()
{
        CREATE_BUSY;
        create_write_byte(131);
        CREATE_FREE;
        create_power_led(64,255);
}

void create_full()
{
        CREATE_BUSY;
        create_write_byte(132);
        CREATE_FREE;
        create_power_led(227,255);
}

void create_spot()
{
        CREATE_BUSY;
        create_write_byte(134);
        CREATE_FREE;
}

void create_cover()
{
        CREATE_BUSY;
        create_write_byte(135);
        CREATE_FREE;
}

void create_demo(int d)
{
        CREATE_BUSY;
        create_write_byte(136); create_write_byte(d);
        CREATE_FREE;
}

void create_cover_dock()
{
        CREATE_BUSY;
        create_write_byte(143);
        CREATE_FREE;
}


//////////////Create Sensor Routines///////////////
// These work for any Create mode except off


//serial connection failure codes are COI packet number + 100000

// updates the bumper and wheel drop states with the current values from the Create
int _get_create_bumpdrop(float lag) {
        char buffer[1];
        if(seconds()-stateOfCreate.bumpsWheelDrops.lastUpdate > lag){
                CREATE_BUSY;
                create_write_byte(142);
                create_write_byte(7);//drops and bumps
                CREATE_WAITFORBUFFER(buffer,1,100007)
                stateOfCreate.bumpsWheelDrops.data=buffer[0];
                stateOfCreate.bumpsWheelDrops.lastUpdate=seconds();
                CREATE_FREE;
        }
        return(stateOfCreate.bumpsWheelDrops.data);
}

int get_create_lbump(float lag)
{
        int b = _get_create_bumpdrop(lag);
        if(b>100000)return b;
        return((b >> 1) & 0x1);
}
int get_create_rbump(float lag)
{
        int b = _get_create_bumpdrop(lag);
        if(b>100000)return b;
        return((b >> 0) & 0x1);
}
int get_create_cwheel(float lag)
{
        int b = _get_create_bumpdrop(lag);
        if(b>100000)return b;
        return((b >> 4) & 0x1);
}
int get_create_lwheel(float lag)
{
        int b = _get_create_bumpdrop(lag);
        if(b>100000)return b;
        return((b >> 3) & 0x1);
}
int get_create_rwheel(float lag)
{
        int b = _get_create_bumpdrop(lag);
        if(b>100000)return b;
        return((b >> 2) & 0x1);
}


//updates and returns wall sensor in digital mode
int get_create_wall(float lag) {
        char buffer[1];
        if(seconds()-stateOfCreate.wall.lastUpdate > lag){
                CREATE_BUSY;
                create_write_byte(142);
                create_write_byte(8);  //  wall seen or not (1 byte)
                CREATE_WAITFORBUFFER(buffer,1,100008)
                stateOfCreate.wall.data=buffer[0];
                stateOfCreate.wall.lastUpdate=seconds();
                CREATE_FREE;
        }
        return(stateOfCreate.wall.data);
}


//updates and returns cliff sensor in digital mode
int get_create_lcliff(float lag) {
        char buffer[1];
        if(seconds()-stateOfCreate.lcliff.lastUpdate > lag){
                CREATE_BUSY;
                create_write_byte(142);
                create_write_byte(9);  //  wall seen or not (1 byte)
                CREATE_WAITFORBUFFER(buffer,1,100009)
                stateOfCreate.lcliff.data=buffer[0];
                stateOfCreate.lcliff.lastUpdate=seconds();
                CREATE_FREE;
        }
        return(stateOfCreate.lcliff.data);
}
//updates and returns cliff sensor in digital mode
int get_create_lfcliff(float lag) {
        char buffer[1];
        if(seconds()-stateOfCreate.lfcliff.lastUpdate > lag){
                CREATE_BUSY;
                create_write_byte(142);
                create_write_byte(10);  //  wall seen or not (1 byte)
                CREATE_WAITFORBUFFER(buffer,1,100010)
                stateOfCreate.lfcliff.data=buffer[0];
                stateOfCreate.lfcliff.lastUpdate=seconds();
                CREATE_FREE;
        }
        return(stateOfCreate.lfcliff.data);
}
//updates and returns cliff sensor in digital mode
int get_create_rfcliff(float lag) {
        char buffer[1];
        if(seconds()-stateOfCreate.rfcliff.lastUpdate > lag){
                CREATE_BUSY;
                create_write_byte(142);
                create_write_byte(11);  //  wall seen or not (1 byte)
                CREATE_WAITFORBUFFER(buffer,1,100011)
                stateOfCreate.rfcliff.data=buffer[0];
                stateOfCreate.rfcliff.lastUpdate=seconds();
                CREATE_FREE;
        }
        return(stateOfCreate.rfcliff.data);
}
//updates and returns cliff sensor in digital mode
int get_create_rcliff(float lag) {
        char buffer[1];
        if(seconds()-stateOfCreate.rcliff.lastUpdate > lag){
                CREATE_BUSY;
                create_write_byte(142);
                create_write_byte(12);  //  wall seen or not (1 byte)
                CREATE_WAITFORBUFFER(buffer,1,100012)
                stateOfCreate.rcliff.data=buffer[0];
                stateOfCreate.rcliff.lastUpdate=seconds();
                CREATE_FREE;
        }
        return(stateOfCreate.rcliff.data);
}


//updates and returns virtual wall sensor in digital mode
int get_create_vwall(float lag) {
        char buffer[1];
        if(seconds()-stateOfCreate.vWall.lastUpdate > lag){
                CREATE_BUSY;
                create_write_byte(142);
                create_write_byte(13);  //  wall seen or not (1 byte)
                CREATE_WAITFORBUFFER(buffer,1,100013)
                stateOfCreate.vWall.data=buffer[0];
                stateOfCreate.vWall.lastUpdate=seconds();
                CREATE_FREE;
        }
        return(stateOfCreate.vWall.data);
}

//updates and returns overcurrents b4 lw, b3 rw, b2 ld2, b1, ld0, b0 ld1
int get_create_overcurrents(float lag) {
        char buffer[1];
        if(seconds()-stateOfCreate.LSDandWheelOvercurrents.lastUpdate > lag){
                CREATE_BUSY;
                create_write_byte(142);
                create_write_byte(14);  //  wall seen or not (1 byte)
                CREATE_WAITFORBUFFER(buffer,1,100014)
                stateOfCreate.LSDandWheelOvercurrents.data=buffer[0];
                stateOfCreate.LSDandWheelOvercurrents.lastUpdate=seconds();
                CREATE_FREE;
        }
        return(stateOfCreate.LSDandWheelOvercurrents.data);
}


//updates and returns infrared byte. 255 if no signal
int get_create_infrared(float lag) {
        char buffer[1];
        if(seconds()-stateOfCreate.infrared.lastUpdate > lag){
                CREATE_BUSY;
                create_write_byte(142);
                create_write_byte(17);  //  infrared
                CREATE_WAITFORBUFFER(buffer,1,100017)
                stateOfCreate.infrared.data=buffer[0];
                stateOfCreate.infrared.lastUpdate=seconds();
                CREATE_FREE;
        }
        return(stateOfCreate.infrared.data);
}


// updates the bumper and wheel drop globals with the current values from the Create
int _get_create_buttons(float lag) {
        char buffer[1];
        if(seconds()-stateOfCreate.buttons.lastUpdate > lag){
                CREATE_BUSY;
                create_write_byte(142);
                create_write_byte(18);//buttons
                CREATE_WAITFORBUFFER(buffer,1,100018)
                stateOfCreate.buttons.data=buffer[0];
                stateOfCreate.buttons.lastUpdate=seconds();
                CREATE_FREE;
        }
        return(stateOfCreate.buttons.data);
}

int get_create_advance_button(float lag){int b=_get_create_buttons(lag); if(b>100000) return(b); return((b >>2) & 0x1);}
int get_create_play_button(float lag){int b=_get_create_buttons(lag); if(b>100000) return(b); return(b & 0x1);}


// these functions updates the distance which has the average distance traveled of the two
// wheels (vehicle odometry) in mm.  Turning in place does not change this value.
// Forward increments, backwards decrements
int get_create_incremental_distance(float lag) {
        char buffer[2];
        if(seconds()-stateOfCreate.distance.lastUpdate > lag){
                CREATE_BUSY;
                create_write_byte(142);
                create_write_byte(19);//buttons
                CREATE_WAITFORBUFFER(buffer,2,100019)
                stateOfCreate.distance.data=(256*buffer[0] + buffer[1]);
                if(stateOfCreate.distance.data > 32767){ //if this was meant to be a negative 16 bit int
                        stateOfCreate.distance.data=stateOfCreate.distance.data - 65536;//convert from 16 bit 2's complement to 32bit int
                }//note that if Create moves more than 32m between create_distance call, there will be overflow

                stateOfCreate.distance.lastUpdate=seconds();
                CREATE_FREE;
                stateOfCreate.accumulatedDistance=stateOfCreate.accumulatedDistance+stateOfCreate.distance.data;
        }
        return(stateOfCreate.distance.data);
}

int get_create_distance(float lag) {get_create_incremental_distance(lag); return stateOfCreate.accumulatedDistance;}
void set_create_distance(int d) {get_create_incremental_distance(.005); stateOfCreate.accumulatedDistance=d;}

// these functions updates angle which stores a normalized angle between 0 and 359 degrees
// and the global gc_total_angle which is not normalized and can be larger than 360 and less than 0.
// CCW angles are positive and CW turns decrement the angle value.
int get_create_incremental_angle(float lag) {
        char buffer[2];
        if(seconds()-stateOfCreate.angle.lastUpdate > lag){
                CREATE_BUSY;
                create_write_byte(142);
                create_write_byte(20);//buttons
                CREATE_WAITFORBUFFER(buffer,2,100020)
                stateOfCreate.angle.data=(256*buffer[0] + buffer[1]);
                if(stateOfCreate.angle.data > 32767){ //if this was meant to be a negative 16 bit int
                        stateOfCreate.angle.data=stateOfCreate.angle.data - 65536;//convert from 16 bit 2's complement to 32bit int
                }//note that if Create moves more than 32m between create_distance call, there will be overflow

                stateOfCreate.angle.lastUpdate=seconds();
                CREATE_FREE;
                stateOfCreate.totalAngle=stateOfCreate.totalAngle+stateOfCreate.angle.data;
                stateOfCreate.normalizedAngle=stateOfCreate.normalizedAngle+stateOfCreate.angle.data;
                if(stateOfCreate.normalizedAngle>359)stateOfCreate.normalizedAngle=stateOfCreate.normalizedAngle-360;
                if(stateOfCreate.normalizedAngle<0)stateOfCreate.normalizedAngle=stateOfCreate.normalizedAngle+360;

        }
        return(stateOfCreate.angle.data);
}

int get_create_total_angle(float lag) {get_create_incremental_angle(lag); return stateOfCreate.totalAngle;}
int get_create_normalized_angle(float lag) {
        get_create_incremental_angle(lag);
        stateOfCreate.normalizedAngle=stateOfCreate.normalizedAngle%360;
        if(stateOfCreate.normalizedAngle<0)stateOfCreate.normalizedAngle=(stateOfCreate.normalizedAngle+360);
        return stateOfCreate.normalizedAngle;
}
void set_create_total_angle(int a) {get_create_normalized_angle(0.005); stateOfCreate.totalAngle=a;}
void set_create_normalized_angle(int a) {
        get_create_total_angle(0.005);//update total angle, clears incremental buffer before setting normalized angle
        stateOfCreate.normalizedAngle=a%360;
        if(stateOfCreate.normalizedAngle<0)stateOfCreate.normalizedAngle=a+360;
}


int get_create_battery_charging_state(float lag)
{
        char buffer[1];
        if(seconds()-stateOfCreate.chargingState.lastUpdate > lag){
                CREATE_BUSY;
                create_write_byte(142);
                create_write_byte(21);
                CREATE_WAITFORBUFFER(buffer,1,100021)
                stateOfCreate.chargingState.data=buffer[0];
                stateOfCreate.chargingState.lastUpdate=seconds();
                CREATE_FREE;
        }
        return(stateOfCreate.chargingState.data);
}

int get_create_battery_voltage(float lag)
{
        char buffer[2];
        if(seconds()-stateOfCreate.voltage.lastUpdate > lag){
                CREATE_BUSY;
                create_write_byte(142);
                create_write_byte(22);
                CREATE_WAITFORBUFFER(buffer,2,100022)
                stateOfCreate.voltage.data=buffer[0]*256;
                stateOfCreate.voltage.data=stateOfCreate.voltage.data+buffer[1];
                stateOfCreate.voltage.lastUpdate=seconds();
                CREATE_FREE;
        }
        return(stateOfCreate.voltage.data);
}

int get_create_battery_current(float lag)
{
        char buffer[2];
        if(seconds()-stateOfCreate.current.lastUpdate > lag){
                CREATE_BUSY;
                create_write_byte(142);
                create_write_byte(23);
                CREATE_WAITFORBUFFER(buffer,2,100023)
                stateOfCreate.current.data=buffer[0]*256;
                stateOfCreate.current.data=stateOfCreate.current.data+buffer[1];
                if(stateOfCreate.current.data>32767)stateOfCreate.current.data=stateOfCreate.current.data-65536;
                stateOfCreate.current.lastUpdate=seconds();
                CREATE_FREE;
        }
        return(stateOfCreate.current.data);
}


int get_create_battery_temp(float lag)
{
        char buffer[1];
        if(seconds()-stateOfCreate.batteryTemp.lastUpdate > lag){
                CREATE_BUSY;
                create_write_byte(142);
                create_write_byte(24);
                CREATE_WAITFORBUFFER(buffer,1,100024)
                stateOfCreate.batteryTemp.data=buffer[0];
                if(stateOfCreate.batteryTemp.data>127)stateOfCreate.batteryTemp.data=stateOfCreate.batteryTemp.data-128;
                stateOfCreate.batteryTemp.lastUpdate=seconds();
                CREATE_FREE;
        }
        return(stateOfCreate.batteryTemp.data);
}

int get_create_battery_charge(float lag)
{
        char buffer[2];
        if(seconds()-stateOfCreate.batteryCharge.lastUpdate > lag){
                CREATE_BUSY;
                create_write_byte(142);
                create_write_byte(25);
                CREATE_WAITFORBUFFER(buffer,2,100025)
                stateOfCreate.batteryCharge.data=buffer[0]*256;
                stateOfCreate.batteryCharge.data=stateOfCreate.voltage.data+buffer[1];
                stateOfCreate.batteryCharge.lastUpdate=seconds();
                CREATE_FREE;
        }
        return(stateOfCreate.batteryCharge.data);
}

int get_create_battery_capacity(float lag)
{
        char buffer[2];
        if(seconds()-stateOfCreate.batteryCapacity.lastUpdate > lag){
                CREATE_BUSY;
                create_write_byte(142);
                create_write_byte(26);
                CREATE_WAITFORBUFFER(buffer,2,100026)
                stateOfCreate.batteryCapacity.data=buffer[0]*256;
                stateOfCreate.batteryCapacity.data=stateOfCreate.voltage.data+buffer[1];
                stateOfCreate.batteryCapacity.lastUpdate=seconds();
                CREATE_FREE;
        }
        return(stateOfCreate.batteryCapacity.data);
}


//updates and returns wall sensor in analog mode
int get_create_wall_amt(float lag) {
        char buffer[2];
        if(seconds()-stateOfCreate.wallSignal.lastUpdate > lag){
                CREATE_BUSY;
                create_write_byte(142);
                create_write_byte(27);  //  wall seen or not (1 byte)
                CREATE_WAITFORBUFFER(buffer,2,100027)
                stateOfCreate.wallSignal.data=256*buffer[0];
                stateOfCreate.wallSignal.data=stateOfCreate.wallSignal.data+buffer[1];
                stateOfCreate.wallSignal.lastUpdate=seconds();
                CREATE_FREE;
        }
        return(stateOfCreate.wallSignal.data);
}

//updates and returns cliff sensor in analog mode
int get_create_lcliff_amt(float lag) {
        char buffer[2];
        if(seconds()-stateOfCreate.lcliffSignal.lastUpdate > lag){
                CREATE_BUSY;
                create_write_byte(142);
                create_write_byte(28);  //  wall seen or not (1 byte)
                CREATE_WAITFORBUFFER(buffer,2,100028)
                stateOfCreate.lcliffSignal.data=256*buffer[0];
                stateOfCreate.lcliffSignal.data=stateOfCreate.lcliffSignal.data+buffer[1];
                stateOfCreate.lcliffSignal.lastUpdate=seconds();
                CREATE_FREE;
        }
        return(stateOfCreate.lcliffSignal.data);
}


//updates and returns cliff sensor in analog mode
int get_create_lfcliff_amt(float lag) {
        char buffer[2];
        if(seconds()-stateOfCreate.lfcliffSignal.lastUpdate > lag){
                CREATE_BUSY;
                create_write_byte(142);
                create_write_byte(29);  // lf cliff
                CREATE_WAITFORBUFFER(buffer,2,100029)
                stateOfCreate.lfcliffSignal.data=256*buffer[0];
                stateOfCreate.lfcliffSignal.data=stateOfCreate.lfcliffSignal.data+buffer[1];
                stateOfCreate.lfcliffSignal.lastUpdate=seconds();
                CREATE_FREE;
        }
        return(stateOfCreate.lfcliffSignal.data);
}


//updates and returns cliff sensor in analog mode
int get_create_rfcliff_amt(float lag) {
        char buffer[2];
        if(seconds()-stateOfCreate.rfcliffSignal.lastUpdate > lag){
                CREATE_BUSY;
                create_write_byte(142);
                create_write_byte(30);  //  rf
                CREATE_WAITFORBUFFER(buffer,2,100030)
                stateOfCreate.rfcliffSignal.data=256*buffer[0];
                stateOfCreate.rfcliffSignal.data=stateOfCreate.rfcliffSignal.data+buffer[1];
                stateOfCreate.rfcliffSignal.lastUpdate=seconds();
                CREATE_FREE;
        }
        return(stateOfCreate.rfcliffSignal.data);
}


//updates and returns cliff sensor in analog mode
int get_create_rcliff_amt(float lag) {
        char buffer[2];
        if(seconds()-stateOfCreate.rcliffSignal.lastUpdate > lag){
                CREATE_BUSY;
                create_write_byte(142);
                create_write_byte(31);  //  right cliff
                CREATE_WAITFORBUFFER(buffer,2,100031)
                stateOfCreate.rcliffSignal.data=256*buffer[0];
                stateOfCreate.rcliffSignal.data=stateOfCreate.rcliffSignal.data+buffer[1];
                stateOfCreate.rcliffSignal.lastUpdate=seconds();
                CREATE_FREE;
        }
        return(stateOfCreate.rcliffSignal.data);
}


//updates and returns cargo bay digitals b4 is device detect; b3 is pin 6; b2 pin 18; b0 pin 17
int get_create_cargo_bay_digitals(float lag) {
        char buffer[1];
        if(seconds()-stateOfCreate.cargoBayDI.lastUpdate > lag){
                CREATE_BUSY;
                create_write_byte(142);
                create_write_byte(32);  //
                CREATE_WAITFORBUFFER(buffer,1,100032)
                stateOfCreate.cargoBayDI.data=buffer[0];
                stateOfCreate.cargoBayDI.lastUpdate=seconds();
                CREATE_FREE;
        }
        return(stateOfCreate.cargoBayDI.data);
}

//updates and returns CB analog sensor pin 4
int get_create_cargo_bay_analog(float lag) {
        char buffer[2];
        if(seconds()-stateOfCreate.cargoBayAI.lastUpdate > lag){
                CREATE_BUSY;
                create_write_byte(142);
                create_write_byte(33);  //  CB analog 0 -1023
                CREATE_WAITFORBUFFER(buffer,2,100033)
                stateOfCreate.cargoBayAI.data=256*buffer[0];
                stateOfCreate.cargoBayAI.data=stateOfCreate.rcliffSignal.data+buffer[1];
                stateOfCreate.cargoBayAI.lastUpdate=seconds();
                CREATE_FREE;
        }
        return(stateOfCreate.cargoBayAI.data);
}


int get_create_battery_charging_source(float lag)
{
        char buffer[1];
        if(seconds()-stateOfCreate.chargingSource.lastUpdate > lag){
                CREATE_BUSY;
                create_write_byte(142);
                create_write_byte(34);
                CREATE_WAITFORBUFFER(buffer,1,100034)
                stateOfCreate.chargingSource.data=buffer[0];//1 if charger 2 if home base 0 if no charge source
                stateOfCreate.chargingSource.lastUpdate=seconds();
                CREATE_FREE;
        }
        return(stateOfCreate.chargingSource.data);
}


//packet 36; currently selected song number
int get_create_song_number(float lag)
{
        char buffer[1];
        if(seconds()-stateOfCreate.songNumber.lastUpdate > lag){
                CREATE_BUSY;
                create_write_byte(142);
                create_write_byte(36);
                CREATE_WAITFORBUFFER(buffer,1,100036)
                stateOfCreate.songNumber.data=buffer[0];//song number 0-15
                stateOfCreate.songNumber.lastUpdate=seconds();
                CREATE_FREE;
        }
        return(stateOfCreate.songNumber.data);
}

//packet 37;1=song is playing, 0 song not playing
int get_create_song_playing(float lag)
{
        char buffer[1];
        if(seconds()-stateOfCreate.songPlaying.lastUpdate > lag){
                CREATE_BUSY;
                create_write_byte(142);
                create_write_byte(37);
                CREATE_WAITFORBUFFER(buffer,1,100037)
                stateOfCreate.songPlaying.data=buffer[0];//song number 0-15
                stateOfCreate.songPlaying.lastUpdate=seconds();
                CREATE_FREE;
        }
        return(stateOfCreate.songPlaying.data);
}

//packet-38, number of streamed packets, 0-43, is returned
int get_create_number_of_stream_packets(float lag)
{
        char buffer[1];
        if(seconds()-stateOfCreate.numStreamPackets.lastUpdate > lag){
                CREATE_BUSY;
                create_write_byte(142);
                create_write_byte(38);
                CREATE_WAITFORBUFFER(buffer,1,100038)
                stateOfCreate.numStreamPackets.data=buffer[0];//song number 0-43
                stateOfCreate.numStreamPackets.lastUpdate=seconds();
                CREATE_FREE;
        }
        return(stateOfCreate.numStreamPackets.data);
}

//packet 39; -500 to 500
int get_create_requested_velocity(float lag)
{
        char buffer[2];
        if(seconds()-stateOfCreate.requestedVelocity.lastUpdate > lag){
                CREATE_BUSY;
                create_write_byte(142);
                create_write_byte(39);
                CREATE_WAITFORBUFFER(buffer,2,100039)
                stateOfCreate.requestedVelocity.data=buffer[0]*256;
                stateOfCreate.requestedVelocity.data=stateOfCreate.requestedVelocity.data+buffer[1];
                if(stateOfCreate.requestedVelocity.data>32767)stateOfCreate.requestedVelocity.data=stateOfCreate.requestedVelocity.data-65536;
                stateOfCreate.requestedVelocity.lastUpdate=seconds();
                CREATE_FREE;
        }
        return(stateOfCreate.requestedVelocity.data);
}

//packet 40; -32768 to 32767
int get_create_requested_radius(float lag)
{
        char buffer[2];
        if(seconds()-stateOfCreate.requestedRadius.lastUpdate > lag){
                CREATE_BUSY;
                create_write_byte(142);
                create_write_byte(40);
                CREATE_WAITFORBUFFER(buffer,2,100040)
                stateOfCreate.requestedRadius.data=buffer[0]*256;
                stateOfCreate.requestedRadius.data=stateOfCreate.requestedRadius.data+buffer[1];
                if(stateOfCreate.requestedRadius.data>32767)stateOfCreate.requestedRadius.data=stateOfCreate.requestedRadius.data-65536;
                stateOfCreate.requestedRadius.lastUpdate=seconds();
                CREATE_FREE;
        }
        return(stateOfCreate.requestedRadius.data);
}

//packet 41; -500 to 500
int get_create_requested_right_velocity(float lag)
{
        char buffer[2];
        if(seconds()-stateOfCreate.requestedRVelocity.lastUpdate > lag){
                CREATE_BUSY;
                create_write_byte(142);
                create_write_byte(41);
                CREATE_WAITFORBUFFER(buffer,2,100041)
                stateOfCreate.requestedRVelocity.data=buffer[0]*256;
                stateOfCreate.requestedRVelocity.data=stateOfCreate.requestedVelocity.data+buffer[1];
                if(stateOfCreate.requestedRVelocity.data>32767)stateOfCreate.requestedRVelocity.data=stateOfCreate.requestedRVelocity.data-65536;
                stateOfCreate.requestedRVelocity.lastUpdate=seconds();
                CREATE_FREE;
        }
        return(stateOfCreate.requestedRVelocity.data);
}

//packet 42; -500 to 500
int get_create_requested_left_velocity(float lag)
{
        char buffer[2];
        if(seconds()-stateOfCreate.requestedLVelocity.lastUpdate > lag){
                CREATE_BUSY;
                create_write_byte(142);
                create_write_byte(42);
                CREATE_WAITFORBUFFER(buffer,2,100042)
                stateOfCreate.requestedLVelocity.data=buffer[0]*256;
                stateOfCreate.requestedLVelocity.data=stateOfCreate.requestedVelocity.data+buffer[1];
                if(stateOfCreate.requestedLVelocity.data>32767)stateOfCreate.requestedLVelocity.data=stateOfCreate.requestedLVelocity.data-65536;
                stateOfCreate.requestedLVelocity.lastUpdate=seconds();
                CREATE_FREE;
        }
        return(stateOfCreate.requestedLVelocity.data);
}


////////////////////CREATE MOVEMENT FUNCTIONS///////////////////////////////////////////////

// This command drives the robot along a curve with radius (in mm) radius; and at a speed (mm/sec) of speed
// a radius of 32767 will drive the robot straight
// a radius of 1 will spin the robot CCW
// a radius of -1 will spin the robot CW
// Negative radii will be right turns, positive radii left turns
void create_drive (int speed, int radius)
{
        if(stateOfCreate.drive==1 && stateOfCreate.radius==radius && stateOfCreate.speed==speed)return;//if create is already doing this, return
        CREATE_BUSY;
        create_write_byte(137);
        create_write_byte(HIGH_BYTE(speed));
        create_write_byte(LOW_BYTE(speed));
        create_write_byte(HIGH_BYTE(radius));
        create_write_byte(LOW_BYTE(radius));
        stateOfCreate.driveDirect=0;
        stateOfCreate.drive=1;
        stateOfCreate.radius=radius;
        stateOfCreate.speed=speed;
        stateOfCreate.lspeed=0;
        stateOfCreate.rspeed=0;
        CREATE_FREE;
}

// special version of command above drives robot at speed 0, stopping it.
void create_stop()
{
        CREATE_BUSY;
        create_write_byte(137);
        create_write_byte(0);
        create_write_byte(0);
        create_write_byte(0);
        create_write_byte(0);
        stateOfCreate.driveDirect=0;
        stateOfCreate.drive=0;
        stateOfCreate.radius=0;
        stateOfCreate.speed=0;
        stateOfCreate.lspeed=0;
        stateOfCreate.rspeed=0;
        CREATE_FREE;
}

// special version of command above drives robot at speed speed.  Negative speed will drive robot backwards
void create_drive_straight (int speed)
{
        if(stateOfCreate.drive==1 && stateOfCreate.radius==32767 && stateOfCreate.speed==speed)return;//if create is already doing this, return
        CREATE_BUSY;
        create_write_byte(137);
        create_write_byte(HIGH_BYTE(speed));
        create_write_byte(LOW_BYTE(speed));
        create_write_byte(HIGH_BYTE(32767));
        create_write_byte(LOW_BYTE(32767));
        CREATE_FREE;
        stateOfCreate.driveDirect=0;
        stateOfCreate.drive=1;
        stateOfCreate.radius=32767;
        stateOfCreate.speed=speed;
        stateOfCreate.lspeed=0;
        stateOfCreate.rspeed=0;
}

// special version of command spins robot CW with the wheels turning at speed speed
void create_spin_CW (int speed)
{
        if(stateOfCreate.drive==1 && stateOfCreate.radius==-1 && stateOfCreate.speed==speed)return;//if create is already doing this, return
        CREATE_BUSY;
        create_write_byte(137);
        create_write_byte(HIGH_BYTE(speed));
        create_write_byte(LOW_BYTE(speed));
        create_write_byte(HIGH_BYTE(-1));
        create_write_byte(LOW_BYTE(-1));
        stateOfCreate.driveDirect=0;
        stateOfCreate.drive=1;
        stateOfCreate.radius=-1;
        stateOfCreate.speed=speed;
        stateOfCreate.lspeed=0;
        stateOfCreate.rspeed=0;
        CREATE_FREE;
}

// special version of command spins robot CCW with the wheels turning at speed speed
void create_spin_CCW (int speed)
{
        if(stateOfCreate.drive==1 && stateOfCreate.radius==1 && stateOfCreate.speed==speed)return;//if create is already doing this, return
        CREATE_BUSY;
        create_write_byte(137);
        create_write_byte(HIGH_BYTE(speed));
        create_write_byte(LOW_BYTE(speed));
        create_write_byte(HIGH_BYTE(1));
        create_write_byte(LOW_BYTE(1));
        stateOfCreate.driveDirect=0;
        stateOfCreate.drive=1;
        stateOfCreate.radius=1;
        stateOfCreate.speed=speed;
        stateOfCreate.lspeed=0;
        stateOfCreate.rspeed=0;
        CREATE_FREE;
}

// this function drives the right wheel at r_speed and the left wheel at l_speed
// speeds for all of these functions are +/-500mm/sec.
void create_drive_direct(int r_speed, int l_speed)
{
        if(stateOfCreate.driveDirect==1 && stateOfCreate.lspeed==l_speed && stateOfCreate.rspeed==r_speed)return;//if create is already doing this, return
        CREATE_BUSY;
        create_write_byte(145);
        create_write_byte(HIGH_BYTE(r_speed));
        create_write_byte(LOW_BYTE(r_speed));
        create_write_byte(HIGH_BYTE(l_speed));
        create_write_byte(LOW_BYTE(l_speed));
        stateOfCreate.driveDirect=1;
        stateOfCreate.drive=0;
        stateOfCreate.radius=0;
        stateOfCreate.speed=0;
        stateOfCreate.lspeed=l_speed;
        stateOfCreate.rspeed=r_speed;
        CREATE_FREE;
}


//This function blocks and does a pretty accurate spin.  Note that the function will
//not return until the spin is complete
//CAUTION, requesting the robot to spin more than about 3600 degrees may never terminate
//the 9/07 firmware update is required for this works. Returns -1 if error
int create_spin_block(int speed, int angle)
{
        char buffer[7];
        char *bptr = buffer;
        int r[7]={18,7,9,24,12,15,21},v[7],i,b;
        long lenc=0L, renc=0L,slenc,srenc,flenc,frenc;
        float offset, ticsPerMM=7.8324283, rad=129.0, pi=3.1415926;
        CREATE_BUSY;
        create_write_byte(8);//version
        CREATE_WAITFORBUFFER(buffer,7,-99)
        for(i=0;i<7;i++){
                v[i]=(*(bptr++));
        }
        CREATE_FREE;
        if(!(v[1]==r[1] && v[2]==r[2] && v[3]==r[3])){
                printf("create_spin_block\n requires Create Firmware dated\n 9/24/2007\n");
                printf("Yours is:%d/%d/%d\nFunction aborted\n", v[2], v[3], v[1]+2000);
                return(-1);
        }
        _create_get_raw_encoders(&slenc, &srenc);
        offset=(float)angle*pi*rad*ticsPerMM/180.0;
        if(angle>0){create_drive_direct(speed,-speed);}
        else {offset=-offset; create_drive_direct(-speed,speed);}
        flenc=slenc+(long)offset;
        frenc=srenc+(long)offset;
        if(frenc > 65535L && flenc > 65000L){
                while(lenc >= slenc)_create_get_raw_encoders(&lenc, &renc);
                while(lenc < flenc-65536L)_create_get_raw_encoders(&lenc, &renc);
        create_stop();}
        else{
                while(lenc < flenc && renc < frenc)_create_get_raw_encoders(&lenc, &renc);
                create_stop();
        }
        return(0);
}
int _create_get_raw_encoders(long *lenc, long *renc)
{
        char buffer[4];
        char *bptr = buffer;
        CREATE_BUSY;
        create_write_byte(149);
        create_write_byte(2);
        create_write_byte(43);
        create_write_byte(44);
        CREATE_WAITFORBUFFER(buffer,4,-14)
        *lenc=(long)(*(bptr++))*256L;
        *lenc=*lenc+(long)(*(bptr++));//left encoder value
        *renc=(long)(*(bptr++))*256L;
        *renc=*renc+(long)(*(bptr++));//left encoder value
        CREATE_FREE;
}



/////////////////////////CREATE LEDs///////////////////

// turns on and off the advance (>>) LED (0 is off, 1 is on)
void create_advance_led(int on)
{
        if(on){
                if(!(stateOfCreate.leds[0] & 8)) stateOfCreate.leds[0]=stateOfCreate.leds[0]+ 8;
        }
        else{
                if(stateOfCreate.leds[0] & 8) stateOfCreate.leds[0]=stateOfCreate.leds[0]- 8;
        }
        CREATE_BUSY;
        create_write_byte(139);
        create_write_byte(stateOfCreate.leds[0]);
        create_write_byte(stateOfCreate.leds[1]);
        create_write_byte(stateOfCreate.leds[2]);
        CREATE_FREE;
}


// turns on and off the play LED (0 is off, 1 is on)
void create_play_led(int on)
{
        if(on){
                if(!(stateOfCreate.leds[0] & 2)) stateOfCreate.leds[0]=stateOfCreate.leds[0]+ 2;
        }
        else{
                if(stateOfCreate.leds[0] & 2) stateOfCreate.leds[0]=stateOfCreate.leds[0]- 2;
        }
        CREATE_BUSY;
        create_write_byte(139);
        create_write_byte(stateOfCreate.leds[0]);
        create_write_byte(stateOfCreate.leds[1]);
        create_write_byte(stateOfCreate.leds[2]);
        CREATE_FREE;
}


//sets create power led. Color =0 is green and color = 255 is red -- with intermediate colors
//brightness of 0 is off and 255 is fully bright
void create_power_led(int color, int brightness)
{
        stateOfCreate.leds[1]=color;
        stateOfCreate.leds[2]=brightness;
        CREATE_BUSY;
        create_write_byte(139);
        create_write_byte(stateOfCreate.leds[0]);
        create_write_byte(stateOfCreate.leds[1]);
        create_write_byte(stateOfCreate.leds[2]);
        CREATE_FREE;
}


/////////// The functions below are for writing to the 25 pin connector

//This function sets the three digital out put pins 20,7,19 where 20 is the high bit and 19 is the low. bits should have a value 0 to 7.
void create_digital_output(int bits)
{
        CREATE_BUSY;
        create_write_byte(147);
        create_write_byte(bits);
        CREATE_FREE;
}

//Sets the PWM signal for the three low side drivers (128 = 100%).  pwm2 is pin 24, pwm1 pin 23 and pwm0 pin 22
void create_pwm_low_side_drivers(int pwm2, int pwm1, int pwm0)
{
        CREATE_BUSY;
        create_write_byte(144);
        create_write_byte(pwm2);
        create_write_byte(pwm1);
        create_write_byte(pwm0);
        CREATE_FREE;
}

//Turns on and off the signal for the three low side drivers (128 = 100%).  pwm2 is pin 24, pwm1 pin 23 and pwm0 pin 22
//A 0 or 1 should be given for each of the drivers to turn them off or on.
void create_low_side_drivers(int pwm2, int pwm1, int pwm0)
{
        CREATE_BUSY;
        create_write_byte(138);
        create_write_byte(4*pwm2+2*pwm1+pwm0);
        CREATE_FREE;
}


/////////// CREATE MUSIC////////////

// this loads a song into the robot's memory.
// song can be numbered 0 to 15. the first element in each row of the array should be
// the number of notes (1-16) the subsequent pairs of bytes should be tone and duration
// see the roomba SCI manual for note codes.
// user's program should load the song data into the array before calling this routine
void create_load_song(int num)
{
        int i, numnotes;
        numnotes=gc_song_array[num][0];
        if(num >= 0 && num <=15 && numnotes > 0 && numnotes <= 16){
                CREATE_BUSY;
                create_write_byte(140);
                create_write_byte(num);
                create_write_byte(numnotes);
                for(i=1; i< 2*numnotes+1; i++)
                create_write_byte(gc_song_array[num][i]);
                CREATE_FREE;
        }
        else printf("illegal song #%d is %d notes long being written to memory\n", num, gc_song_array[num][0]);
}

// see the roomba SCI manual for note codes.
void create_play_song(int num)
{
        int i;
        if(num >= 0 && num <=15 ){
                CREATE_BUSY;
                create_write_byte(141);
                create_write_byte(num);
                CREATE_FREE;
        }
        else printf("Song array reference is out of bounds\n");
}


////////////////////CREATE IR FUNCTIONS///////////////////////////////////////////

// The send_IR command assumes an IR transmitter is connected per the Create Open
// interface spec - uses the switched +5V source (pin 8 of cargo bay adapter)
// and low side driver 1 (pin 23).  An IR data byte is sent in the form expected
// by the Create IR receiver.

//Commands IR transmitter to send a byte of data
//129-143 are sent by the remote control, 240-254 by the base station
void create_send_IR(int byte) {
        CREATE_BUSY;
        create_write_byte(151);
        create_write_byte(byte);
        CREATE_FREE;
}
                /* replaced by get_create_infrared(float lag)
                // gc_IR=255 indicates no IR byte is being received
                int create_read_IR() {
                char buffer[1];
                char *bptr = buffer;
                CREATE_BUSY;
                create_write_byte(142);
                create_write_byte(17); // IR packet
                CREATE_WAITFORBUFFER(buffer,1,-1)
                gc_IR=(*(bptr++));
                CREATE_FREE;
                return(0);
                }
*/

////////////////////CREATE MOVEMENT USING SCRIPTS//////////////////

// WARNING: During script operation, serial commands are ignored,
// potentially hanging the control computer in cases such as
// waiting for a Create sensor update.
// Each movement command signals on pin 20 when it is done
// If this signal is not read, then a timing estimate will
// need to be used instead.
// The most recent script may be repeated by create_write_byte(153)

// script to move dist mm at speed mm/sec
void create_script_move(int dist, int speed) {
        CREATE_BUSY;
        create_write_byte(152); // start script
        create_write_byte(17);  // script length
        create_write_byte(147); // on pin 20
        create_write_byte(0);   //   output 0
        create_write_byte(137); // move
        create_write_byte(HIGH_BYTE(speed));
        create_write_byte(LOW_BYTE(speed));
        create_write_byte(128); // no angle (go straight)
        create_write_byte(0);
        create_write_byte(156); // wait for distance done
        create_write_byte(HIGH_BYTE(dist));
        create_write_byte(LOW_BYTE(dist));
        create_write_byte(137); // stop move
        create_write_byte(0);   // no speed
        create_write_byte(0);
        create_write_byte(0);   // no angle
        create_write_byte(0);
        create_write_byte(147); // on pin 20
        create_write_byte(0);   //   output 1
        // end of script
        create_write_byte(153); // run script
        CREATE_FREE;
}

// script to move with individual motor control
// dist mm with r_speed mm/sec on right wheel and l_speed on left
void create_script_move_direct(int dist, int r_speed, int l_speed) {
        CREATE_BUSY;
        create_write_byte(152); // start script
        create_write_byte(17);  // script length
        create_write_byte(147); // on pin 20
        create_write_byte(0);   //   output 0
        create_write_byte(145); // move
        create_write_byte(HIGH_BYTE(r_speed));
        create_write_byte(LOW_BYTE(r_speed));
        create_write_byte(HIGH_BYTE(l_speed));
        create_write_byte(LOW_BYTE(l_speed));
        create_write_byte(156); // wait for distance done
        create_write_byte(HIGH_BYTE(dist));
        create_write_byte(LOW_BYTE(dist));
        create_write_byte(137); // stop move
        create_write_byte(0);   // no speed
        create_write_byte(0);
        create_write_byte(0);   // no angle
        create_write_byte(0);
        create_write_byte(147); // on pin 20
        create_write_byte(0);   //   output 1
        // end of script
        create_write_byte(153); // run script
        CREATE_FREE;
}


// script to rotate in place through deg degrees
// deg > 0 turn CCW; deg < 0 turn CW
void create_script_turn(int deg, int speed) { // degrees, vel in mm/sec
        CREATE_BUSY;
        create_write_byte(152); // start script
        create_write_byte(17);  // script length
        create_write_byte(147); // on pin 20
        create_write_byte(0);   //   output 0
        create_write_byte(137); // move
        create_write_byte(HIGH_BYTE(speed));
        create_write_byte(LOW_BYTE(speed));
        if (deg > 0){ // CCW case
                create_write_byte(0);
        create_write_byte(1); }
        else { // CW case
                create_write_byte(255);
        create_write_byte(255); }
        create_write_byte(157); // wait for angle done
        create_write_byte(HIGH_BYTE(deg));
        create_write_byte(LOW_BYTE(deg));
        create_write_byte(137); // stop move
        create_write_byte(0);   // no speed
        create_write_byte(0);
        create_write_byte(0);   // no angle
        create_write_byte(0);
        create_write_byte(147); // on pin 20
        create_write_byte(0);   //   output 1
        // end of script
        create_write_byte(153); // run script
        CREATE_FREE;
}

// script to trace an arc of radius rad until deg is reach
// NOTE: if the turn is not in the direction of deg, the arc won't end
void create_script_arc(int rad, int deg, int speed) { // rad in mm, degrees, vel in mm/sec
        CREATE_BUSY;
        create_write_byte(152); // start script
        create_write_byte(17);  // script length
        create_write_byte(147); // on pin 20
        create_write_byte(0);   //   output 0
        create_write_byte(137); // move
        create_write_byte(HIGH_BYTE(speed));
        create_write_byte(LOW_BYTE(speed));
        create_write_byte(HIGH_BYTE(rad));
        create_write_byte(HIGH_BYTE(rad));
        create_write_byte(157); // wait for angle done
        create_write_byte(HIGH_BYTE(deg));
        create_write_byte(LOW_BYTE(deg));
        create_write_byte(137); // stop move
        create_write_byte(0);   // no speed
        create_write_byte(0);
        create_write_byte(0);   // no angle
        create_write_byte(0);
        create_write_byte(147); // on pin 20
        create_write_byte(0);   //   output 1
        // end of script
        create_write_byte(153); // run script
        CREATE_FREE;
}

// Script builder
// cwinton: 11/18/2007
                /* usage:
                create_script_start(); // initiates script build
                create_do_<action>;    // one or more build actions
                create_script_end();   // instantiates script build
                create_script_run();   // runs current build
                *//*
                The script signals on pin 20 when it is done;
                if this signal is not read, then a timing estimate will
                need to be used instead to determine when script action ends.

                Maximum length from do actions is 91 bytes;
                each do action below is 8 bytes, limiting script to no
                more than 11 do actions.
*/
int _create_script[102]={152,0,147,0}, // build space
// (script command, length (to be set), pin 20, output value)
_create_script_indx=0;
void create_script_start() { // initialize script
        _create_script_indx=4;
}

int create_do_move(int dist, int speed) {
        if ((91-_create_script_indx)<8) {
                printf("max script exceeded\n");
        return(_create_script_indx); }  // not enough script space left
        _create_script[_create_script_indx++]=137; // move command
        _create_script[_create_script_indx++]=HIGH_BYTE(speed);
        _create_script[_create_script_indx++]=LOW_BYTE(speed);
        _create_script[_create_script_indx++]=128; // no angle (go straight)
        _create_script[_create_script_indx++]=0;
        _create_script[_create_script_indx++]=156; // wait for distance done
        _create_script[_create_script_indx++]=HIGH_BYTE(dist);
        _create_script[_create_script_indx++]=LOW_BYTE(dist);
        return(0);
}
int create_do_move_direct(int dist, int r_speed, int l_speed) {  // individual motor control move
        if ((91-_create_script_indx)<8) {
                printf("max script exceeded\n");
        return(_create_script_indx); }  // not enough script space left
        _create_script[_create_script_indx++]=145; // move command
        _create_script[_create_script_indx++]=HIGH_BYTE(r_speed);
        _create_script[_create_script_indx++]=LOW_BYTE(r_speed);
        _create_script[_create_script_indx++]=HIGH_BYTE(l_speed);
        _create_script[_create_script_indx++]=LOW_BYTE(l_speed);
        _create_script[_create_script_indx++]=156; // wait for distance done
        _create_script[_create_script_indx++]=HIGH_BYTE(dist);
        _create_script[_create_script_indx++]=LOW_BYTE(dist);
        return(0);
}
int create_do_arc(int rad, int deg, int speed) {
        // warning: CCW movement increases degree, CW decreases
        // opposite signs on rad and vel cause CW rotation
        // if direction is CW, this won't stop if deg > 0
        if ((91-_create_script_indx)<8) {
                printf("max script exceeded\n");
        return(_create_script_indx); }  // not enough script space left
        _create_script[_create_script_indx++]=137; // move command
        _create_script[_create_script_indx++]=HIGH_BYTE(speed);
        _create_script[_create_script_indx++]=LOW_BYTE(speed);
        _create_script[_create_script_indx++]=HIGH_BYTE(rad);
        _create_script[_create_script_indx++]=LOW_BYTE(rad);
        _create_script[_create_script_indx++]=157; // wait for angle done
        _create_script[_create_script_indx++]=HIGH_BYTE(deg);
        _create_script[_create_script_indx++]=LOW_BYTE(deg);
        return(0);
}

int create_do_turn(int deg, int speed) {  // speed > 0
        if ((91-_create_script_indx)<8) {
                printf("max script exceeded\n");
        return(_create_script_indx); }  // not enough script space left
        _create_script[_create_script_indx++]=137; // move command
        _create_script[_create_script_indx++]=HIGH_BYTE(speed);
        _create_script[_create_script_indx++]=LOW_BYTE(speed);
        if (deg > 0){ // CCW case
                _create_script[_create_script_indx++]=0;
        _create_script[_create_script_indx++]=1; }
        else { // CW case
                _create_script[_create_script_indx++]=255;
        _create_script[_create_script_indx++]=255; }
        _create_script[_create_script_indx++]=157; // wait for angle done
        _create_script[_create_script_indx++]=HIGH_BYTE(deg);
        _create_script[_create_script_indx++]=LOW_BYTE(deg);
        return(0);
}

void create_script_end() {
        int i;
        _create_script[_create_script_indx++]=137; // stop move
        _create_script[_create_script_indx++]=0;   // (no speed)
        _create_script[_create_script_indx++]=0;
        _create_script[_create_script_indx++]=0;   // (no angle)
        _create_script[_create_script_indx++]=0;
        _create_script[_create_script_indx++]=147; // on pin 20
        _create_script[_create_script_indx++]=1;   //   output 1
        // set script length
        _create_script[1]=_create_script_indx-2;
        // load script from build space
        CREATE_BUSY;
        for(i=0;i<_create_script_indx;i++)
        create_write_byte(_create_script[i]);
        CREATE_FREE;
}

void create_script_run() {
        create_write_byte(153); // run script
}

int create_read_block(char *data, int count)
{
        float start_time;
        int read_count = 0;

        start_time = seconds();
        while(read_count < count && seconds()-start_time < COMM_TIMEOUT)
        read_count += serial_read(data+read_count, count-read_count);

        return read_count;
}

void create_write_byte(char byte) {
        serial_write(&byte, 1);
}
