#ifndef _MOVEMENT_H_
#define _MOVEMENT_H_

#include <Servo.h>

class Movement {
private:
	Servo leftMotor;
	Servo rightMotor;
	Servo rearLeftMotor;
    Servo rearRightMotor;
    Servo topMotor;
	void setSpeed(Servo motor, float speed, bool inverted){
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

	void goToDeg(int d){
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
	
public:
	void liftUp() {
		goToDeg(90);
	}

	void setDown() {
		goToDeg(180);
	}

	void init(){
		leftMotor.attach(LEFT_MOTOR);
		rightMotor.attach(RIGHT_MOTOR);
		rearLeftMotor.attach(REAR_MOTOR_L);
	    rearRightMotor.attach(REAR_MOTOR_R);
	    topMotor.attach(TOP_MOTOR);
	}

	void slideLeft(float speed){
		//set rear motor left by speed
		setSpeed(REAR_MOTOR_L,-speed);
	    setSpeed(REAR_MOTOR_R,-speed);
	}

	void slideRight(float speed){
		//set rear motor right by speed
		setSpeed(REAR_MOTOR_L,speed);
		setSpeed(REAR_MOTOR_R,speed);
	}

	void turnLeft(float speed){
		//set left motor forward by speed
		setSpeed(LEFT_MOTOR,speed);
		//set right motor backward by speed
		setSpeed(RIGHT_MOTOR,-speed);
	}

	void turnRight(float speed){
		//set left motor backward by speed
		setSpeed(LEFT_MOTOR,-speed);
		//set right motor forward by speed
		setSpeed(RIGHT_MOTOR,speed);
	}

	void forward(float speed){
		//set both motors forward by speed
		setSpeed(LEFT_MOTOR,speed);
		setSpeed(RIGHT_MOTOR,speed);
	}

	void backward(float speed){
		//set both motors backward by speed
		setSpeed(LEFT_MOTOR,-speed);
		setSpeed(RIGHT_MOTOR,-speed);
	}

	void stop(){
		//stop all motors
		setSpeed(LEFT_MOTOR,0);
		setSpeed(RIGHT_MOTOR,0);
	    setSpeed(REAR_MOTOR_L,0);
	    setSpeed(REAR_MOTOR_R,0);
	}

	//TODO: Need to find speeds necessary to turn around
	void turnAround() {
		this->backward(0.25);
		this->turnLeft(0.25);
	}

	bool setSpeed(int servo, float speed){
		switch(servo){
			case LEFT_MOTOR:
				setSpeed(leftMotor,speed,false);
				break;
			case RIGHT_MOTOR:
				setSpeed(rightMotor,speed,true);
				break;
	                case REAR_MOTOR_L:
	                case REAR_MOTOR_R:
	                	setSpeed(rearLeftMotor,speed,false);
	                        setSpeed(rearRightMotor,speed,false);
				break;
			default:
				//opps
				return false;
				break;
		}
		return true;
	}
};

#endif