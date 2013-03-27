
//Sketch for testing the movement class

#include <Arduino.h>
#include <Servo.h>
#include "common.h"
#include "Movement.h"

Movement move;

void setup(){
  Serial.begin(9600);
  move.init();
}

void loop(){
  if( move.turnAround() == 1){
     Serial.println("Turned Around");
  }
}
