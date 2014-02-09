/* hubeeTest2.ino
 *
 * James Reed, CpE
 * Created 2/9/2014
 *
 * This file contains a working test for the HUBeeBMDWheel class.
 */

#include <HUBeeBMDWheel.h>
//****************Pin Assignments*****************
// HuBee wheel reference: http://www.creative-robotics.com/bmdsresources
// 
// Pin IN1 on hubee wheel. Digital pin 22
const int PIN_IN1 = 22;
//
// Pin IN2 on hubee wheel. Digital pin 24
const int PIN_IN2 = 24;
//
// PWM pin on hubee wheel. PWM pin 9
const int PIN_PWM = 9;

//***************Wire assignments ***************
//
// BLACK: GND
// WHITE: +V
// GREY: IN1
// PURPLE: IN2
// BLUE: PWM
// GREEN: Standby (active low - can be left disconnected)
// YELLOW/ORANGE: Quadrature sensor channels A and B (not used in this)
//
HUBeeBMDWheel Wheel; // create our wheel object

int Speed = 255; // static speed for testing purposes

signed char timeout= -255; // demonstration timeout

void setup()
{
  //setup our pins
  // setupPins(int In1Pin, int In2Pin, int PWMPin);
  Wheel.setupPins(PIN_IN1, PIN_IN2, PIN_PWM);
  // set direction mode to true to invert normal direction
  Wheel.setDirectionMode(false);
  
  Serial.begin(9600);
  
  // set motor power with signed integer -255 to 255
  Wheel.setMotorPower(Speed);
}

void loop()
{
   Serial.print("loop()\n");
   Serial.print(Wheel.getMotorPower());
   Serial.print("\n");
   
   //Drive wheels. Using timeout variable to switch direction
   Wheel.setMotorPower((timeout++ < 0) ? Speed : -Speed);

   delay(1); 
}
