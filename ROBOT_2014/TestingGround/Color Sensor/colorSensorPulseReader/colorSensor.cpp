#include "ColorSensor.h"
/*Pin Constants */

/**
 * calls the setUp method for the TC3200 sensor
 */
ColorSensor::ColorSensor() {}


//Setup the sensor's pins
void ColorSensor::setup(int S0, int S1, 
                        int S2, int S3, 
                        int out, int LED) {

    this->S0 = S0;
    this->S1 = S1;
    this->S2 = S2;
    this->S3 = S3;
    this->taosOutPin = out;
    this->LED = LED;

    //communication freq (sensitivity) selection
    pinMode(S0,OUTPUT);
    pinMode(S1,OUTPUT);
    //color mode selection
    pinMode(S2,OUTPUT); //S2 pinE
    pinMode(S3,OUTPUT); //s3 pinF
    //initialize pins
    pinMode(LED,OUTPUT); //LED pinD
    //color response pin (only actual input from taos)
    pinMode(taosOutPin, INPUT);

    //Set sensitivity
    digitalWrite(S0, HIGH);
    digitalWrite(S1, LOW);
    
    digitalWrite(LED, HIGH);
}

//Read the passed in color
bool ColorSensor::colorRead(Color color)    {

    //set the S2 and S3 pins to select the color to be sensed
    //White
    if(color == 0){
        digitalWrite(S3, LOW); //S3
        digitalWrite(S2, HIGH); //S2
    }
    //Red
    else if(color == 1){
        digitalWrite(S3, LOW); //S3
        digitalWrite(S2, LOW); //S2
    }
    //Blue
    else if(color == 2){
        digitalWrite(S3, HIGH); //S3
        digitalWrite(S2, LOW); //S2
    }
    //Green
    else if(color == 3){
        digitalWrite(S3, HIGH); //S3
        digitalWrite(S2, HIGH); //S2
    }

    bool readPulse; //where the pulse reading from sensor will go

    //delay(100); //Delay for 100ms for before reading

    // now take a measurement from the sensor, timing a low pulse on the sensor's "out" pin
    readPulse = pulseIn(taosOutPin, LOW, 80000);

    // return the pulse value back to whatever called for it...
    return readPulse;
}