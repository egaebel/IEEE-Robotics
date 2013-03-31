#define TWI_FREQ 25000L

#define SONAR_LEFT byte(0x70)
#define SONAR_RIGHT byte(0x69)

#include "Wire.h"

void setup() {
  Serial.begin(115200);
  Wire.begin();
}


int i2cRead(int address) {

  int data = 0;

  Wire.beginTransmission(address);
  Wire.write(0x51);
  Wire.endTransmission();
  delay(25);
  Wire.beginTransmission(address);
  Wire.requestFrom(address, 2);

  if (Wire.available()) {
    data = Wire.read() << 8;
  }
  if (Wire.available()) {
    data |= Wire.read();
  }

  return data;
}



int sensor=SONAR_RIGHT;
unsigned long time=0;

void loop(){
  if(
    millis()>time+500){
    Serial.print("sonar: ");
    Serial.println(i2cRead(sensor));
    time=millis();
    /*
    if(sensor==SONAR_LEFT)
      sensor=SONAR_RIGHT;
    else
      sensor=SONAR_LEFT;*/
    }
    
}
