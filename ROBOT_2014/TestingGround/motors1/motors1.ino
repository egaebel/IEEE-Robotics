#include "Arduino.h"
#include "motors.h"

motors m1;
void setup()
{
	Serial.begin(9600);
	m1.setup(255);
    delay(2);
}

void loop()
{
	m1.motorsDrive(FORWARD); 
	Serial.write("Going.....\n");
	delay(10000);
	m1.motorsDrive(BACKWARD);
	delay(10000);
}
