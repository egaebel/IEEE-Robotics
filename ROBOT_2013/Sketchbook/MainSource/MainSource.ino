#include "FiniteStateMachine.h"
#include "States.h"
#include "common.h"
#include "Movement.h"
#include "WallFollower.h"
#include "LineSensor.h"
#include <Servo.h>

//#include "BackEndColorSensor.h"
extern State initState;
//BackEndColorSensor colorSensor;
Claw lClaw;
Claw rClaw;
LineSensor line;
FiniteStateMachine fsm(initState);

void setup()
{
  //colorSensor = new BackEndColorSensor();
  
  //setup claws
  lClaw  = new Claw();
  rClaw = new Claw();
  lClaw.init(LCLAW_EXTEND_SERVO, LCLAW_SERVO);
  rClaw.init(RCLAW_EXTEND_SERVO, RCLAW_SERVO);
  Serial.begin(9600);
}

void loop()
{
    Serial.println("White:  Blue:  Red:  Green:");
    //Serial.println("%d  %d  %d  %d", colorSensor.readPulse(0), colorSensor.readPulse(1), colorSensor.readPulse(2), colorSensor.readPulse(3));
    fsm.update();
}
