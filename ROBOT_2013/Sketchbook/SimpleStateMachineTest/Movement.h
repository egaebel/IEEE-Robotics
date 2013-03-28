#ifndef _MOVEMENT_H_
#define _MOVEMENT_H_

#include "common.h"
#include "Servo.h"
class Movement {
public:
	void init(); //Setup all motors


    void dropDown();

    //simple movements
	void slideLeft(float speed);
	void slideRight(float speed);
	void turnLeft(float speed);
	void turnRight(float speed);
	void forward(float speed);
	void backward(float speed);
    void stop();

    //Used for complex movements
	bool setSpeed(float speedFL,float speedFR, float speedBL, float speedBR);
private:
	bool setSpeed(int servo, float speed);
	Servo leftMotor;
	Servo rightMotor;
	Servo backLeftMotor;
    Servo backRightMotor;
    Servo topMotor;
	void setSpeed(Servo motor, float speed, bool inverted);
};

#endif
