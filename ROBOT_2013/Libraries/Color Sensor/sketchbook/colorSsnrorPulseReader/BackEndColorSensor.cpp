#include "BackEndColorSensor.h"
/*Pin Constants */
int S0 = 22;//pinB
int S1 = 24;//pinA //26 on other
int S2 = 26;//pinE //30 on other
int S3 = 28;//pinF //34 on other
int taosOutPin = 30;//pinC //38 on other
int LED = 32;//pinD //42 on other

bool LEDstate = 1; //LED is turned on


//------------------------------TESTING ONLY ------------------------------------------------
/**
 * FOR TESTING SERIAL OUTPUT READ PULSE
 */
 /*
void loop()	{
	//delay(150); //Pauses program for 100ms
	detectColor(taosOutPin); //read pulse readings for each color W,R,G,B
	delay(100); //100ms delay between pulse reads
}
*/
/**
 * FOR TESTING SERIAL OUTPUT READ PULSE
 */
 /*
void setup() {
	TCS3200setup();
	Serial.begin(115200); //Sets the data transfer rate at 115.2 Kbps
	Serial.print("\n\n\nready\n\n\n\n\n\n");

	Serial.print("White: ");
	Serial.print(colorRead(0));

	Serial.print("\n\nBlue: ");
	Serial.print(colorRead(1));

	Serial.print("\n\nRed: ");
	Serial.print(colorRead(2));

	Serial.print("\n\nGreen: ");
	Serial.print(colorRead(3));

	Serial.print("\n\n\n\n");
}
*/
//--------------------------------END OF TESTING ONLY --------------------------------------------


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
 * @returns the pulse of the desired color
 */
int BackEndColorSensor::colorRead(int color)    {

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
void BackEndColorSensor::TCS3200setup()   {
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
 * Returns the pulse value for the desired color
 *
 */


