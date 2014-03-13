//Sergey Sabirov
//Electrical Engineering Junior, Class 2015
//
//**************************IMPORTANT************************************
//                     For ARDUINO MEGA 2560:
//
// Connect ShiftRegister IC CLOCK pin(2) to digital PIN #52 (SCK)!!!
// Connect ShiftRegister IC inverted OUTPUT pin(7) to digital PIN #50(MISO)
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


#ifndef LINE_FOLLOW_2_H
#define LINE_FOLLOW_2_H

#include <SPI.h> // Not needed for parallel data input; FALSE, byte TYPE IS DEFINED WITHIN! 
class ParallelLineFollower
{
private:
	unsigned short sensor; // ENABLE PIN, SINGLE
	byte L_bits;
	byte R_bits;
	byte Line_Data;
	void Get_Line_Data();
	unsigned short PIN_LF_S0;
    unsigned short PIN_LF_S1;
    unsigned short PIN_LF_S2;
    unsigned short PIN_LF_S3;
    unsigned short PIN_LF_S4;
    unsigned short PIN_LF_S5;
    unsigned short PIN_LF_S6;
    unsigned short PIN_LF_S7;

public:
	void setup(unsigned short sensorPin,
			   unsigned short PIN_LF_S0, unsigned short PIN_LF_S1, 
			   unsigned short PIN_LF_S2, unsigned short PIN_LF_S3,
			   unsigned short PIN_LF_S4, unsigned short PIN_LF_S5, 
			   unsigned short PIN_LF_S6, unsigned short PIN_LF_S7);
	bool isCentered(byte& L_bits, byte& R_bits);
	bool intersection(byte& L_bits, byte& R_bits);
	void Get_Line_Data(byte& L_bits, byte& R_bits);
};

#endif