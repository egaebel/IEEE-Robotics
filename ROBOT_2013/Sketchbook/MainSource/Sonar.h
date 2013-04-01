#ifndef SONAR_H_
#define SONAR_H_

#include "Arduino.h"
#include "Wire.h"

class Sonar {
	
	public:
		Sonar(int address,int interruptPin = -1);
		int getDistance();
		boolean update();
                void setDataReady(boolean val);
                void changeAddress(byte oldAddress, byte newAddress);
	private:
                boolean dataReady;
                int addr;
                int intPin;
		volatile int distance;
		int i2cRead();
                int i2cRequest();
                
        friend int sonarISR(Sonar* sensor);
};

int sonarISR(Sonar* sensor);

#endif
