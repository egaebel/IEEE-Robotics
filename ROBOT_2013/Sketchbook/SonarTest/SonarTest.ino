#include <Arduino.h>
#include <Wire.h>
#include "common.h"
#include "Sonar.h"

Sonar left(SONAR_LEFT,SONAR_LEFT_INT);
Sonar right(SONAR_RIGHT,SONAR_RIGHT_INT);


void handleSonarLeft(){
  left.setDataReady(true);
}

void handleSonarRight(){
  right.setDataReady(false);
}

void setup(){
  Serial.begin(9600);
  Wire.begin();
  delay(100); //Make sure we don't catch the PWM from POR of Sonar
  attachInterrupt(SONAR_LEFT_INT,handleSonarLeft,FALLING);
  attachInterrupt(SONAR_RIGHT_INT,handleSonarRight,FALLING);
  
  //TWBR=25000L; //Set I2C clock to 25k Hz
  //sensor.changeAddress(0x70,SONAR_LEFT); //Set address of Sonar with address 0x70 to SONAR_LEFT
}

void loop(){;
    left.update();
    right.update();
  
    Serial.print("Left:  0x");
    Serial.println(left.getDistance(),HEX);
    Serial.print("Right: 0x");
    Serial.println(right.getDistance(),HEX);
}
