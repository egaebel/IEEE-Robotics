#include "Movement.h"


void Movement::init(){
	leftMotor.attach(MOTOR_FRONT_L);
	rightMotor.attach(MOTOR_FRONT_R);
	backLeftMotor.attach(MOTOR_BACK_L);
    backRightMotor.attach(MOTOR_BACK_R);
    //rightClawMotor.attach(RCLAW_SERVO);
    //rightExtendMotor.attach(RCLAW_EXTEND_SERVO);
    topMotor.attach(3);
    rightExtendMotor.write(90);
    stop();
    Serial.println("MOVEMENT INITED");
}

bool Movement::turn90(side s){
    static int state;
    static Timer timer(TURN_90_TIME);
    switch( state ){
        case 0: //Setup
        {
            timer.start();
            state++;
            if(s == LEFT){
                turnLeft(1);
            }else{
                turnRight(1);
            }
            break;
        }
        case 1: //Run
        {
            if( timer.isDone() ){
                stop();
                timer.stop();
                state = 0;       
                return true;
            }
            break;
        }
    }
    return false;
}

bool Movement::turnAround(side s) {
    static int step;
    switch( step ){
        case 0:
	        if(turn90(s)){
	            step++;
	        }
	        break;
	    case 1:
	        if(turn90(s)){
	            step=0;
	            return true;
	        }
	        break;
	}
	return false;
}

//Drops the block in the claw on side s
bool Movement::dropClaw(side s) {

	static int zeState = 0;
	switch (zeState) {
		case 0:
			if (extendClaw(s)) {
				zeState++;
			}
			break;
		case 1:
			if (openClaw(s)) {
				zeState;
			}
			break;
		case 2:
			if (retractClaw(s)) {
				zeState++;
			}
			break;
		case 3:
			if (closeClaw(s)) {
				zeState = 0;
				return true;
			}
			break;
	}

	return false;
}

//picks up a block in the claw on side s
bool Movement::pickupClaw(side s) {

	static int zeState = 0;
	switch (zeState) {
		case 0:
			if (openClaw(s)) {
				zeState++;
			}
			break;
		case 1:
			if (extendClaw(s)) {
				zeState++;
			}
			break;
		case 2:
			if (closeClaw(s)) {
				zeState++;
			}
			break;
		case 3:
			if (retractClaw(s)) {
				zeState = 0;
				return true;
			}
	}
	return false;
}

bool Movement::openClaw(side s) {

	static Timer time(OPEN_CLAW_TIME);
	if (time.isDone()) {
		getClawMotor(s)->write(0);
		return true;
	}
	else {
		getClawMotor(s)->write(-180);
		return false;
	}
}

bool Movement::closeClaw(side s) {

	static Timer time(CLOSE_CLAW_TIME);
	if (time.isDone()) {
		getClawMotor(s)->write(0);
		return true;
	}
	else {
		getClawMotor(s)->write(180);
		return false;
	}
}

bool Movement::extendClaw(side s){
	
	static Timer time(EXTEND_CLAW_TIME);
	if (time.isDone()) {
		getExtendMotor(s)->write(0);
		return true;
	}
	else {
		getExtendMotor(s)->write(180);
		return false;
	}
}

bool Movement::retractClaw(side s){
	
	if(!(digitalRead(22))){
		getClawMotor(s)->write(90);
		return false;
	}
	else{
		getClawMotor(s)->write(0);
		return true;
	}
}

Servo * Movement::getClawMotor(side s) {

	if (s == RIGHT) {
		return &rightClawMotor;
	}
	else {
		return &leftClawMotor;
	}
}

Servo * Movement::getExtendMotor(side s) {

	if (s == RIGHT) {
		return &rightExtendMotor;
	}
	else {
		return &leftExtendMotor;
	}
}

void Movement::slideLeft(float speed){
	setSpeed(0,0,speed,speed);    
}

void Movement::slideRight(float speed){
	setSpeed(0,0,-speed,-speed);    
}

void Movement::turnLeft(float speed){
	setSpeed(-speed,speed,0,0);	
}

void Movement::turnRight(float speed){
	setSpeed(speed,-speed,0,0);
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


void Movement::setSpeed(float speedFL,float speedFR, float speedBL, float speedBR){
	setSpeed(MOTOR_FRONT_L,speedFL);
	setSpeed(MOTOR_FRONT_R,speedFR);
	setSpeed(MOTOR_BACK_L,-speedBL);
	setSpeed(MOTOR_BACK_R,-speedBR);
}

bool Movement::liftUp() {
	return goToDeg(topMotor,27);
}

bool Movement::setDown() {
	return goToDeg(topMotor,0);
}

bool Movement::goToDeg(Servo motor, int d){
	static int curD = 0;
	static Timer t(100);
	Serial.println("GoToDeg");
	t.start();
	if(t.isDone()){
		Serial.println("Timer went off!");
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
void Movement::slideWall(side s){
	switch(s){
		case RIGHT:
			setSpeed(.25,0,-.1,-.1);
			break;
		case LEFT:
    		setSpeed(0,.25,.1,.1);
			break;
		default:

			break;
	}
}


bool Movement::backOffWall(){
	
	//static Timer timer(WALL_BACKUP_TIME);
	static Timer theTime(1000);

	if(!theTime.isStarted()){
		// leftMotor.detach();
		// rightMotor.detach();
		// leftMotor.attach(MOTOR_FRONT_L);
		// rightMotor.attach(MOTOR_FRONT_R);
		theTime.start();
	}
	if(theTime.isDone()) {
		stop();
		theTime.stop();
		theTime.reset();
		Serial.println("WE'RE DONE HERE GENTLEMEN&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&");
		return true;
	}
	else {
		Serial.println("HERHRHRP");
		//leftMotor.attach(MOTOR_FRONT_L);
		//rightMotor.attach(MOTOR_FRONT_R);
		this->backward(0.5);
		return false;
	}
}

bool Movement::timerTest(){
	static Timer timer(1000);
	timer.start();
	if(timer.isDone()){
		Serial.println("DING FRIES ARE DONE");
		timer.reset();
	}
}

/**
 * move foreward at speed: speed and for duration: time_ms
 */
void Movement::forwardForDuration(float speed, int time_ms) {
	static Timer timer(time_ms);
	
	if(!timer.isStarted()) {
		timer.start();
	}
	
	if(timer.isDone()) {
		stop();
	} else
	{
		setSpeed(speed,speed,0,0);
	}
}

/**
 * move backward at speed: speed and for duration: time_ms
 */
void Movement::backwardForDuration(float speed, int time_ms){
	static Timer timer(time_ms);
	setSpeed(-speed,-speed,0,0);
	if(!timer.isStarted()) {
		timer.start();
	}
	if(timer.isDone()) {
		stop();
	}
}

/**
 * Slide right at speed: speed and for duration: time_ms
 */
void Movement::slideRightForDuration(float speed, int time_ms){
	static Timer timer(time_ms);
	setSpeed(0,0,-speed,-speed);
	timer.start();
	if(timer.isDone()) {
		this->stop();
	}
}

/**
 * Slide left at speed: speed and for duration: time_ms
 */
void Movement::slideLeftForDuration(float speed, int time_ms){
	static Timer timer(time_ms);
	setSpeed(0,0,speed,speed);  
	timer.start();
	if(timer.isDone()) {
		this->stop();
	}
}





