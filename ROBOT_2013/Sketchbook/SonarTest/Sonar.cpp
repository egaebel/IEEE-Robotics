#include "Sonar.h"

Sonar::Sonar() {
}

int Sonar::getLeftDistance() {

  return leftDistance;
}

int Sonar::getRightDistance() {

  return rightDistance;
}

void Sonar::read() {

  leftDistance = i2cRead(SONAR_LEFT);
  rightDistance = i2cRead(SONAR_RIGHT);
}

int Sonar::i2cRead(int address) {

  int data = -1;

  Wire.beginTransmission(address);
  Wire.write(0x51);
  //Wire.endTransmission();
  delay(100);
  //Wire.beginTransmission(address);
  Wire.requestFrom(address, 2);
  if (Wire.available() >= 2) {
    data = Wire.read() << 8;
    data |= Wire.read();
  }
  Wire.endTransmission();
  //delay(25);
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

