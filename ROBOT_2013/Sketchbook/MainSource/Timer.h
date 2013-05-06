#ifndef TIMER_H_
#define TIMER_H_

#include <Arduino.h>

#define BACK_OFF_WALL_TIME 1000





/* This class lets you set an egg timer pretty much, so it will return true after X amount of time,
then you can just reset it*/
class Timer {

	public:
		Timer(int endDur);
		void init(int endDuration);
		bool isDone();
		//will start the timer, if not already started. So it's safe to keep calling
		void start();
		//stop the timer
		void stop();
		//will restart the timer regardless of time
		void reset();	
		bool isStarted();
	private:
		long endTime;
		int endDuration;
		bool started;
};

#endif