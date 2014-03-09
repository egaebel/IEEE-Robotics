#include "ColorSensor.h"
#include "Arduino.h"
static ColorSensor colorSensor;
static Color color;

void setup() {
    Serial.begin(9600);
    colorSensor.setup(22, 26, 
                        30, 34, 
                        38, 42);
}

void loop()	{
    
    color = colorSensor.getColor();
    switch(color) {

        case WHITE:
            Serial.println("WHITE");
            break;
        case BLUE:
            Serial.println("BLUE");
            break;
        case RED:
            Serial.println("RED");
            break;
        case GREEN:
            Serial.println("GREEN");
            break;
    }
    Serial.println("LOOP COMPLETE---------------\n\n\n\n\n");
    delay(3000);
}