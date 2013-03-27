#ifndef TIMER_H_
#define TIMER_H_

#include <Arduino.h>

class Timer {

	public:
		Timer();
		void reset();
		int getTime();	
	private:
		int time;	
};

#endif