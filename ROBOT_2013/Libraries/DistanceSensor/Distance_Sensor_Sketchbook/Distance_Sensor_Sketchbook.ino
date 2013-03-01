#include "DistanceSensor.h"
#include "Arduino.h"

DistanceSensor distanceSensor;

void setup() {
	Serial.begin(9600);
}

void loop()
{
	//return distanceSensor.findDistance();
	Serial.print("\nDistance: ");
	Serial.print(distanceSensor.findDistance());
}

