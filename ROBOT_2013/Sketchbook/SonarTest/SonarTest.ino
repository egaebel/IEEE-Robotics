#include <Arduino.h>
#include <Wire.h>
#include "Sonar.h"

Sonar sensor;

void setup(){
  Serial.begin(9600);
  Wire.begin();
  //TWBR=25000L;
  //sensor.changeAddress(0x70,SONAR_LEFT);
}

void loop(){;
    sensor.read();
    Serial.print("Left: 0x");
    Serial.println(sensor.getLeftDistance(),HEX);
    Serial.print("Right: 0x");
    Serial.println(sensor.getRightDistance(),HEX);
}
