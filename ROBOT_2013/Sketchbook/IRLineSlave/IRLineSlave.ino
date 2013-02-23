#include <Arduino.h>
#include "LineSensor.h"
#include "EasyTransfer.h"


LineSensor sensor;

struct DATA_TX_STRUCT{
   boolean line[16];
}dataTX;

EasyTransfer ET;

void setup()
{
  Serial.begin(115200); //Sets the data transfer rate at 115.2 Kbps
  Serial1.begin(9600);
  Serial.print("\n\n\nready\n\n\n\n\n\n");
  ET.begin(details(dataTX),&Serial1);
}

void loop()
{
  sensor.getTotal(dataTX.line);
  
  for(int i=0;i<16;i++)
  {
    Serial.print(analogRead(i));
    Serial.print(", ");
  }
  
  Serial.println("");
  
  for(int i=0;i<16;i++)
  {
    Serial.print(dataTX.line[i]);
    Serial.print(", ");
  }
  
  Serial.println("");
  Serial.println("");
  ET.sendData();
  delay(1000);
}
