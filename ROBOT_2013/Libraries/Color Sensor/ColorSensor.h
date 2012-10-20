/*
||
|| @file colorSensor.h
|| @author VT IEEE Hardware Team
||
|| @description
|| | Provides a abstract method to call in order to get the color of the object in front of the robot. (For TCS3200 Color sensor)
|| #
||
|| @license
|| | Declares the methods from Eecher's TCS3200 program adapted from code found at reibot.org
|| #
||
*/

//define the functionality of the states
class State {
	public:
		void senseColor();
		
	private:
		int colorRead(int taosOutPin);
		void TCS3200setup();
};




