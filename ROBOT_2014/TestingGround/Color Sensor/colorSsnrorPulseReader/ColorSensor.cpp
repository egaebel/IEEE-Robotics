#include "ColorSensor.h"
/*Pin Constants */





/**
 * calls the setUp method for the TC3200 sensor
 */
ColorSensor::ColorSensor()    {
    TCS3200setup();
}

/**
 * Public method called on from the ColorSensor class
 * @param color : the desried color to pulse read
                    0 = white
                    1 = blue
                    2 = red
                    3 = green
 *
 * @returns the pulse of the desired color
 */
int ColorSensor::colorRead(int color)    {

    //set the S2 and S3 pins to select the color to be sensed
    if(color == 0){//white
        digitalWrite(S3, LOW); //S3
        digitalWrite(S2, HIGH); //S2
        // Serial.print(" w");
    }

    else if(color == 1){//red
        digitalWrite(S3, LOW); //S3
        digitalWrite(S2, LOW); //S2
        // Serial.print(" r");
    }

    else if(color == 2){//blue
        digitalWrite(S3, HIGH); //S3
        digitalWrite(S2, LOW); //S2
        // Serial.print(" b");
    }

    else if(color == 3){//green
        digitalWrite(S3, HIGH); //S3
        digitalWrite(S2, HIGH); //S2
        // Serial.print(" g");
    }

    int readPulse; //where the pulse reading from sensor will go

    

    delay(100); //Delay for 100ms for before reading

    // now take a measurement from the sensor, timing a low pulse on the sensor's "out" pin
    readPulse = int(pulseIn(taosOutPin, LOW, 80000));

    //if the pulseIn times out, it returns 0 and that throws off numbers, so set any to 1.
    if(readPulse < 1){
        readPulse = 1;
    }

    // return the pulse value back to whatever called for it...
    return readPulse;
}

/**
  * Setup for the Sensor
  */
void ColorSensor::TCS3200setup()   {
    //initialize pins
    pinMode(LED,OUTPUT); //LED pinD

    //color mode selection
    pinMode(S2,OUTPUT); //S2 pinE
    pinMode(S3,OUTPUT); //s3 pinF

    //color response pin (only actual input from taos)
    pinMode(taosOutPin, INPUT); //taosOutPin pinC

    //communication freq (sensitivity) selection
    pinMode(S0,OUTPUT); //S0 pinB
    pinMode(S1,OUTPUT); //S1 pinA

    //this will put in 1:1, highest sensitivity
    digitalWrite(S0, HIGH); //S0
    digitalWrite(S1, HIGH); //S1
    
    digitalWrite(LED, HIGH);
    
    return;
}

/**
  * Return values:
  *     0 = White
  *     1 = Black
  *     2 = Blue
  *     3 = Un-sure
  */
int ColorSensor::getColor() {
    int whiteVal = colorRead(0);
    int redVal = colorRead(2);
    int greenVal = colorRead(3);
    int blueVal = colorRead(1);

    if(blueVal > redVal && blueVal + greenVal + redVal > 70) {
        return 2;
    }

    else if(blueVal + greenVal + redVal < 60) {
        return 0;
    }

    else if(blueVal + greenVal + redVal > 70) {
        return 1;
    }

    else { //Un-sure of color
        return 3;
    }
}



