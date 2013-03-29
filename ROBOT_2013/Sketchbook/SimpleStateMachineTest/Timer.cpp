#include "Timer.h"

Timer::Timer(int end) {
	endDuration = end;
	endTime = millis() + endDuration;
	started = false;
}

bool Timer::isDone(){
	if(started&&(millis()>endTime)){
		return true;
	} 
	return false;
}
void Timer::stop(){
	started = false;
}
void Timer::start(){
	if(!started){
		reset();
	}
}
void Timer::reset(){
	started = true;

	endTime	= millis() + endDuration;
}