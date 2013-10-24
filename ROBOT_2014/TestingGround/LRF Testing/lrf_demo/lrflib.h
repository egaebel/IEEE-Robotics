#ifndef LRFLIB_H_
#define LRFLIB_H_

#include <Arduino.h>

class LRFAverager
{
	public:
		LRFAverager();
		int get10ReadingAverage();
		int getSingleReading();
};

#endif