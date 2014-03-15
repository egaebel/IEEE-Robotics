#ifndef PIN_DEFINITIONS_H_
#define PIN_DEFINITIONS_H_

//Motor Pins---------------------------------------
//Motor Pair 1 (ON THE LEFT SIDE)
static const unsigned short PIN_DIRECTION_LEFT = 26; //Direction
static const unsigned short PIN_PWM_LEFT = 3; //Speed
//Motor Pair 2 (ON THE RIGHT SIDE)
static const unsigned short PIN_DIRECTION_RIGHT = 28; //Direction
static const unsigned short PIN_PWM_RIGHT = 4; //Speed
static const unsigned int DEFAULT_SPEED = 255;

//FRONT Line Sensor Pins-------------------------------------------------------------------------
static const unsigned short PIN_SENSOR_FRONT = 22; // Controls SerialLineFollower's Enable pin
static const unsigned short PIN_LF_FRONT_S0 = 44; //44
static const unsigned short PIN_LF_FRONT_S1 = 46; //46
static const unsigned short PIN_LF_FRONT_S2 = 48; //48
static const unsigned short PIN_LF_FRONT_S3 = 50; //50
static const unsigned short PIN_LF_FRONT_S4 = 32; //32
static const unsigned short PIN_LF_FRONT_S5 = 34; //34
static const unsigned short PIN_LF_FRONT_S6 = 36; //36
static const unsigned short PIN_LF_FRONT_S7 = 38; //38
//BACK Line Sensor Pins---------------------------------------------------------------------------
static const unsigned short PIN_SENSOR_BACK = 23; // Controls SerialLineFollower's Enable pin
static const unsigned short PIN_LF_BACK_S0 = 45; //45
static const unsigned short PIN_LF_BACK_S1 = 47; //47
static const unsigned short PIN_LF_BACK_S2 = 49; //49
static const unsigned short PIN_LF_BACK_S3 = 51; //51
static const unsigned short PIN_LF_BACK_S4 = 33; //33
static const unsigned short PIN_LF_BACK_S5 = 35; //35
static const unsigned short PIN_LF_BACK_S6 = 37; //37
static const unsigned short PIN_LF_BACK_S7 = 39; //39

//Color Sensor Pins-------------------------------
static const unsigned short PIN_CS_S0 = 24; // 24
static const unsigned short PIN_CS_S1 = 25; // 25
static const unsigned short PIN_CS_S2 = 27; // 27
static const unsigned short PIN_CS_S3 = 29; // 29
static const unsigned short PIN_CS_OUT = 30; // 30
static const unsigned short PIN_CS_LED = 31; // 31

//Start Button Pin--------------------------------
static const unsigned short PIN_START = 42;

//Firing Pins----------------------------------------
// output pin to notify BeagleBone that it should aim using the parameters of the next barrel, either 2 or 3
const int START_AIMING_PIN = 43;

// interrupt 0 is on digital pin 2 for Arduino Uno
const int READY_TO_FIRE_INTERRUPT = 0;
const int READY_TO_FIRE_PIN = 2;

// Firing Servo Pins
const int FIRING_SERVO_1_PIN = 13;
const int FIRING_SERVO_2_PIN = 12;
const int FIRING_SERVO_3_PIN = 11;

#endif