/*HUB-ee BMD Arduino Lib
Provides an object for a single motor using the BMD motor driver, includes optional standby control.
Designed for the BMD or BMD-S motor driver which uses the Toshiba TB6593FNG motor driver IC
Created by Creative Robotics Ltd in 2012.
Released into the public domain.
*/
#ifndef HUBeeBMDWheel_H
#define HUBeeBMDWheel_H
#include <Arduino.h>

class Hubee
{
	public:
		Hubee();
		void setup(int In1Pin, int In2Pin, int PWMPin, unsigned int motorPower = 255);
		//Stop the motors with braking
		void stop();
		//Move forward
		void forward();
		//Move backward
		void backward();
		void setMotorPower(int motorPower);
	
	private:
		void setMotor(bool direction);
		int motorPower;
		bool movingForward;

		//pin assignments
		unsigned int IN1;
		unsigned int IN2;
		unsigned int PWM;
};
#endif

