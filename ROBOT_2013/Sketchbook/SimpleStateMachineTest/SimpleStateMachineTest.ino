#include "FiniteStateMachine.h"
#include "States.h"
#include "EasyTransfer.h"
#include <Servo.h>
#include "cmu.h"
extern UART_STRUCT uart;
extern State state1;
extern bool buttonPushed;

EasyTransfer ET;

extern FiniteStateMachine fsm;
cam camR(2);
void setup() {
    Serial.begin(9600);
    pinMode(BUMPER_L, INPUT);
    pinMode(BUMPER_R, INPUT);
    camR.init();
}

//go through the entire state machine
void loop() {
    //fsm.update();
    camR.getTrackingData(BROWN);

    delay(100);
}
