#ifndef _CLAW_H_
#define _CLAW_H_

#include "common.h"
#include <Servo.h>

class Claw{

	public:
		void init(int extend_servo_pin, int grab_servo_pin);
		void extend();
		void retract();
		void grab();
		void drop();
	private:
		Servo EXTEND_SERVO;
		Servo GRAB_SERVO;
};

void Claw::init(int extend_servo_pin, int grab_servo_pin){
	EXTEND_SERVO.attach(extend_servo_pin);
	GRAB_SERVO.attach(grab_servo_pin);
}

void Claw::extend(){
	EXTEND_SERVO.write(180);
}

void Claw::retract(){
	EXTEND_SERVO.write(0);
}

void Claw::grab(){
	GRAB_SERVO.write(0);
}

void Claw::drop(){
	GRAB_SERVO.write(180);
}


#endif