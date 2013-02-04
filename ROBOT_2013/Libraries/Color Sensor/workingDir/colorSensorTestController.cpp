#include "ColorSensor.h"

ColorSensor sensor;





void setup() {
    sensor = new ColorSensor();
}

void loop()	{
    int result = sensor.detectColor();
    Serial.begin(115200);
    Serial.print("\n\n\ANOTHER:  ");
	Serial.print("result: ");
	Serial.print(result);
}






