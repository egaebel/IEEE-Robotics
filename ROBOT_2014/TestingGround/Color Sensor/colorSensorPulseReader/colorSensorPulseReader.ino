#include "ColorSensor.h"
#include "Arduino.h"
ColorSensor colorSensor;

void setup() {
    Serial.begin(9600);
    colorSensor.setup(22, 26, 30, 34, 38, 42);
}

void loop()	{
    
    Serial.print("\n\n\n\n\n");

    if (colorSensor.colorRead(WHITE)) {
        Serial.println("WHITE");
    }
    else if (colorSensor.colorRead(GREEN)) {
        Serial.println("GREeEn");
    }
    else if (colorSensor.colorRead(BLUE)) {

        Serial.println("BLUE!");
    }
    else if (colorSensor.colorRead(RED)) {
        Serial.println("RED");
    }
    else {
        Serial.println("NOT BLUE!");
    }
}