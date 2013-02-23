#include <Arduino.h>
#include "LineSensor.h"

LineSensor sensor;
boolean lineResult[16];

void setup()
{
  Serial.begin(115200); //Sets the data transfer rate at 115.2 Kbps
  Serial.print("\n\n\nready\n\n\n\n\n\n");
}

void loop()
{
  sensor.getTotal(lineResult);
  
  for(int i=0;i<16;i++)
  {
    Serial.print(analogRead(i));
    Serial.print(", ");
  }
  
  Serial.println("");
  
  for(int i=0;i<16;i++)
  {
    Serial.print(lineResult[i]);
    Serial.print(", ");
  }
  
  Serial.println("");
  Serial.println("");
  
  delay(1000);
}
