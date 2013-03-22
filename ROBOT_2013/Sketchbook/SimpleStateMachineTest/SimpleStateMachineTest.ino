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
    pinMode(53, INPUT);
    camR.init();
}

//go through the entire state machine
void loop() {
    fsm.update();
    camR.inZone();

    delay(100);
}
