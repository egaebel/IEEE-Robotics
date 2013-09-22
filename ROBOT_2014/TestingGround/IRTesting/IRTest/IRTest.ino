#include "Arduino.h"
#include "IRLib.h"

IRAverager ir;

void setup() {
  
    Serial.begin(9600);
    Serial.println("Serial has started\n");    
}

void loop() {

    val = Serial.analogRead(0);
    Serial.print("The val is:: ");
    Serial.println(val);
}

