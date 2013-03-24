#ifndef _MOVEMENT_H_
#define _MOVEMENT_H_

#include <Servo.h>
#include "common.h"

//modded, cus i canz
class Movement {
	public:
		void init();
		void slideLeft(float speed);
		void slideRight(float speed);
		void turnLeft(float speed);
		void turnRight(float speed);
		void forward(float speed);
		void backward(float speed);
		void stop();
		//TODO: Need to find speeds necessary to turn around
		void turnAround();
		bool setSpeed(int servo, float speed);
		void liftUp();
		void setDown();
	private:
		Servo leftMotor;
		Servo rightMotor;
		Servo rearLeftMotor;
	    Servo rearRightMotor;
	    Servo topMotor;
	    void setSpeed(Servo motor, float speed, bool inverted);
	    void goToDeg(int d);
};

#endif