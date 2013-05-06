#include "RSonar.h"

RSonar::RSonar(int pin){
    adcPin = pin;
    distance = -2;
}

int RSonar::getDistance(){
    return distance;
}

boolean RSonar::update(){
    distance = analogRead(adcPin)/2;
    return true;
}
