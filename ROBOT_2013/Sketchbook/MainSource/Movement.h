#ifndef _MOVEMENT_H_
#define _MOVEMENT_H_

#include "common.h"
#include "Servo.h"
#include "Timer.h"
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
		void turnAround();
		void openClaw(side s);
		void closeClaw(side s);
    	//Used for complex movements
		bool setSpeed(float speedFL,float speedFR, float speedBL, float speedBR);
		void liftUp();
		void setDown();
private:
		bool setSpeed(int servo, float speed);

		Servo leftMotor;
		Servo rightMotor;
		Servo backLeftMotor;
    	Servo backRightMotor;
	    Servo topMotor;

	    Servo rightClawMotor;
	    Servo rightExtendMotor;

	    void setSpeed(Servo motor, float speed, bool inverted);
	    bool goToDeg(Servo motor,int d);
};

#endif