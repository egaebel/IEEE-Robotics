/*

Eecher's TCS3200 program
adapted from code found at reibot.org

*/

int S0 = 8;//pinB
int S1 = 9;//pinA
int S2 = 12;//pinE
int S3 = 11;//pinF
int taosOutPin = 10;//pinC
int LED = 13;//pinD

/*void setup() {
TCS3200setup();
Serial.begin(115200);
Serial.print("\n\n\nready\n\n\n");
delay(100);
}
*/

/*
// primary loop takes color readings from all four channels and displays the raw values once per second.  What you might wish to do with those values is up to you...
void loop() {

detectColor(taosOutPin);
Serial.print("\n\n\n");
delay(1000);

}
*/

int detectColor(int taosOutPin){

    float white = colorRead(taosOutPin,0,1);
    float red = colorRead(taosOutPin,1,1);
    float blue = colorRead(taosOutPin,2,1);
    float green = colorRead(taosOutPin,3,1);

    Serial.print("white ");
    Serial.println(white);

    Serial.print("red ");
    Serial.println(red);

    Serial.print("blue ");
    Serial.println(blue);

    Serial.print("green ");
    Serial.println(green);

}

/*
This section will return the pulseIn reading of the selected color.  
It will turn on the sensor at the start taosMode(1), and it will power off the sensor at the end taosMode(0)
color codes: 0=white, 1=red, 2=blue, 3=green
if LEDstate is 0, LED will be off. 1 and the LED will be on.
taosOutPin is the ouput pin of the TCS3200.
*/

float colorRead(int taosOutPin, int color, boolean LEDstate){ 
      
    //turn on sensor and use highest frequency/sensitivity setting
    taosMode(1);

    //setting for a delay to let the sensor sit for a moment before taking a reading.
    int sensorDelay = 100;

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

    // create a var where the pulse reading from sensor will go
    float readPulse;

    //  turn LEDs on or off, as directed by the LEDstate var
    if(LEDstate == 0){
        digitalWrite(LED, LOW);
    }
    if(LEDstate == 1){
        digitalWrite(LED, HIGH);
    }

    // wait a bit for LEDs to actually turn on, as directed by sensorDelay var
    delay(sensorDelay);

    // now take a measurement from the sensor, timing a low pulse on the sensor's "out" pin
    readPulse = pulseIn(taosOutPin, LOW, 80000);

    //if the pulseIn times out, it returns 0 and that throws off numbers. just cap it at 80k if it happens
    if(readPulse < .1){
        readPulse = 80000;
    }

    //turn off color sensor and LEDs to save power 
    taosMode(0);

    // return the pulse value back to whatever called for it... 
    return readPulse;

}

// Operation modes area, controlled by hi/lo settings on S0 and S1 pins.
//setting mode to zero will put taos into low power mode. taosMode(0);

void taosMode(int mode){
    
    if(mode == 0){
        //power OFF mode-  LED off and both channels "low"
        digitalWrite(LED, LOW);
        digitalWrite(S0, LOW); //S0
        digitalWrite(S1, LOW); //S1
        //  Serial.println("mOFFm");
        
    }else if(mode == 1){
        //this will put in 1:1, highest sensitivity
        digitalWrite(S0, HIGH); //S0
        digitalWrite(S1, HIGH); //S1
        // Serial.println("m1:1m");
    
    }else if(mode == 2){
        //this will put in 1:5
        digitalWrite(S0, HIGH); //S0
        digitalWrite(S1, LOW); //S1
        //Serial.println("m1:5m");
    
    }else if(mode == 3){
        //this will put in 1:50
        digitalWrite(S0, LOW); //S0
        digitalWrite(S1, HIGH); //S1 
        //Serial.println("m1:50m");
    }
    
    return;

}

void TCS3200setup(){

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
    
    return;
}