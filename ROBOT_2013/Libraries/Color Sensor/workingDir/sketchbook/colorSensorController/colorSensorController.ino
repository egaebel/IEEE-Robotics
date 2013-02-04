#include "ColorSensor.h"
#include "Arduino.h"

ColorSensor *sensor;


void setup() {
    sensor = &ColorSensor();
}

void loop()	{
    int result = sensor->detectColor();
    Serial.begin(115200);
    Serial.print("\n\n\ANOTHER:  ");
	Serial.print("result: ");
	Serial.print(result);
}
