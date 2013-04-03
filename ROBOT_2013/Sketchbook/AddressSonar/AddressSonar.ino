#include <Arduino.h>
#include <Wire.h>
#include "common.h"
#include "Sonar.h"

void setup(){
  Serial.begin(9600);
  Wire.begin();
}

void getAddresses(int* pOldAddress){
  char str[3] = "";
  Serial.print("\tCurrent Address: 0x");
  while(!Serial.available());
  Serial.readBytes(str,2);
  *pOldAddress = strtol(str,0,16);  
}

void setAddress(int oldAddress, int newAddress){
       Serial.println(oldAddress,HEX);
       Serial.print("\tNew Address: 0x");
       Serial.println(newAddress,HEX);
       int ret = Sonar::changeAddress(byte(oldAddress),byte(newAddress));
       if( ret != 0 ){
         Serial.print("\tERROR: Failed to change address. Code: ");
         Serial.println(ret);
       }
}

void loop(){
  int oldAddress = 0x70;
  int choice = 0;
  Serial.println("Choose an Option:");
  Serial.println(" 1 Set Left Address");
  Serial.println(" 2 Set Right Address");
  Serial.println(" 0 Exit");
  
  while(!Serial.available());
  choice = Serial.parseInt();
  
  switch(choice){
    case 1:
    {
       Serial.println("\tDisconnect Right Sonar.");
       getAddresses(&oldAddress);
       setAddress(oldAddress,SONAR_LEFT);
       Serial.print("\n\n");
       break;
    }
    case 2:
    {
       Serial.println("\tDisconnect Left Sonar.");
       getAddresses(&oldAddress);
       setAddress(oldAddress,SONAR_RIGHT);
       Serial.print("\n\n");
       break;
    }
    case 0:
    default:
      Serial.print("\n\n\n\n\n\n\n\n\n");
      break; 
  }
}
