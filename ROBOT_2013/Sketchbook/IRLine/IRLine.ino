#include <Arduino.h>

int pinLineR[8];
int pinLineL[8];

#define PIN_LINE 38
void setup()
{
  Serial.begin(115200); //Sets the data transfer rate at 115.2 Kbps
  Serial.print("\n\n\nready\n\n\n\n\n\n");
  int iRight = 0;
  int iLeft = 0;
  int i= PIN_LINE;
  
  for(; i<(16+PIN_LINE); i++){
    if(i%2==0){
      pinLineL[iLeft] = i;
      iLeft++;
    }
    else{
      pinLineR[iRight] =i;
      iRight++;
    }    
  }
}

void loop()
{
  int outR[8];
  int outL[8];
  for(int i=0; i<8; i++){
      outL[i] = digitalRead(pinLineL[i]);
      outR[i] = digitalRead(pinLineR[i]);
    }
  for(int i=0; i<17;i++){
    if(i<8)
      Serial.print(outL[i]);
    else if (i==8)
      Serial.println("hindsight");
    else
      Serial.print(outR[(i-1)/2]);
    
  }
  Serial.println("");
  delay(1000);
}
