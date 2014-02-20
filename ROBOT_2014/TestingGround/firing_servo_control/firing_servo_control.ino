#include <Servo.h>

// interrupt 0 is on digital pin 2 for Arduino Uno
const int READY_TO_FIRE_INTERRUPT = 0;
const int READY_TO_FIRE_PIN = 2;

const int FIRING_SERVO_1_PIN = 9;
const int FIRING_SERVO_2_PIN = 9;
const int FIRING_SERVO_3_PIN = 9;
const int SERVO_FIRE_POSITION = 544;
const int SERVO_REST_POSITION = 2400;
const int LOOP_DELAY = 3;

// The three firing barrel servos
Servo firing_servo_1, firing_servo_2, firing_servo_3;

// Set by hardware interrupt, indicates whether or not the camera is aiming at the target and ready to fire
volatile boolean fire = false;

// Interrupt handler that triggers on change of the ready to fire notification pin
void fire_handler()
{
  fire = ( digitalRead( READY_TO_FIRE_PIN ) == LOW );
}

// Initialize the servos and set up the hardware interrupt
void setup()
{
  firing_servo_1.attach( FIRING_SERVO_1_PIN );
  firing_servo_1.writeMicroseconds( SERVO_REST_POSITION );
  firing_servo_2.attach( FIRING_SERVO_2_PIN );
  firing_servo_2.writeMicroseconds( SERVO_REST_POSITION );
  firing_servo_3.attach( FIRING_SERVO_3_PIN );
  firing_servo_3.writeMicroseconds( SERVO_REST_POSITION );
  
  pinMode( READY_TO_FIRE_PIN, INPUT_PULLUP );
  // when this pin is pulled low, the servo will fire
  attachInterrupt( READY_TO_FIRE_INTERRUPT, fire_handler, CHANGE );
}

// The main program junk
void loop()
{
  while( !fire )
  {
    // wait for camera to aim, perhaps have a timeout here that rotates the entire robot or something in case the camera has lost the target completely
  }
  firing_servo_1.writeMicroseconds( SERVO_FIRE_POSITION );
  delay( LOOP_DELAY );
  while( !fire )
  {
    // wait for camera to aim, perhaps have a timeout here that rotates the entire robot or something in case the camera has lost the target completely
  }
  firing_servo_2.writeMicroseconds( SERVO_FIRE_POSITION );
  delay( LOOP_DELAY );
  while( !fire )
  {
    // wait for camera to aim, perhaps have a timeout here that rotates the entire robot or something in case the camera has lost the target completely
  }
  firing_servo_3.writeMicroseconds( SERVO_FIRE_POSITION );
  delay( LOOP_DELAY );
}

