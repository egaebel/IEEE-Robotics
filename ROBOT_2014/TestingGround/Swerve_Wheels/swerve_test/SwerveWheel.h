#ifndef SWERVE_H
#define SWERVE_H

#include "Ax12.h"
#include "Hubee.h"
#include <Arduino.h>

// HuBee wheel reference: http://www.creative-robotics.com/bmdsresources
// 
// Pin IN1 on hubee wheel. Digital pin 22
static const unsigned int HUBEE_PIN_IN1 = 22;
//
// Pin IN2 on hubee wheel. Digital pin 24
static const unsigned int HUBEE_PIN_IN2 = 24;
//
// PWM pin on hubee wheel. PWM pin 9
static const unsigned int HUBEE_PIN_PWM = 9;

//Standby pin on hubee
static const unsigned int HUBEE_PIN_STANDBY = 26;

//***************Wire assignments ***************
//
// BLACK: GND
// WHITE: +V
// GREY: IN1
// PURPLE: IN2
// BLUE: PWM
// GREEN: Standby (active low - can be left disconnected)
// YELLOW/ORANGE: Quadrature sensor channels A and B (not used in this)

//~Class for moving all of the swerve wheels
class SwerveWheel
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
                //Takes an angle between (inclusive) -90 and 90
                //with 0 pointing the wheels forward
       		int setAngle(int angle);
		//Takes an angle between (inclusive) -90 and 90
                //with 0 pointing the wheels forward
                int moveAngle(int angle);
		void moveForward();
		void moveBackward();
		void stop();
	private:
                Hubee hubee;
	        Ax12 ax;
};
#endif
