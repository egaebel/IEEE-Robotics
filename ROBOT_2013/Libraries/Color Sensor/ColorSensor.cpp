/*
Eecher's TCS3200 program
adapted from code found at reibot.org
*/
struct colorID { 
	float value;
	int color; /* color = [0 is white] [1 is red] [2 is blue] [3 is green] */
};

int S0 = 8;//pinB
int S1 = 9;//pinA
int S2 = 12;//pinE
int S3 = 11;//pinF
int taosOutPin = 10;//pinC
int LED = 13;//pinD


int readCounter; //counter for looping colorReads
colorID whites [10]; //array of white pulses
colorID reds [10]; //array of white pulses
colorID blues [10]; //array of white pulses
colorID greens [10]; //array of white pulses

colorID sumWhite;
colorID sumRed;
colorID sumBlue;
colorID sumGreen;

void setup() {
	TCS3200setup();
	Serial.begin(115200); //Sets the data transfer rate at 115.2 Kbps
	Serial.print("\n\n\nready\n\n\n\n\n\n");
	
	//Loop prep
	readCounter = 0; 
	whites[:10].color = 0;
	reds[:10].color = 1;
	blues[:10].color = 2;
	greens[:10].color = 3;
}

void loop()	{
	delay(100); //Pauses program for 100ms
	detectColor(taosOutPin); //read pulse readings for each color W,R,G,B
}

void TCS3200setup()	{
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

/* COMMENTED OUT FOR TESTING
// primary loop takes color readings from all four channels and displays the raw values once per second.  
void loop() {
detectColor(taosOutPin);
Serial.print("\n\n\n");
delay(1000);
}
*/

/*
 * Detects the color (calls colorRead on each color)
 */
void detectColor(int taosOutPin){

	if(readCounter < 10)	{
		whites[readCounter].value = colorRead(taosOutPin,0,1);
		reds[readCounter].value = colorRead(taosOutPin,1,1);
		blues[readCounter].value = colorRead(taosOutPin,2,1);
		greens[readCounter].value = colorRead(taosOutPin,3,1);
		
		readCounter++; //Increments readCoutner
	}
	
	else if(readCounter == 10)	{
		//Sums all the values
		for(readCounter = 0; readCounter < 10; readCounter++)	{
			sumWhite.value += (int)whites[readCounter];
			sumRed.value += (int)reds[readCounter];
			sumBlue.value += (int)blues[readCounter];
			sumGreen.value += (int)greens[readCounter];
		}
		
		Serial.print((std::min(std::min(std::min(sumWhite, sumRed), sumBlue), sumGreen)).color); //returns the lowest's pulse readings color (the dominant color)
	}
	
	else { readCounter = 0; }

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

    float readPulse; //where the pulse reading from sensor will go

    //  turn LEDs on or off, as directed by the LEDstate var
    if(LEDstate == 0){
        digitalWrite(LED, LOW);
    }
    if(LEDstate == 1){
        digitalWrite(LED, HIGH);
    }

    delay(100); //Delay for 100ms for before reading

    // now take a measurement from the sensor, timing a low pulse on the sensor's "out" pin
    readPulse = pulseIn(taosOutPin, LOW, 80000);

    //if the pulseIn times out, it returns 0 and that throws off numbers. just cap it at 80k if it happens
    if(readPulse < .1){
        readPulse = 80000;
    }

    //turn off color sensor and LEDs to save power POSSIBLE REMOVE --> b/c in the IEEE Comp power is not a constraint
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


