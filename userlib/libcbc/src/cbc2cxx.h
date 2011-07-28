/* 
 * C++ Wrapper for libcbc, by Braden McDorman <bmcdorman@kipr.org>
 * Copyright 2011 KISS Institute for Practical Robotics
 *
 * If you don't like how I did it, write your own.
 */

#ifndef __CBC2_CXX_H__
#define __CBC2_CXX_H__

#ifdef __cplusplus

typedef unsigned char port_t;

struct Sound {
	static void tone(int frequency, int duration) { tone(frequency, duration); }
	static void beep() { ::beep(); }
};

struct Port {
	Port(port_t port) : m_port(port) {}
	
	void setPort(port_t port) { m_port = port; }
	port_t getPort() const { return m_port; }
protected:
	port_t m_port;	
};

struct Motor : Port {
	Motor(port_t port) : Port(port) {}
	
	void moveAtVelocity(int velocity) { move_at_velocity(m_port, velocity); }
	void mav(int velocity) { moveAtVelocity(velocity); }
	
	void moveToPosition(int speed, long goalPos) { move_to_position(m_port, speed, goalPos); }
	void mtp(int speed, long goalPos) { moveToPosition(speed, goalPos); }
	
	void moveRelativePosition(int speed, long goalPos) { move_relative_position(m_port, speed, goalPos); }
	void mrp(int speed, long goalPos) { moveRelativePosition(speed, goalPos); }
	
	int freeze() { ::freeze(m_port); }
	
	bool isMotorDone() { get_motor_done(m_port); }
	bool isDone() { return isMotorDone(); }
	
	void blockMotorDone() { block_motor_done(m_port); }
	void blockDone() { blockMotorDone(); }
	void bmd() { blockDone(); }
	
	int setPWM(int pwm) { setpwm(m_port, pwm); }
	
	void fd() { ::fd(m_port); }
	void forward() { fd(); }
	
	void bk() { ::bk(m_port); }
	void backward() { bk(); }
	
	void motor(int speed) { ::motor(m_port, speed); }
	
	void off() { ::off(m_port); }
	
	void setPIDGains(int p, int i, int d, int pd, int id, int dd) { set_pid_gains(m_port, p, i, d, pd, id, dd); }
	
	static void allOff() { ao(); }
};

struct Servo : Port {
	Servo(port_t port) : Port(port) {}
	
	void setPosition(int pos) { set_servo_position(m_port, pos); }
	int getPosition() { return get_servo_position(m_port); }
	
	static void enable() { enable_servos(); }
	static void disable() { disable_servos(); }
	
	static void setEnabled(bool enabled) { if(enabled) enable(); else disable(); }
};

struct IntegerSensor {
	virtual int getValue() const = 0;
};

struct Analog : IntegerSensor, Port {
	Analog(port_t port) : Port(port) {}
	
	int getValue10() const { return analog10(m_port); }
	int getValue8() const { return analog(m_port); }
	
	int getValue() const { return getValue8(); }
	
	static void setEachAnalogState(bool a0, bool a1, bool a2, bool a3, bool a4, bool a5, bool a6, bool a7) {
		set_each_analog_state(a0 ? 1 : 0, a1 ? 1 : 0, a2 ? 1 : 0, a3 ? 1 : 0, a4 ? 1 : 0, a5 ? 1 : 0, a6 ? 1 : 0, a7 ? 1 : 0);
	}
	static void setAnalogFloats(int mask) { set_analog_floats(mask); }
};

struct BooleanSensor {
	virtual bool getValue() const = 0;
};

struct Digital : BooleanSensor, Port {
	Digital(port_t port) : Port(port) {}
	
	bool getValue() const { return digital(m_port); }
	
	void setOutputValue(bool value) { set_digital_output_value(m_port, value ? 1 : 0); }
};

namespace Button {
	struct A : BooleanSensor 	{ bool getValue() const { return a_button() == 1; } };
	struct B : BooleanSensor 	{ bool getValue() const { return b_button() == 1; } };
	struct Up : BooleanSensor 	{ bool getValue() const { return up_button() == 1; } };
	struct Down : BooleanSensor 	{ bool getValue() const { return down_button() == 1; } };
	struct Left : BooleanSensor 	{ bool getValue() const { return left_button() == 1; } };
	struct Right : BooleanSensor 	{ bool getValue() const { return right_button() == 1; } };
	struct Black : BooleanSensor 	{ bool getValue() const { return black_button() == 1; } };
}

