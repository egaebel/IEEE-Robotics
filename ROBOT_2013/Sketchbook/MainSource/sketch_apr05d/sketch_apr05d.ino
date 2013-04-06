#include "Servo.h"

Servo s;

void setup(){
  s.attach(6);
  
}

void loop(){
  s.write(90);
}
