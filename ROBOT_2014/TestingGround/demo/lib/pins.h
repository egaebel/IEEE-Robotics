#ifndef PIN_DEFINITIONS_H_
#define PIN_DEFINITIONS_H_

//Pin assignments--------------------------
/*
//Motor Pair 1 (ON THE LEFT SIDE)
#define PIN_DIRECTION_LEFT 8 //Direction
#define PIN_PWM_LEFT 3 //Speed
//Motor Pair 2 (ON THE RIGHT SIDE)
#define PIN_DIRECTION_RIGHT 9 //Direction
#define PIN_PWM_RIGHT 4 //Speed
*/

//Line Sensor Pins
#define Load 22 // Controls ShiftRegister's Shift/Load pin(1)
#define sensor 24 // Controls LineFollower's Enable pin
#define L_side_pin 10 // PWM pin for left trek
#define R_side_pin 9  // PWM pin for right trek
#define Dir_Right_Side 30 // Controls direction of the right track(LOW - forward, HIGH - Reverse)
#define Dir_Left_Side 31 // Controls direction of the left track (LOW - forward, HIGH - Reverse)

//Color Sensor pins
#define CS_S0 31
#define CS_S1 33
#define CS_S2 37
#define CS_S3 39
#define CS_OUT 41
#define CS_LED 43

#endif
