#include "Servo.h"


void goToDeg(int d);

Servo servo;

void setup(){
 servo.attach(13);
  
  
}

void loop(){
  if(digitalRead(53)){
    goToDeg(180);
  }
  else{
    goToDeg(0);
  }
  delay(10);
}

void goToDeg(int d){
  static int curD = 0;

  if(curD<d){
    servo.write(curD);
    curD++;
  }
  else if(curD>d){
    servo.write(curD);
    curD--;
  }
  
  
}
