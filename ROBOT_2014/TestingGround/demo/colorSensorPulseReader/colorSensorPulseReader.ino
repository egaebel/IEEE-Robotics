#include "ColorSensor.h"
#include "Arduino.h"
ColorSensor colorSensor;

void setup() {
    colorSensor = ColorSensor();
}

void loop()	{
    Serial.begin(115200);
    Serial.print("\n\n\n\n\n");

   int colorResult = colorSensor.getColor();

      /*  --- For Testing --- */
   if(colorResult == 0) {
      Serial.print("\n WHITE \n");
   } else if(colorResult == 1) {
      Serial.print("\n BLACK \n");
   } else if(colorResult == 2) {
      Serial.print("\n BLUE \n");
   } else {
      Serial.print("\n UN SURE \n");
   }

    delay(2000);
}