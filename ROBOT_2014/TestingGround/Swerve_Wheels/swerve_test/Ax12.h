#ifndef AX12_H
#define AX12_H

#include <Arduino.h>

// instruction table for Dynamixel AX-12A
//---------------------------------------
#define ping_ins 1 // checked
#define read_ins 2 // checked
#define write_ins 3 // Checked
#define reg_write_ins 4 // Checked 
#define action_ins 5 // Checked
#define reset_ins 6 // Checked 
#define sync_write_ins 131 

//---------------------------------------

//registers table for Dynamixel AX-12A
//---------------------------------------
#define servo_id 3
#define baud_rate 4
#define max_torque 14
#define alarm_led 17
#define alarm_shutdown 18
#define torque_enable 24
#define goal_position 30
#define moving_speed 32
#define current_position 36
#define present_temp 43
//---------------------------------------

//Other necessary values for Dynamixel AX-12A
//---------------------------------------
#define broadcast_id 0xFE
//Goal_position that points directly in front of the robot
#define DEAD_AHEAD 660

//---------------------------------------

//~Class for moving all of the AX12 servos at once
class Ax12
{
	//==================================================
	//~~SERIAL INFORMATION=================================
	//==================================================
	/*
		Code is set up to communicate on serial3.
	*/
	//==================================================

	public:
		void setup();
		int setAngle(int angle);
	private:
		void recieve();
		void transmit();
		void reg_write_1_byte(int id, int location, int val);
		void reg_write_2_byte(int id, int location,int val);
		void reset(int id);
		void Action(int id);
};
#endif
