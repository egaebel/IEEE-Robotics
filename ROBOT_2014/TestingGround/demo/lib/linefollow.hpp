//Sergey Sabirov
//Electrical Engineering Junior, Class 2015
/*-------------------------IMPORTANT-----------------------------------------
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
*/

#ifndef LINE_FOLLOW_H
#define LINE_FOLLOW_H

#include <SPI.h>

class LineFollower
{
private:
	unsigned short Load; // Controls ShiftRegister's Shift/Load pin(1)
	unsigned short sensor; // Controls LineFollower's Enable pin

	byte L_bits;
	byte R_bits;
	byte Line_Data;
	void Get_Line_Data();
public:
	void setup(unsigned short loadPin, 
				unsigned short sensorPin);
	bool isCentered(byte& L_bits, byte& R_bits);
	bool intersection(byte& L_bits, byte& R_bits);
	void Get_Line_Data(byte& L_bits, byte& R_bits);
};

#endif