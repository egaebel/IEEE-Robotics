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

//FRONT Line Sensor Pins---------------------------------------------------------------------------
//static const unsigned short PIN_LOAD = 22; // Controls ShiftRegister's Shift/Load pin(1)
static const unsigned short PIN_SENSOR_FRONT = 24; // Controls SerialLineFollower's Enable pin
//BACK Line Sensor Pins-------------------------------------------------------------------------
static const unsigned short PIN_SENSOR_BACK = 25; // Controls ParallelLineFollower's Enable pin
static const unsigned short PIN_LF_S0 = 45;
static const unsigned short PIN_LF_S1 = 47;
static const unsigned short PIN_LF_S2 = 49;
static const unsigned short PIN_LF_S3 = 51;
static const unsigned short PIN_LF_S4 = 30;
static const unsigned short PIN_LF_S5 = 32;
static const unsigned short PIN_LF_S6 = 34;
static const unsigned short PIN_LF_S7 = 36;

//Color Sensor Pins-------------------------------
static const unsigned short PIN_CS_S0 = 31;
static const unsigned short PIN_CS_S1 = 33;
static const unsigned short PIN_CS_S2 = 37;
static const unsigned short PIN_CS_S3 = 39;
static const unsigned short PIN_CS_OUT = 41;
static const unsigned short PIN_CS_LED = 43;

//Start Button Pin--------------------------------
static const unsigned short PIN_START = 42;

//Firing Pins----------------------------------------
// output pin to notify BeagleBone that it should aim using the parameters of the next barrel, either 2 or 3
const int AIM_NEXT_BARREL_PIN = 0;

// interrupt 0 is on digital pin 2 for Arduino Uno
const int READY_TO_FIRE_INTERRUPT = 0;
const int READY_TO_FIRE_PIN = 2;

// Firing Servo Pins
const int FIRING_SERVO_1_PIN = 13;
const int FIRING_SERVO_2_PIN = 14;
const int FIRING_SERVO_3_PIN = 15;

#endif
