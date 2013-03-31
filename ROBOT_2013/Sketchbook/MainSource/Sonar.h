#ifndef SONAR_H_
#define SONAR_H_

#include "Arduino.h"
#include "Wire.h"

#define SONAR_LEFT 0x34 //0x70
#define SONAR_RIGHT 0x36 //0x69

class Sonar {
	
	public:
		Sonar();
		int getLeftDistance();
		int getRightDistance();
		void read();
                void changeAddress(byte oldAddress, byte newAddress);
	private:
		unsigned int leftDistance;
		unsigned int rightDistance;
		int i2cRead(int address);
};

#endif
