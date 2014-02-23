//Sergey Sabirov
//Electrical Engineering Junior, Class 2015
//
//**************************IMPORTANT************************************
//                     For ARDUINO MEGA 2560:
//
// Connect ShiftRegister IC CLOCK pin(2) to digital PIN #52 (SCK)!!!
// Coonect ShiftRegister IC inverted OUTPUT pin(7) to digital PIN #50(MISO)
//                     For 74HC165 IC
// Do not forget to connect PIN15 (Clock Inhibit) to the ground!
// Bit Reversal is implemented in hardware. See below the wiring instruction.

//James Reed
//Computer Engineering Sophomore, Class 2016
//Modified 2/2/2014
//
// CHANGELOG
// -Wrapped all of this into a class with simple methods 
// -Tested 
//
//
// Pin Assignments - Arduino Mega 2560
// -Line Sensor enable line should be connected to Arduino pin 24
// -Shift/!enable on shift register (pin 1) should be connected to Arduino pin 22
// -Line Sensor pins OUT0-OUT3 correspond to pins D-A on the shift register
// -Line Sensor pins OUT4-OUT7 correspond to pins E-H on the shift register
//
//
//Preprocessor Directives:
// -Symbol DEBUG_STANDALONE allows this to be compiled and uploaded as 
//  a standalone Arduino sketch. The demo methods from core have been preserved.
// -Symbols DEBUG prints the raw line data byte to the serial output


#ifndef LINE_FOLLOW_H
#define LINE_FOLLOW_H

#include "SPI.h"

//#define DEBUG_STANDALONE

class LineFollower
{
private:
	static const short Load    = 22; // Controls ShiftRegister's Shift/Load pin(1)
	static const short sensor  = 24; // Controls LineFollower's Enable pin

	#ifdef DEBUG_STANDALONE
	static const short L_side_pin = 10; // PWM pin for left trek
	static const short R_side_pin = 9;  // PWM pin for right trek
	static const short Dir_Right_Side  =  30; // Controls direction of the right track(LOW - forward, HIGH - Reverse)
	static const short Dir_Left_Side  =   31; // Controls direction of the left track (LOW - forward, HIGH - Reverse)
	static const byte max_speed = 100;   // PWM value 0-255
	#endif
	
	byte L_bits;
	byte R_bits;
	byte Line_Data;
	
	#ifdef DEBUG_STANDALONE
	short L_PWM;
	short R_PWM;
	short Gate_flag;
	short U_Turn_flag;
	short Num_LT;
	short Hor_Line_pass;
	#endif
	
	void Get_Line_Data();
	
	/* NOTE: these methods are part of the demo code.
	 *
	 * Follow_the_line(), Move_Control(), Left_Turn(), U_Turn(), Demo_Run(), Halt()
	 *
	 * In production, Motor control should be handled outside this class 
	 */
	
	#ifdef DEBUG_STANDALONE
	void Move_Control(short L_PWM, short R_PWM);
	void Follow_the_line();
	void Left_Turn();
	void U_Turn();
	void Demo_Run();
	void Halt();
	#endif
public:
	LineFollower();
	bool isCentered(byte& L_bits, byte& R_bits);
	bool intersection(byte& L_bits, byte& R_bits);
	void Get_Line_Data(byte& L_bits, byte& R_bits);
	
	//these methods allow this to be compiled as a standalone Arduino sketch.
	#ifdef DEBUG_STANDALONE
	void loop();
	void setup();
	#endif

		
};

#endif