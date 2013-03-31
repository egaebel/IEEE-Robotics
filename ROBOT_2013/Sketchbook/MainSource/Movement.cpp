#include "Movement.h"


void Movement::init(){
	leftMotor.attach(MOTOR_FRONT_L);
	rightMotor.attach(MOTOR_FRONT_R);
	backLeftMotor.attach(MOTOR_BACK_L);
    backRightMotor.attach(MOTOR_BACK_R);
    rightClawMotor.attach(RCLAW_SERVO);
    rightExtendMotor.attach(RCLAW_EXTEND_SERVO);
    rightExtendMotor.write(90);
    stop();
}

void Movement::turnAround() {
	this->backward(0.25);
	this->turnLeft(0.25);
}

void Movement::extendClaw(side s){
	rightExtendMotor.write(180);
}
bool Movement::retractClaw(side s){
	if(!(digitalRead(22))){
		rightExtendMotor.write(90);
		return false;
	}
	else{
		rightExtendMotor.write(0);
		return true;
	}

}

void Movement::slideLeft(float speed){
	setSpeed(0,0,speed,speed);    
}

void Movement::slideRight(float speed){
	setSpeed(0,0,-speed,-speed);    
}

void Movement::turnLeft(float speed){
	setSpeed(speed,-speed,0,0);	
}

void Movement::turnRight(float speed){
	setSpeed(-speed,speed,0,0);
}

void Movement::forward(float speed){
	//set both motors forward by speed
	setSpeed(speed,speed,0,0);
}

void Movement::backward(float speed){
	//set both motors backward by speed
	setSpeed(-speed,-speed,0,0);
}
void Movement::stop(){
	//stop all motors
	setSpeed(0,0,0,0);
}


bool Movement::setSpeed(float speedFL,float speedFR, float speedBL, float speedBR){
	setSpeed(MOTOR_FRONT_L,speedFL);
	setSpeed(MOTOR_FRONT_R,speedFR);
	setSpeed(MOTOR_BACK_L,speedBL);
	setSpeed(MOTOR_BACK_R,speedBR);
}

void Movement::liftUp() {
	goToDeg(topMotor,90);
}

void Movement::setDown() {
	goToDeg(topMotor,180);
}

void Movement::openClaw(side clawSide){
	if(clawSide==RIGHT)
		rightClawMotor.write(120);
}
void Movement::closeClaw(side clawSide){
	if(clawSide==RIGHT)
		rightClawMotor.write(90);
}
bool Movement::goToDeg(Servo motor, int d){
	static int curD = 0;
	static Timer t(100);
	t.start();
	if(t.isDone()){
		if(curD<d){		
			motor.write(curD);
			curD++;
		}
		else if(curD>d){
			motor.write(curD);
			curD--;
		}
		else {
			t.stop();
			return true;
		}
		t.reset();
	}
	return false;
}

bool Movement::setSpeed(int servo, float speed){
	switch(servo){
		case MOTOR_FRONT_L:
			setSpeed(leftMotor,speed,true);
			break;
		case MOTOR_FRONT_R:
			setSpeed(rightMotor,speed,false);
			break;
        case MOTOR_BACK_L:
            setSpeed(backLeftMotor,speed,false);
            break;
        case MOTOR_BACK_R:
            setSpeed(backRightMotor,speed,false);
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
