#include <Arduino.h>
#include "BackEndColorSensor.h"
BackEndColorSensor colorSensor;

void setup()
{
  colorSensor.TCS3200setup();
  Serial.begin(9600);
}

void loop()
{
        Serial.print("White:  ");
        Serial.print(colorSensor.detectColor(0));
        Serial.print("  ");
        Serial.print("Blue:  ");
        Serial.print(colorSensor.detectColor(1));
        Serial.print("  ");
        Serial.print("Red:  ");
        Serial.print(colorSensor.detectColor(2));
        Serial.print("  ");
        Serial.print("Green:  ");
        Serial.print(colorSensor.detectColor(3));
        Serial.print("\n");
        delay(500);
}
