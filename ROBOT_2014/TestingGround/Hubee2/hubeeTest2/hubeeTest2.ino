/* hubeeTest2.ino
 *
 * James Reed, CpE
 * Created 2/9/2014
 *
 * This file contains a working test for the HUBeeBMDwheel class.
 */

#include <Hubee.h>
#include <Arduino.h>
//****************Pin Assignments*****************
// HuBee wheel reference: http://www.creative-robotics.com/bmdsresources
// 
// Pin IN1 on hubee wheel. Digital pin 22
static const unsigned int PIN_IN1 = 22;
//
// Pin IN2 on hubee wheel. Digital pin 24
static const unsigned int PIN_IN2 = 24;
//
// PWM pin on hubee wheel. PWM pin 9
static const unsigned int PIN_PWM = 9;

//Standby pin on hubee
static const unsigned int PIN_STANDBY = 26;

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
Hubee wheel; // create our wheel object

int Speed = 255; // static speed for testing purposes

signed int timeout= -254; // demonstration timeout

void setup()
{

  Serial.begin(9600);
  
  //setup our pins and motor Power
  wheel.setup(PIN_IN1, PIN_IN2, PIN_PWM, 255);
}

void loop()
{
   Serial.print("loop()\n");
   //Serial.print(wheel.getMotorPower());
   Serial.print("\n");

   //Definitely works...
   //digitalWrite(PIN_IN1, LOW);
   //digitalWrite(PIN_IN2, HIGH);
   //analogWrite(PIN_PWM, 255);
   //------------------------------
   
   wheel.forward();
   delay(4000);
   wheel.stop();
   delay(1000);
   wheel.backward();
   delay(4000);
   wheel.stop();
   delay(1000);
}
