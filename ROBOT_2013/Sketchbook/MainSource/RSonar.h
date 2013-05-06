#ifndef RSONAR_H_
#define RSONAR_H_

#include "Arduino.h"

class RSonar {
	
	public:
		RSonar(int pin);
		int getDistance();
		boolean update();
    private:
        int adcPin;
		volatile int distance;
};

#endif
