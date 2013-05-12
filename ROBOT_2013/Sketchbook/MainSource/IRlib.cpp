#include "IRlib.h"
#include <cmath>
#include "Arduino.h"

IRAverager::IRAverager(int pin) {
	ir_pin = pin;
	slot = 0;
	sum = 0;
	for (int i = 0; i < lengthArray; i++) {
		valArray[i] = 0;
	}
}

void IRAverager::updateIR() {
	sum -= valArray[slot];
    double x = analogRead(ir_pin);
    double distance3 = 10650.08*pow(x,-0.935)-10;
    if (distance3 > 150)
      distance3 = 150;
	valArray[slot] = distance3;
	sum += valArray[slot];
	slot++;
	if (slot > lengthArray-1) {
		slot = 0;
	}
}

float IRAverager::getIR() {
	float devsum = 0;
	float mean = sum/lengthArray;
	for (int i = 0; i < lengthArray-1; i++) {
		devsum += pow((valArray[i] - mean),2);
	}
	
	float stdDev = sqrt(devsum/(lengthArray));

	int out = 0;
	float newSum;
	for (int i = 0; i < lengthArray-1; i++) {
		if (valArray[i] > mean - (2*stdDev) && valArray[i] < mean + (2*stdDev))
		{
			newSum += valArray[i];
		}
		else
			out++;
	}
	return newSum/(lengthArray-out);
}
