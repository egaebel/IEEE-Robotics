#include "FiniteStateMachine.h"
#include "States.h"
#include <Servo.h>
#include "cmu.h"
extern UART_STRUCT uart;
extern State state1;

extern FiniteStateMachine fsm;
void setup() {
    Serial.begin(9600);
    pinMode(BUMPER_L, INPUT);
    pinMode(BUMPER_R, INPUT);
}

//go through the entire state machine
void loop() {
    fsm.update();
}
