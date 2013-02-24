
//Sketch for testing the movement class

#include <Arduino.h>
#include <Servo.h>
#include "common.h"
#include "Movement.h"

Movement move;

void setup(){
  move.init();
}

void loop(){
  /*
  move.forward(1);
  delay(5000);
  move.backward(1);
  delay(5000);
  move.turnLeft(1);
  delay(5000);
  move.turnRight(1);
  move.stop();
  delay(5000);
  move.slideLeft(1);
  delay(5000);
  move.slideRight(1);
  delay(5000);
  */
  move.lift();
  delay(5000);
  move.drop();
  delay(5000);
}
