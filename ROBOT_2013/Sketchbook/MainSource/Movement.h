#ifndef _MOVEMENT_H_
#define _MOVEMENT_H_

#include "common.h"
#include "Servo.h"
#include "Timer.h"

#define TURN_90_TIME 820

class Movement {
public:
		void init();
		void slideWall(side s);
		void slideLeft(float speed);
		void slideRight(float speed);
		void turnLeft(float speed);
		void turnRight(float speed);
		void forward(float speed);
		void backward(float speed);
		void stop();
		int turn90(side s = LEFT);
		int turnAround(side s = LEFT);
		bool openClaw(side s);
		bool closeClaw(side s);
		bool extendClaw(side s);
		bool retractClaw(side s);
    //Used for complex movements
		bool setSpeed(float speedFL,float speedFR, float speedBL, float speedBR);
		void liftUp();
		void setDown();
		bool backOffWall();
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
