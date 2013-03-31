
//Sketch for testing the movement class

#include <Arduino.h>
#include <Servo.h>
#include "common.h"
#include "Movement.h"

Movement move;
bool run = true;

void setup(){
  Serial.begin(9600);
  move.init();
}

void loop(){
  if( run ){
    if( move.turnAround() == 1){
      run = false;
      Serial.println("Turned 90");
    }
  }
}
