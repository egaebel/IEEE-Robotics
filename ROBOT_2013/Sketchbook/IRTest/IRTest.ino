#include <Arduino.h>
#include "IRlib.h"

IRAverager *ir_sea;
IRAverager *ir_rail;

int sea;
int rail;

void setup()
{
  	Serial.begin(115200); //Sets the data transfer rate at 115.2 Kbps
	Serial.print("\n\n\nready\n\n\n\n\n\n");
  ir_sea = &IRAverager(1);
  ir_rail= &IRAverager(0);
}

void loop()
{
  
 ir_sea->updateIR();
 ir_rail->updateIR();
 Serial.println(ir_sea->getIR());
 sea = analogRead(1);
 rail = analogRead(0);
 if(rail>480)
   Serial.println("boom rail block");
 else if(sea>480)
   Serial.println("boom sea block");
  else
    Serial.println("air block!");
    delay(1000);
}
