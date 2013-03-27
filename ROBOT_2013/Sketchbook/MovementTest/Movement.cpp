#include "Movement.h"
#include "Arduino.h"

void Movement::init(){
	leftMotor.attach(LEFT_MOTOR);
	rightMotor.attach(RIGHT_MOTOR);
	rearLeftMotor.attach(REAR_MOTOR_L);
    rearRightMotor.attach(REAR_MOTOR_R);
    stop();
}

int Movement::turn90(int left){
    static int state;
    switch( state ){
        case 0: //Setup
        {
            this->time = millis() + TURN_90_TIME;
            state++;
            if(left){
                turnLeft(1);
            }else{
                turnRight(1);
            }
            break;
        }
        case 1: //Run
        {
            if( this->time < millis() ){
                state++;
                stop();
                state = 0;
                return 1;
            }
            break;
        }
    }
    return 0;
}

int Movement::turnAround() {
    static int step;
    switch( step ){
        case 0:
	        if(turn90(0)){
	            step++;
	        }
	        break;
	    case 1:
	        if(turn90(0)){
	            step=0;
	            return 1;
	        }
	        break;
	}
	return 0;
}

void Movement::slideLeft(float speed){
	//set rear motor left by speed
	setSpeed(REAR_MOTOR_L,speed);
	setSpeed(REAR_MOTOR_R,speed);
}

void Movement::slideRight(float speed){
	//set rear motor right by speed
	setSpeed(REAR_MOTOR_L,-speed);
    setSpeed(REAR_MOTOR_R,-speed);
}

void Movement::turnLeft(float speed){
	//set left motor forward by speed
	setSpeed(LEFT_MOTOR,speed);
	//set right motor backward by speed
	setSpeed(RIGHT_MOTOR,-speed);
}

void Movement::turnRight(float speed){
	//set left motor backward by speed
	setSpeed(LEFT_MOTOR,-speed);
	//set right motor forward by speed
	setSpeed(RIGHT_MOTOR,speed);
}

void Movement::forward(float speed){
	//set both motors forward by speed
	setSpeed(LEFT_MOTOR,speed);
	setSpeed(RIGHT_MOTOR,speed);
}

void Movement::backward(float speed){
	//set both motors backward by speed
	setSpeed(LEFT_MOTOR,-speed);
	setSpeed(RIGHT_MOTOR,-speed);
}

void Movement::stop(){
	//stop all motors
	setSpeed(LEFT_MOTOR,0);
	setSpeed(RIGHT_MOTOR,0);
    setSpeed(REAR_MOTOR_L,0);
    setSpeed(REAR_MOTOR_R,0);
}

void Movement::liftUp() {
	goToDeg(90);
}

void Movement::setDown() {
	goToDeg(180);
}

void Movement::goToDeg(int d){
	static int curD = 0;

	if(curD<d){
		topMotor.write(curD);
		curD++;
	}
	else if(curD>d){
		topMotor.write(curD);
		curD--;
	}
}

bool Movement::setSpeed(int servo, float speed){
	switch(servo){
		case LEFT_MOTOR:
			setSpeed(leftMotor,speed,false);
			break;
		case RIGHT_MOTOR:
			setSpeed(rightMotor,speed,true);
			break;
        case REAR_MOTOR_L:
            setSpeed(rearLeftMotor,speed,false);
            break;
        case REAR_MOTOR_R:
            setSpeed(rearRightMotor,speed,false);
			break;
		default:
			//opps
			return false;
			break;
	}
	return true;
}

void Movement::setSpeed(Servo motor, float speed, bool inverted){
	if( speed < -1.0f || speed > 1.0f ){
		//Invalid magnitude, set to neutral
		speed = 0;
	}

	if( inverted == true ){
		//invert magnitude
		speed = -speed;
	}

	//Adjust speed to correct range
	speed = (90*speed)+90; //0-180

	motor.write(speed); //set speed
}
