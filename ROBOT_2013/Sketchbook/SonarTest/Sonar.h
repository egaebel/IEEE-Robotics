#ifndef SONAR_H_
#define SONAR_H_

#define TWI_FREQ 25000L

#define SONAR_LEFT byte(0x70)
#define SONAR_RIGHT byte(0x69)

#include "Wire.h"

class Sonar {
	
	public:
		Sonar();
		int getLeftDistance();
		int getRightDistance();
	private:
		int leftDistance;
		int rightDistance;
		int i2cRead(int address);
};

#endif