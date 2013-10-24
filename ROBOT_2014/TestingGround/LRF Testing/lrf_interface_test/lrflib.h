#ifndef LRFLIB_H_
#define LRFLIB_H_

#include <Arduino.h>

class LRFAverager
{
	public:
		LRFAverager();
                void initialize();
		int get10ReadingAverage();
		int getSingleReading();
};

#endif
