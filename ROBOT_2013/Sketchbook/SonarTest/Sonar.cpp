#include "Sonar.h"

Sonar::Sonar(int address, int interruptPin) {
  addr = address;
  intPin = interruptPin;
  distance = -2;
}

int Sonar::getDistance() {
  return distance;
}

boolean Sonar::update() {
  if( intPin == -1 ){ //No Interrupts
    if( i2cRequest() == 0){
      //Serial.println("Delay Start");
      delay(100);
      //Serial.println("Delay End");
      int dist = i2cRead();
      if( dist == -1 ){
        return true; //ERROR - no data
      }
      distance = dist;
    }
    return false; //SUCCESS
  }
  else{ //Interrupts
     if(dataReady){
       //Get Data
       sonarISR(this);
     }
     else{
        //Request Data
        i2cRequest(); //Could use some work... Sends excessive requests
     }
     return false; //SUCCESS  
  }
}

int Sonar::i2cRequest(){
  //Serial.println("Requesting");
  Wire.beginTransmission(addr);
  Wire.write(0x51);
  return (Wire.endTransmission());
}

int Sonar::i2cRead() {
  //Serial.println("Reading");
  int data = -1;

  Wire.beginTransmission(addr);
  Wire.requestFrom(addr, 2);
  if (Wire.available() >= 2) {
    data = Wire.read() << 8;
    data |= Wire.read();
  }
  Wire.endTransmission();
  
  return data;
}

/* Commands a sensor at oldAddress to change its address to newAddress 
oldAddress must be the 7-bit form of the address that is used by Wire 
7BitHuh determines whether newAddress is given as the new 7 bit version or the 8 bit version of the address 
\ If true, if is the 7 bit version, if false, it is the 8 bit version 
*/
void Sonar::changeAddress(byte oldAddress, byte newAddress){ 
       Wire.beginTransmission(oldAddress);                 //Begin addressing
       Wire.write(0xAA);              //Send first change address command
       Wire.write(0xA5);              //Send second change address command 
 
       Wire.write(newAddress<<1);                                          //Send the new address to change to 
       Wire.endTransmission();
}

/* To be called from actual ISR so that we don't have to duplicate code.
    See SonarTest.ino for example.
*/
int sonarISR(Sonar* sensor){
       sensor->setDataReady(false);
       //Get values
       int dist = sensor->i2cRead();
       if( dist != -1 ){
         sensor->distance = dist;
       }
       //Request again
       return sensor->i2cRequest();
}

void Sonar::setDataReady(boolean val){
  dataReady = val;
}

