#ifndef TIMER_H_
#define TIMER_H_

#include <Arduino.h>


/* This class lets you set an egg timer pretty much, so it will return true after X amount of time,
then you can just reset it*/
class Timer {

	public:
		Timer(int endDur);
		bool isDone();
		//will start the timer, if not already started. So it's safe to keep calling
		void start();
		//stop the timer
		void stop();
		//will restart the timer regardless of time
		void reset();	
	private:
		long endTime;
		int endDuration;
		bool started;
};

#endif