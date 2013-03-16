#ifndef _MOVEMENT_H_
#define _MOVEMENT_H_

#include "common.h"
#include "Servo.h"
class Movement {
public:
	void init(); //Setup all motors
        void dropDown();
	void slideLeft(float speed);
	void slideRight(float speed);
	void turnLeft(float speed);
	void turnRight(float speed);
	void forward(float speed);
	void backward(float speed);
        void stop();
	bool setSpeed(int servo, float speed);
private:
	Servo leftMotor;
	Servo rightMotor;
	Servo rearLeftMotor;
        Servo rearRightMotor;
        Servo topMotor;
	void setSpeed(Servo motor, float speed, bool inverted);
};

#endif