namespace Accelerometer {
	struct X : IntegerSensor { int getValue() const { return accel_x(); } };
	struct Y : IntegerSensor { int getValue() const { return accel_y(); } };
	struct Z : IntegerSensor { int getValue() const { return accel_z(); } };
};

namespace Vision {
	struct Blob {
		Blob(int ch, int i) : m_ch(ch), m_i(i) {}
		
		int getSize() { track_size(m_ch, m_i); }
		int getX() { track_x(m_ch, m_i); }
		int getY() { track_y(m_ch, m_i); }
		
		int getConfidence() { return track_confidence(m_ch, m_i); }
		int getBBoxLeft() { return track_bbox_left(m_ch, m_i); }
		int getBBoxRight() { return track_bbox_right(m_ch, m_i); }
		int getBBoxTop() { return track_bbox_top(m_ch, m_i); }
		int getBBoxBottom() { return track_bbox_bottom(m_ch, m_i); }
		int getBBoxWidth() { return track_bbox_width(m_ch, m_i); }
		int getBBoxHeight() { return track_bbox_height(m_ch, m_i); }
		float getAngle() { return track_angle(m_ch, m_i); }
		int getMajorAxis() { return track_major_axis(m_ch, m_i); }
		int getMinorAxis() { return track_minor_axis(m_ch, m_i); }
	private:
		int m_ch;
		int m_i;
	};
	
	struct Channel {
		Channel(int ch) : m_ch(ch) {}
		
		Blob getBlob(int i) { return Blob(m_ch, i); }
		int getBlobCount() { track_count(m_ch); }
		
		void setModelHSV(int hMin, int hMax, int sMin, int vMin) { track_set_model_hsv(m_ch, hMin, hMax, sMin, vMin); }
		void getModelHSV(int* hMin, int* hMax, int* sMin, int* vMin) { track_get_model_hsv(m_ch, hMin, hMax, sMin, vMin); }
	private:
		int m_ch;
	};
	
	struct Camera {
		Camera() : m_channels((Channel[4]) { Channel(0), Channel(1), Channel(2), Channel(3) }) {}

		void update() { track_update(); }
		int getFrame() { return track_get_frame(); }

		int getTimeSinceCapture() { return track_capture_time(); }

		Channel& getChannel(int ch) { return m_channels[ch]; }

	private:
		Channel m_channels[4];
	};
}

namespace CreateSensor {
	struct Lagged {
		Lagged(float lag) : m_lag(lag) {}
	protected:
		float m_lag;
	};
	
	struct LeftBump : Lagged, BooleanSensor {
		LeftBump(float lag = 0.1) : Lagged(lag) {}
		bool getValue() const { return get_create_lbump(m_lag) == 1; }
	};

	struct RightBump : Lagged, BooleanSensor {
		RightBump(float lag = 0.1) : Lagged(lag) {}
		bool getValue() const { return get_create_rbump(m_lag) == 1; }
	};
	
	struct LeftWheelDrop : Lagged, BooleanSensor {
		LeftWheelDrop(float lag = 0.1) : Lagged(lag) {}
		bool getValue() const { return get_create_lwdrop(m_lag) == 1; }
	};
	
	struct CasterWheelDrop : Lagged, BooleanSensor {
		CasterWheelDrop(float lag = 0.1) : Lagged(lag) {}
		bool getValue() const { return get_create_cwdrop(m_lag) == 1; }
	};
	
	struct RightWheelDrop : Lagged, BooleanSensor {
		RightWheelDrop(float lag = 0.1) : Lagged(lag) {}
		bool getValue() const { return get_create_rwdrop(m_lag) == 1; }
	};
	
	struct Wall : Lagged, BooleanSensor {
		Wall(float lag = 0.1) : Lagged(lag) {}
		bool getValue() const { return get_create_wall(m_lag) == 1; }
	};

	struct LeftCliff : Lagged, BooleanSensor {
		LeftCliff(float lag = 0.1) : Lagged(lag) {}
		bool getValue() const { return get_create_lcliff(m_lag) == 1; }
	};

	struct RightCliff : Lagged, BooleanSensor {
		RightCliff(float lag = 0.1) : Lagged(lag) {}
		bool getValue() const { return get_create_rcliff(m_lag) == 1; }
	};

	struct LeftFrontCliff : Lagged, BooleanSensor {
		LeftFrontCliff(float lag = 0.1) : Lagged(lag) {}
		bool getValue() const { return get_create_lfcliff(m_lag) == 1; }
	};
	
	struct RightFrontCliff : Lagged, BooleanSensor {
		RightFrontCliff(float lag = 0.1) : Lagged(lag) {}
		bool getValue() const { return get_create_rfcliff(m_lag) == 1; }
	};
	
