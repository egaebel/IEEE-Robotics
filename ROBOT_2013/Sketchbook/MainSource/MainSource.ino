#define TWI_FREQ 25000L
#include "FiniteStateMachine.h"
#include "States.h"
#include "States_Test.h"
#include <Servo.h>
#include <Wire.h>
#include "IRlib.h"
#include "common.h"

#define DEBUG_FSM 0

Sonar sonarLeft(SONAR_LEFT,SONAR_LEFT_INT);
Sonar sonarRight(SONAR_RIGHT,SONAR_RIGHT_INT);
IRAverager leftIR;
IRAverager rightIR;

Movement move;

cam leftCam = cam(LCAM_PIN);
cam rightCam = cam(RCAM_PIN);


extern State state1;
FiniteStateMachine fsm_debug(state1);
extern State initState;
FiniteStateMachine fsm(initState);

void handleSonarLeft(){
  sonarLeft.setDataReady(true);
}

void handleSonarRight(){
  sonarRight.setDataReady(true);
}

void setup() {
        delay(100); //Make sure we don't catch the PWM from POR of Sonar
        attachInterrupt(SONAR_LEFT_INT,handleSonarLeft,FALLING);
        attachInterrupt(SONAR_RIGHT_INT,handleSonarRight,FALLING);
        Wire.begin();
        
        delay(200);
	//setup claws
	//lClaw.init(LCLAW_EXTEND_SERVO, LCLAW_SERVO);
	//rClaw.init(RCLAW_EXTEND_SERVO, RCLAW_SERVO);
	//fsm
	Serial.begin(9600);
        Serial.println("HEPR");
        move.init();
      
      }

void loop() {
    sonarLeft.update();
    sonarRight.update();
    Serial.println(sonarLeft.getDistance());
    //leftIR.updateIR();
    //rightIR.updateIR();
/*    Serial.print("Sonar L: ");
    Serial.print(sonarLeft.getDistance());
    Serial.print("\tSonar R: ");
    Serial.println(sonarRight.getDistance());*/
#if DEBUG_FSM ==0
    fsm.update();
#else
    fsm_debug.update();
#endif
}
