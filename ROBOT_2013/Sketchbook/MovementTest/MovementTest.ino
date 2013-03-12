
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
  
  move.forward(1);
  delay(5000);
  move.stop();
  move.backward(1);
  delay(5000);
    move.stop();
  move.turnLeft(1);
  delay(5000);
    move.stop();
  move.turnRight(1);
  delay(5000);
    move.stop();
  move.slideLeft(1);
  delay(5000);
    move.stop();
  move.slideRight(1);
  delay(5000);
<<<<<<< Updated upstream
  */
  move.liftUp();
  delay(5000);
  move.liftStop();
  delay(5000);
  move.liftDown();
  delay(5000);
=======
    move.stop();
>>>>>>> Stashed changes
}
