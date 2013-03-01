#include "DistanceSensor.h"
#include "common.h"
#include "Arduino.h"

/**
 * Returns the number of centimeters <int> of the closest object.
 * If returns if distance-to-object < 20cm (returns 20)
 */
int DistanceSensor::findDistance()  { 
	long duration;
	int distanceCM;
 
 
	pinMode(DISTANCE_SENSOR_PIN_OUT, OUTPUT); 
  
	digitalWrite(DISTANCE_SENSOR_PIN_OUT, LOW);
	delayMicroseconds(2);
	digitalWrite(DISTANCE_SENSOR_PIN_OUT, HIGH);
	delayMicroseconds(5);
	digitalWrite(DISTANCE_SENSOR_PIN_OUT, LOW);

	pinMode(DISTANCE_SENSOR_PIN_OUT, INPUT);
	duration = pulseIn(DISTANCE_SENSOR_PIN_OUT, HIGH);

	return microsecondsToCentimeters(duration);
}
 
 /**
  * Converts microseconds -> centimeters. (using speed of sound = 340 m/s = 29 microseconds/cm)
  */
 int DistanceSensor::microsecondsToCentimeters(long time)  {
	 return (int)((time / 29) / 2);
 }
