#include "BackEndColorSensor.h"
#include "common.h"
/*Pin Constants */
int S0 = 22;//pinB
int S1 = 24;//pinA
int S2 = 26;//pinE
int S3 = 28;//pinF
int taosOutPin = 30;//pinC
int LED = 32;//pinD

bool LEDstate = 1; //LED is turned on

/**
 * calls the setUp method for the TC3200 sensor
 */
BackEndColorSensor::BackEndColorSensor()    {
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
 * Returns the pulse of the desired color
 */
int BackEndColorSensor::colorRead(int color)    {

    //set the S2 and S3 pins to select the color to be sensed
    if(color == 0){//white
        digitalWrite(COLOR_SENSOR_PIN_S3, LOW); //S3
        digitalWrite(COLOR_SENSOR_PIN_S2, HIGH); //S2
        // Serial.print(" w");
    }

    else if(color == 1){ //blue
        digitalWrite(COLOR_SENSOR_PIN_S3, HIGH); //S3
        digitalWrite(COLOR_SENSOR_PIN_S2, LOW); //S2
        // Serial.print(" b");
    }

    else if(color == 2){//red
        digitalWrite(COLOR_SENSOR_PIN_S3, LOW); //S3
        digitalWrite(COLOR_SENSOR_PIN_S2, LOW); //S2
        // Serial.print(" r");
    }

    else if(color == 3){//green
        digitalWrite(COLOR_SENSOR_PIN_S3, HIGH); //S3
        digitalWrite(COLOR_SENSOR_PIN_S2, HIGH); //S2
        // Serial.print(" g");
    }

    int readPulse; //where the pulse reading from sensor will go

    //  turn LEDs on or off, as directed by the LEDstate var
    if(LEDstate == 0){
        digitalWrite(COLOR_SENSOR_PIN_LED, LOW);
    }
    if(LEDstate == 1){
        digitalWrite(COLOR_SENSOR_PIN_LED, HIGH);
    }

    delay(100); //Delay for 100ms for before reading

    // now take a measurement from the sensor, timing a low pulse on the sensor's "out" pin
    readPulse = int(pulseIn(COLOR_SENSOR_PIN_OUT, LOW, 80000));

    //if the pulseIn times out, it returns 0 and that throws off numbers. just cap it at 80k if it happens
    if(readPulse < .1){
        readPulse = 1;
    }

    // return the pulse value back to whatever called for it...
    return readPulse;
}

/**
* Setup for the Sensor
*/
void BackEndColorSensor::TCS3200setup()   {
    //initialize pins
    pinMode(LED,OUTPUT); //LED pinD

    //color mode selection
    pinMode(COLOR_SENSOR_PIN_S2,OUTPUT); //S2 pinE
    pinMode(COLOR_SENSOR_PIN_S3,OUTPUT); //s3 pinF

    //color response pin (only actual input from taos)
    pinMode(COLOR_SENSOR_PIN_OUT, INPUT); //taosOutPin pinC

    //communication freq (sensitivity) selection
    pinMode(COLOR_SENSOR_PIN_S0,OUTPUT); //S0 pinB
    pinMode(COLOR_SENSOR_PIN_S1,OUTPUT); //S1 pinA

    //this will put in 1:1, highest sensitivity
    digitalWrite(COLOR_SENSOR_PIN_S0, HIGH); //S0
    digitalWrite(COLOR_SENSOR_PIN_S1, HIGH); //S1

    return;
}


