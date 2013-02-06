#include "ColorSensor.h"
#include "Arduino.h"

ColorSensor *sensor;


void setup() {
    sensor = &ColorSensor();
    Serial.begin(115200);
}

void loop()	{
    
    Serial.print("\n\n ANOTHER:  ");
    
    int result[5];
    sensor->detectColor(result);
    int blue = result[0];
    int yellow = result[1];
    int brown = result[2];
    int red = result[3];
    int purple = result[4];
    
    Serial.print("result: ");
      //Result 
      Serial.println(blue);
      Serial.println(yellow);
      Serial.println(brown);
      Serial.println(red);
      Serial.println(purple);
      Serial.println("Blue value");
      
      if(blue==1){
        Serial.println("its 1");
      }
      if(blue == 0) {
        Serial.println("its 0");
      }
}
