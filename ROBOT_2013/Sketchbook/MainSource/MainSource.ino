#include <Arduino.h>
#include "FiniteStateMachine.h"
#include "States.h"
//#include "BackEndColorSensor.h"
extern State initState;
FiniteStateMachine fsm(initState);
//BackEndColorSensor colorSensor;

void setup()
{
  //colorSensor = new BackEndColorSensor();
  Serial.begin(9600);
}

void loop()
{
        Serial.println("White:  Blue:  Red:  Green:");
        //Serial.println("%d  %d  %d  %d", colorSensor.readPulse(0), colorSensor.readPulse(1), colorSensor.readPulse(2), colorSensor.readPulse(3));
	//fsm.update();
}
