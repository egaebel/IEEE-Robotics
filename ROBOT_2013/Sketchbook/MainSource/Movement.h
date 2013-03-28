#ifndef _MOVEMENT_H_
#define _MOVEMENT_H_

#include "common.h"
#include "Servo.h"
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
		void turnARound();
    //Used for complex movements
	bool setSpeed(float speedFL,float speedFR, float speedBL, float speedBR);
private:
		bool setSpeed(int servo, float speed);
		void liftUp();
		void setDown();
		Servo leftMotor;
		Servo rightMotor;
	Servo backLeftMotor;
    Servo backRightMotor;
	    Servo topMotor;
	    void setSpeed(Servo motor, float speed, bool inverted);
	    void goToDeg(int d);
};

#endif