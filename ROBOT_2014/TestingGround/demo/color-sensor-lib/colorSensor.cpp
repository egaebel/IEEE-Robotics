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
    //2% = LOW, HIGH
    //100% = HIGH, HIGH
    //20% = HIGH, LOW
    digitalWrite(S0, HIGH);
    digitalWrite(S1, HIGH);
    
    //Turn LED on or off
    digitalWrite(LED, LOW);
}

//Gets the color that the color sensor is currently over
Color ColorSensor::getColor() {

    unsigned int bluePulse = colorRead(BLUE);
    unsigned int redPulse = colorRead(RED);
    unsigned int greenPulse = colorRead(GREEN);
    unsigned int whitePulse = colorRead(WHITE);
    
    //Put into array to loop
    unsigned int colorPulseArray[4];
    colorPulseArray[0] = bluePulse;
    colorPulseArray[1] = redPulse;
    colorPulseArray[2] = greenPulse;
    colorPulseArray[3] = whitePulse;

    //Loop over all pulses to discover if all pulses are within 10 of eachother
        //or what the smallest pulse is (ignoring the whitePulse value)
    Color minColor;
    //Set min to max
    unsigned int minPulse = 0xffff;
    unsigned int biggestDifference = 0;
    for (unsigned short i = 0; i < 3; i++) {
        Serial.print("color pulse value ==");
        Serial.println(colorPulseArray[i]);
        //Find the smallest pulse value
        if (i == 0 || colorPulseArray[i] < minPulse) {
            minPulse = colorPulseArray[i];

            //Which color are we on?
            switch (i) {
                case 0:
                    minColor = BLUE;
                    break;
                case 1:
                    minColor = RED;
                    break;
                case 2:
                    minColor = GREEN;
                    break;
                case 3:
                    minColor = WHITE;
                    break;
            }
        }

        //If not the first iteration
        if (i != 0) {

            //Compare this pulse value and the previous pulse value
            if ((colorPulseArray[i] - colorPulseArray[i - 1]) > biggestDifference) {
                biggestDifference = (colorPulseArray[i] - colorPulseArray[i - 1]);
            }
        }
    }

    //if all values within 10
    //White
    if (biggestDifference < 10) {

        return WHITE;
    }
    //Take min color
    else {
        return minColor;
    }
}

//Read the passed in color
int ColorSensor::colorRead(Color color)    {

    //set the S2 and S3 pins to select the color to be sensed
    //White
    if(color == WHITE){
        digitalWrite(S3, LOW); //S3
        digitalWrite(S2, HIGH); //S2
        Serial.println("white");
    }
    //Red
    else if(color == RED){
        digitalWrite(S3, LOW); //S3
        digitalWrite(S2, LOW); //S2
        Serial.println("Red");
    }
    //Blue
    else if(color == BLUE){
        digitalWrite(S3, HIGH); //S3
        digitalWrite(S2, LOW); //S2
        Serial.println("Blue");
    }
    //Green
    else if(color == GREEN){
        digitalWrite(S3, HIGH); //S3
        digitalWrite(S2, HIGH); //S2
        Serial.println("Green");
    }

    int readPulse; //where the pulse reading from sensor will go

    //Turn on LED
    digitalWrite(LED, HIGH);

    delay(100);

    // now take a measurement from the sensor, timing a low pulse on the sensor's "out" pin
    readPulse = pulseIn(taosOutPin, LOW, 80000);

    //Turn off LED
    digitalWrite(LED, LOW);

    Serial.print("Read Pulse == ");
    Serial.println(readPulse);

    return readPulse;
}