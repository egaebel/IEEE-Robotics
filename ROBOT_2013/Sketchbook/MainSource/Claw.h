#ifndef _CLAW_H_
#define _CLAW_H_

#include "common.h"
#include <Servo.h>

class Claw{

	private:
		Servo EXTEND_SERVO;
		Servo GRAB_SERVO;
	public:
		void init(int extend_servo_pin, int grab_servo_pin){
			EXTEND_SERVO.attach(extend_servo_pin);
			GRAB_SERVO.attach(grab_servo_pin);
		}

		void extend(){
			EXTEND_SERVO.write(180);
		}

		void retract(){
			EXTEND_SERVO.write(0);
		}

		void grab(){
			GRAB_SERVO.write(0);
		}

		void drop(){
			GRAB_SERVO.write(180);
		}
};

#endif