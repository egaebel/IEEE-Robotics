#include "ColorSensor.h"
#include "Arduino.h"

ColorSensor *sensor;
int result;


void setup() {
    sensor = &ColorSensor();
    Serial.begin(115200);
}

void loop()	{
    result = sensor->detectColor();
    
    Serial.print("\n\n\n\n\n\n");
    if(result == 0)  {
      Serial.println("RED");
    }
    
    else if(result == 1)  {
      Serial.println("BLUE");
    }
    
    else if(result == 2)  {
      Serial.println("BROWN");
    }
    
    else if(result == 3)  {
      Serial.println("YELLOW");
    }
    
    else if(result == 4)  {
      Serial.println("PURPLE");
    }
    
    else if(result == 5)  {
      Serial.println("GREEN");
    }
}
