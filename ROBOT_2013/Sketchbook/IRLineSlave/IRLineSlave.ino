#include <Arduino.h>
#include "EasyTransfer.h"

int pinLineR[8];
int pinLineL[8];

EasyTransfer ET; 

struct SEND_DATA_STRUCTURE{
  int L[8];
  int R[8];
}lineData;



#define PIN_LINE 0
void setup()
{
  Serial.begin(115200); //Sets the data transfer rate at 115.2 Kbps
  Serial1.begin(9600);
  ET.begin(details(lineData), &Serial1);
  Serial.print("\n\n\nready\n\n\n\n\n\n");
  
  pinMode(13, OUTPUT);
  
  int iRight = 0;
  int iLeft = 0;
  int i= PIN_LINE;
  /*
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
  */
  for(;i<8;i++){
    pinLineL[i] = i;
  }
}

void loop()
{
  int outR[8];
  int outL[8];
  for(int i=0; i<8; i++){
      outL[i] = analogRead(pinLineL[i]);
      lineData.L[i] = outL[i];
      //outR[i] = analogRead(pinLineR[i]);
    }
  for(int i=0; i<17;i++){
    if(i<8){
      Serial.print(outL[i]);
      Serial.print(",");
    }
    else if(i==8)
      Serial.println("");
    //else if (i>8)
      //Serial.print(outR[(i-1)/2]);
    
  }
  ET.sendData();
  Serial.println("");
  delay(1000);
}
