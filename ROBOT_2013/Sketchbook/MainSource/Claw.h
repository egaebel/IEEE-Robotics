#ifndef _CLAW_H_
#define _CLAW_H_

#include "common.h"
#include "servo.h"

class Claw{

public:
	void init();
	bool extend();
	bool retract();
	void grab();
private:
};

#endif