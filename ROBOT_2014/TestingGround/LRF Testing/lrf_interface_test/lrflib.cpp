#include "LRFlib.h"
#include <cmath>
#include "Arduino.h"

LRFAverager::LRFAverager() {
}

void LRFAverager::initialize() {
        Serial1.begin(9600);  //LRF to Serial Monitor
	delay(100);
	Serial1.write('U');
	Serial.println("LRF Initialization Done.");
        Serial1.write('L');
        delay(100);
}
/*
 * Returns an average distance from LRF (created from ten readings) in millimetres
 * or -1 no value returned.
 */
int LRFAverager::get10ReadingAverage() {
	int idx = 0;
	int average = 0;
	int seenError = 0;
	for(idx = 0; idx < 10; idx++)
	{
		int retValue = getSingleReading();
		if(retValue >=0)
		{
			average = average + getSingleReading();
		}
		else if(seenError > 0)
		{
			return -1;
		}
		else
		{
			seenError = 1;
			idx--;
		}
	}
	average = (int)((average * 1.0)/10);
	return average;
}
/*
 * Returns the distance from LRF in millimetres or -1 no value returned.
 */
int LRFAverager::getSingleReading() {
        Serial1.flush();
	char lrfData[32];
	int offset = 0;
	lrfData[0] = 0;
        while (Serial1.available() <= 0)
        {
                delay(10);  
        }
        delay(20);
	while (Serial1.available() > 0)
        {
		// read what the LRF sends back 
		lrfData[offset] = Serial1.read();
		if(lrfData[offset] == ':' || lrfData[offset] == '?')
		{
			lrfData[offset] = 0; // null terminate the string of bytes and break out of the loop
			break;
		}
		offset++;
	}
	if(offset > 0)
	{ 
		//integer to convert lrfData string to an integer value
		int distance = atoi(&lrfData[4]);
		return distance;
	}
	return -1;
}
