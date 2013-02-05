#include <Arduino.h>
#include "IRlib.h"

IRAverager ir_sea(1);
IRAverager ir_rail(0);

int sea;
int rail;

void setup()
{
  	Serial.begin(115200); //Sets the data transfer rate at 115.2 Kbps
	Serial.print("\n\n\nready\n\n\n\n\n\n");
}

void loop()
{
 ir_sea.updateIR();
 ir_rail.updateIR();
 
 if(ir_rail.getIR()<4.5)
   Serial.println("boom rail block");
 else if(ir_sea.getIR()<4.5)
   Serial.println("boom sea block");
  else
    Serial.println("air block!");
}