	struct VirtualWall : Lagged, BooleanSensor {
		VirtualWall(float lag = 0.1) : Lagged(lag) {}
		bool getValue() const { return get_create_vwall(m_lag) == 1; }
	};
	
	struct Overcurrents : Lagged, IntegerSensor {
		Overcurrents(float lag = 0.1) : Lagged(lag) {}
		int getValue() const { return get_create_overcurrents(m_lag); }
	};
	
	struct Infrared : Lagged, IntegerSensor {
		Infrared(float lag = 0.1) : Lagged(lag) {}
		int getValue() const { return get_create_infrared(m_lag); }
	};
	
	struct AdvanceButton : Lagged, BooleanSensor {
		AdvanceButton(float lag = 0.1) : Lagged(lag) {}
		bool getValue() const { return get_create_advance_button(m_lag) == 1; }
	};
	
	struct PlayButton : Lagged, BooleanSensor {
		PlayButton(float lag = 0.1) : Lagged(lag) {}
		bool getValue() const { return get_create_play_button(m_lag) == 1; }
	};
	
	struct BayDigital : Lagged, BooleanSensor {
		BayDigital(port_t port, float lag = 0.1) : Lagged(lag), m_port(port) {}
		bool getValue() const { return get_create_bay_DI(m_lag) & ((m_port >= 3) ? 4 : m_port); }
	private:
		port_t m_port;
	};
	
	struct BayAnalog : Lagged, IntegerSensor {
		BayAnalog(float lag = 0.1) : Lagged(lag) {}
		int getValue() const { return get_create_bay_AI(m_lag); }
	};
	
	struct WallAmount : Lagged, IntegerSensor {
		WallAmount(float lag = 0.1) : Lagged(lag) {}
		int getValue() const { return get_create_wall_amt(m_lag); }
	};

	struct LeftCliffAmount : Lagged, IntegerSensor {
		LeftCliffAmount(float lag = 0.1) : Lagged(lag) {}
		int getValue() const { return get_create_lcliff_amt(m_lag); }
	};

	struct RightCliffAmount : Lagged, IntegerSensor {
		RightCliffAmount(float lag = 0.1) : Lagged(lag) {}
		int getValue() const { return get_create_rcliff_amt(m_lag); }
	};

	struct LeftFrontCliffAmount : Lagged, IntegerSensor {
		LeftFrontCliffAmount(float lag = 0.1) : Lagged(lag) {}
		int getValue() const { return get_create_lfcliff_amt(m_lag); }
	};
	
	struct RightFrontCliffAmount : Lagged, IntegerSensor {
		RightFrontCliffAmount(float lag = 0.1) : Lagged(lag) {}
		int getValue() const { return get_create_rfcliff_amt(m_lag); }
	};
}

enum CreateMode {
	Passive,
	Safe,
	Full
};

struct Create {
	int connect() { return create_connect(); }
	void disconnect() { create_disconnect(); }
	
	void start() { create_start(); }
	void passive() { create_passive(); }
	void safe() { create_safe(); }
	void full() { create_full(); }
	void spot() { create_spot(); }
	void cover() { create_cover(); }
	void demo(int d) { create_demo(d); }
	void coverDock() { create_cover_dock(); }
	
	void setMode(CreateMode mode) {
		switch(mode) {
			case Safe: safe(); return;
			case Full: full(); return;
		}
		passive();
	}
	
	CreateMode getMode(float lag = 0.1) const {
		switch(get_create_mode(lag)) {
			case 1: return Passive;
			case 2: return Safe;
		}
		return Full; 
	}
	
