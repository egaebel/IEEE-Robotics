#include "Arduino.h"
#include "motors.h"

motors m1;
void setup()
{
  m1.speed = 255;  
}

void loop()
{
 m1.motorDrive(motor1, turnCW); 
}
