#ifndef PIN_DEFINITIONS_H_
#define PIN_DEFINITIONS_H_

//Pin assignments--------------------------

//Motor Pair 1 (ON THE LEFT SIDE)
static const unsigned short PIN_DIRECTION_LEFT = 26; //Direction
static const unsigned short PIN_PWM_LEFT = 3; //Speed
//Motor Pair 2 (ON THE RIGHT SIDE)
static const unsigned short PIN_DIRECTION_RIGHT = 28; //Direction
static const unsigned short PIN_PWM_RIGHT = 4; //Speed
static const unsigned int DEFAULT_SPEED = 255;

//Line Sensor Pins
static const unsigned short PIN_LOAD = 22; // Controls ShiftRegister's Shift/Load pin(1)
static const unsigned short PIN_SENSOR = 24; // Controls LineFollower's Enable pin

#endif
