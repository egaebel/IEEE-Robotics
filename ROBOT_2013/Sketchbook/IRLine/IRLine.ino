#include <Arduino.h>
#include "EasyTransfer.h"

//create object
EasyTransfer ET; 

struct RECV_DATA_STRUCTURE{
  int L[8];
  int R[8];
}lineData;

void setup(){
  Serial.begin(115200);
  Serial1.begin(9600);
  //start the library, pass in the data details and the name of the serial port. Can be Serial, Serial1, Serial2, etc.
  ET.begin(details(lineData), &Serial1);
  
  pinMode(13, OUTPUT);
  
  randomSeed(analogRead(0));
  
}

void loop(){
  if(ET.receiveData()){
    for(int i=0;i<8;i++){
      Serial.print(lineData.L[i]);
    }
  }
}