	CreateSensor::LeftBump getLeftBump(float lag = 0.1) const {return CreateSensor::LeftBump(lag); }
	CreateSensor::RightBump getRightBump(float lag = 0.1) const { return CreateSensor::RightBump(lag); }
	CreateSensor::LeftWheelDrop getLeftWheelDrop(float lag = 0.1) const { return CreateSensor::LeftWheelDrop(lag); }
	CreateSensor::CasterWheelDrop getCasterWheelDrop(float lag = 0.1) const { return CreateSensor::CasterWheelDrop(lag); }
	CreateSensor::RightWheelDrop getRightWheelDrop(float lag = 0.1) const { return CreateSensor::RightWheelDrop(lag); }
	CreateSensor::Wall getWall(float lag = 0.1) const { return CreateSensor::Wall(lag); }
	CreateSensor::LeftCliff getLeftCliff(float lag = 0.1) const { return CreateSensor::LeftCliff(lag); }
	CreateSensor::RightCliff getRightCliff(float lag = 0.1) const { return CreateSensor::RightCliff(lag); }
	CreateSensor::LeftFrontCliff getLeftFrontCliff(float lag = 0.1) const { return CreateSensor::LeftFrontCliff(lag); }
	CreateSensor::RightFrontCliff getRightFrontCliff(float lag = 0.1) const { return CreateSensor::RightFrontCliff(lag); }
	CreateSensor::VirtualWall getVirtualWall(float lag = 0.1) const { return CreateSensor::VirtualWall(lag); }
	CreateSensor::Overcurrents getOvercurrents(float lag = 0.1) const { return CreateSensor::Overcurrents(lag); }
	CreateSensor::Infrared getInfrared(float lag = 0.1) const { return CreateSensor::Infrared(lag); }
	CreateSensor::AdvanceButton getAdvanceButton(float lag = 0.1) const { return CreateSensor::AdvanceButton(lag); }
	CreateSensor::PlayButton getPlayButton(float lag = 0.1) const { return CreateSensor::PlayButton(lag); }
	
	int getDistance(float lag = 0.1) { return get_create_distance(lag); }
	void setDistance(int dist) { set_create_distance(dist); }
	
	int getNormalizedAngle(float lag = 0.1) { return get_create_normalized_angle(lag); }
	int getTotalAngle(float lag = 0.1) { return get_create_total_angle(lag); }
	void setNormalizedAngle(int a) { set_create_normalized_angle(a); }
	void setTotalAngle(int a) { set_create_total_angle(a); }
	
	CreateSensor::WallAmount getWallAmount(float lag = 0.1) const { return CreateSensor::WallAmount(lag); }
	CreateSensor::LeftCliffAmount getLeftCliffAmount(float lag = 0.1) const { return CreateSensor::LeftCliffAmount(lag); }
	CreateSensor::LeftFrontCliffAmount getLeftFrontCliffAmount(float lag = 0.1) const { return CreateSensor::LeftFrontCliffAmount(lag); }
	CreateSensor::RightFrontCliffAmount getRightFrontcliffAmount(float lag = 0.1) const { return CreateSensor::RightFrontCliffAmount(lag); }
	CreateSensor::RightCliffAmount getRightCliffAmount(float lag = 0.1) const { return CreateSensor::RightCliffAmount(lag); }
	CreateSensor::BayDigital getBayDI(port_t port, float lag = 0.1) const { return CreateSensor::BayDigital(port, lag); }
	CreateSensor::BayAnalog getBayAI(float lag = 0.1) const { return CreateSensor::BayAnalog(lag); }
	
	int getSongNumber(float lag = 0.1) const { get_create_song_number(lag); }
	int getSongPlaying(float lag = 0.1) const { get_create_song_playing(lag); }
	int getNumberOfStreamPackets(float lag = 0.1) const { get_create_number_of_stream_packets(lag); }
	int getRequestedVelocity(float lag = 0.1) const { get_create_requested_velocity(lag); }
	int getRequestedRadius(float lag = 0.1) const { get_create_requested_radius(lag); }
	int getRequestedRightVelocity(float lag = 0.1) const { get_create_requested_right_velocity(lag); }
	int getRequestedLeftVelocity(float lag = 0.1) const { get_create_requested_left_velocity(lag); }

	void stop() { create_stop(); }
	void drive(int speed, int radius) { create_drive(speed, radius); }
	void driveStraight(int speed) { create_drive_straight(speed); }
	void spinCW(int speed) { create_spin_CW(speed); }
	void spinCCW(int speed) { create_spin_CCW(speed); }
	void driveDirect(int rSpeed, int lSpeed) { create_drive_direct(rSpeed, lSpeed); }


	int spinBlock(int speed, int angle) { create_spin_block(speed, angle); }

	void setAdvanceLed(bool state) { create_advance_led(state ? 1 : 0); }
	void setPlayLed(bool state) { create_play_led(state ? 1 : 0); }
	void setPowerLed(int color, int brightness) { create_power_led(color, brightness); }
	void setDigitalOutput(int bits) { create_digital_output(bits); }
	void setPWMLowSideDrivers(int pwm24, int pwm22, int pwm23) { create_pwm_low_side_drivers(pwm24, pwm22, pwm23); }
	void setLowSideDrivers(int pin24, int pin22, int pin23) { create_low_side_drivers(pin24, pin22, pin23); }

	void loadSong(int num) { create_load_song(num); }
	void playSong(int num) { create_play_song(num); }
};

#endif
#endif