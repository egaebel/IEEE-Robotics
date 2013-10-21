/*
 Parallax Laser Range Finder with Arduion Mega 2560
 
 An example of using the Arduino board to receive data from the 
 computer, and output it to the LRF Module, In turn what the LRF
 module sends gets displayed on the serial port.
 
 Open up the Serial Monitor, and type U to innitialze communications
 with the LRF, for a list of commands type H.
 
 Works with Arduino Mega 2560
 verifyed with Arduino 022
 
 5v to VCC
 GND to GND
 18  uart1 with SIN
 19  uart1 with SOUT
 
 This code is in copyleft Paul Sobczak.
 themountainfold.com
 */
#define TRUE 1
#define FALSE 0

boolean header1 = TRUE;

// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led = 13;

void setup() 
{
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);  
  
  // initialize serial communication:
  Serial.begin(9600);   //Serial Monitor to LRF
  Serial1.begin(9600);  //LRF to Serial Monitor
  delay(100);
  Serial1.write('U');
  // the LRF will send back a : and a newline, we just read and ignore those here
  Serial1.read();
  Serial1.read();
  Serial.println("Setup Done.");
  Serial1.write('L');
}

void loop() 
{
    // delay a tiny bit before the next read
  delay(200);
  char lrfData[32];
  int offset = 0;
  lrfData[0] = 0;
  while (Serial1.available() > 0) {
    // read what the LRF sends back 
    lrfData[offset] = Serial1.read();
    if(lrfData[offset] == ':')
    {
      lrfData[offset] = 0; // null terminate the string of bytes and break out of the loop
      break;
    }
    offset++;
  }
  if(offset > 0)
  {
    // print out the data from the LRF (it sends back a string in the form "D = xxxx mm" where xxxx is the distance in milimeters 
    Serial.println(lrfData);
  
    //integer to convert lrfData string to an integer value
    int distance = atoi(&lrfData[4]);
    Serial.print("Distance: ");
    Serial.println(distance);
    //sound the alert if the distance is less than one meter
    if (distance < 1000)
    {
      digitalWrite(led, HIGH);
    }
    else
    {
      digitalWrite(led, LOW);
    }
  }
}
