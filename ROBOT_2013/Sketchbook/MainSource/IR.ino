// RIGHT_IR  is 5
// LEFT_IR	 is 6
    
 //1 ir is sea
 //2 irs is rail
int val = 0;           

void setup()
{
  Serial.begin(9600);          //  setup serial
}


void loop()
{
  	Serial.println(analogRead(LEFT_IR));            
  	Serial.println(analogRead(RIGHT_IR));
}

void isBlockUnder() {
	if(analogRead(LEFT_IR) && analogRead(RIGHT_IR)) {
		//Do somthing
	} else {
		//Go to rail
	}
}