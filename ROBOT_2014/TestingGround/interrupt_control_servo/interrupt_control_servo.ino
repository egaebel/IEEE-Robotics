#include <Servo.h>

// interrupt 0 is on digital pin 2 for Arduino Uno
const int advance_interrupt = 0;
const int advance_pin = 2;
volatile boolean advance = false;

// interrupt 1 is on digital pin 3 for Arduino Uno
const int retreat_interrupt = 1;
const int retreat_pin = 3;
volatile boolean retreat = false;

const int servo_pin = 9;
const int servo_min = 544;
const int servo_max = 2400;
const int rate_limit = 1;
Servo servo;
int servo_pos = 1500;

void advance_handler()
{
  advance = ( digitalRead( advance_pin ) == LOW );
}

void retreat_handler()
{
  retreat = ( digitalRead( retreat_pin ) == LOW );
}

void setup()
{
  servo.attach( servo_pin );
  servo.writeMicroseconds( servo_pos );
  
  pinMode( advance_pin, INPUT_PULLUP );
  // when this pin is pulled low, the servo will move forward
  attachInterrupt( advance_interrupt, advance_handler, CHANGE );
  
  pinMode( retreat_pin, INPUT_PULLUP );
  // when this pin is pulled low, the servo will move reverse
  attachInterrupt( retreat_interrupt, retreat_handler, CHANGE );
}

void loop()
{
  if( advance && ( servo_pos < servo_max ) )
    servo_pos++;
  else if( retreat && ( servo_pos > servo_min ) )
    servo_pos--;
  servo.writeMicroseconds( servo_pos );
  delay( rate_limit );
}

