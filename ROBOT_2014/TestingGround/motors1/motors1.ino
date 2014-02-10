#include "Arduino.h"
#include "motors.h"

static int state = 0;
static motors m1;
void setup()
{
	Serial.begin(9600);
	m1.setup(255);
    delay(2);
}

void loop()
{
	switch (state) {
		case 0:
			Serial.write("CASE 0\n");
			//m1.motorsDrive(FORWARD);
			//delay(6000);
			state++;
			break;
		case 1:
			//Serial.write("CASE 1\n");
			//m1.motorsStop();
			//delay(6000);
			state++;
			break;
		case 2:
			Serial.write("CASE 2\n");
			m1.motorsTurnLeft();
			delay(6000);
			state++;
			break;
		case 3:
			Serial.write("CASE 3\n");
			//m1.motorsDrive(FORWARD);
			//delay(6000);
			state++;
			break;
		case 4:
			Serial.write("CASE 4\n");
			m1.motorsStop();
			delay(6000);
			state++;
			break;
		case 5:
			Serial.write("CASE 5\n");
			m1.motorsTurnRight();
			delay(6000);
			state++;
			break;
		case 6:
			return;
	}
}
