#include "Sonar.h"

Sonar::Sonar() {

    read();
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