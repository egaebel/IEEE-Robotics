#include "Arduino.h"
#include "LRFLib.h"

LRFAverager lrf;

void setup()
{
  Serial.begin(9600);
  lrf.initialize();
}

void loop()
{
  int distance = lrf.getSingleReading();
  Serial.print("Distance: ");
  Serial.println(distance);
}
