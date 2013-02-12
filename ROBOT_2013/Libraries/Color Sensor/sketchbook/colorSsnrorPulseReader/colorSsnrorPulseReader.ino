#include "BackEndColorSensor.h"
#include "Arduino.h"
BackEndColorSensor *backEnd;


void setup() {
    backEnd = &BackEndColorSensor();
    
}

void loop()	{
    Serial.begin(115200);
    Serial.print("\n\n\n\n\n");
	Serial.print("White");
	Serial.print(backEnd->colorRead(0));
        Serial.print("\n Blue");
        Serial.print(backEnd->colorRead(1));
        Serial.print("\n Red:");
        Serial.print(backEnd->colorRead(2));
        Serial.print("\n Green:");
        Serial.print(backEnd->colorRead(3));
   delay(2000);
}
