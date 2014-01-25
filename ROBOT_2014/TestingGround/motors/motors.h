


class motors
{
private:

/*
Sample Code to run the Sparkfun TB6612FNG 1A Dual Motor Driver using Arduino UNO R3

This code conducts a few simple manoeuvres to illustrate the functions:
  - motorDrive(motorNumber, motorDirection, motorSpeed)
  - motorBrake(motorNumber)
  - motorStop(motorNumber)
  - motorsStandby

Connections:
- Pin 3 ---> PWMA
- Pin 8 ---> AIN2
- Pin 9 ---> AIN1
- Pin 10 ---> STBY
- Pin 11 ---> BIN1
- Pin 12 ---> BIN2
- Pin 5 ---> PWMB

- Motor 1: A01 and A02
- Motor 2: B01 and B02

*/

//Define the Pins

//Motor 1
#define pinAIN1 9; //Direction
#define pinAIN2 8; //Direction
#define pinPWMA 3; //Speed

//Motor 2
#define pinBIN1 11; //Direction
#define pinBIN2 12; //Direction
#define pinPWMB 5; //Speed

//Standby
#define pinSTBY 10;

//Constants to help remember the parameters
#define turnCW false;  //for motorDrive function
#define turnCCW true; //for motorDrive function
#define motor1 false;  //for motorDrive, motorStop, motorBrake functions
#define motor2 true;  //for motorDrive, motorStop, motorBrake functions
public:
	void setup();
	void motorDrive(boolean motorNumber, boolean motorDirection);
	void motorBrake(boolean motorNumber);
	void motorStop(boolean motorNumber);
	void motorsStandby();
	int speed;
};
