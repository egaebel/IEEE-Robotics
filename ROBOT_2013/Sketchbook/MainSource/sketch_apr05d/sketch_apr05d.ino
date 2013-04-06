#include "Servo.h"

Servo s;
Servo s1;

void setup(){
  Serial.begin(9600);
  s.attach(4); 
 // s1.attach(7);
}
//left out
//left in

void loop(){
  for(int i=0;i<180;i++){
    Serial.println(i);
    s.write(i);
    delay(200);
  }
}
