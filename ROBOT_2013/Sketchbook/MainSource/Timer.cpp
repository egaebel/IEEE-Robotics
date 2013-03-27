#include "Timer.h"

Timer::Timer() {

	time = millis();
}

void Timer::reset() {

	time = millis();
}

int Timer::getTime() {

	return millis() - time;
}