#include "FiniteStateMachine.h"
#include "States.h"
#include "EasyTransfer.h"
#include <Servo.h>
extern UART_STRUCT uart;
extern State state1;
extern bool buttonPushed;

EasyTransfer ET;

extern FiniteStateMachine fsm;

void setup() {
    Serial.begin(9600);
    pinMode(53, INPUT);
    Serial1.begin(9600);
    ET.begin(details(uart), &Serial);
}

//go through the entire state machine
void loop() {
    fsm.update();
    if(ET.receiveData())
       Serial.print("Rec'vd data");
    delay(100);
}
