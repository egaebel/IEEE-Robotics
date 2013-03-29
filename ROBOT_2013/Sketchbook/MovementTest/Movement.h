#ifndef _MOVEMENT_H_
#define _MOVEMENT_H_

#include <Servo.h>
#include "common.h"

#define TURN_90_TIME 3000 /* milliseconds */

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
		int turnAround();
		int turn90(int left);
		bool setSpeed(int servo, float speed);
		void liftUp();
		void setDown();
	private:
	    unsigned long time;
	    Servo leftMotor;
            Servo rightMotor;
	    Servo rearLeftMotor;
	    Servo rearRightMotor;
	    Servo topMotor;
	    void setSpeed(Servo motor, float speed, bool inverted);
	    void goToDeg(int d);
	    
};

#endif
