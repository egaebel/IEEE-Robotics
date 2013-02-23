#include "FiniteStateMachine.h"
#include "States.h"
#include "EasyTransfer.h"
#include <Servo.h>
extern UART_STRUCT uart;
extern State state1;

EasyTransfer ET;

FiniteStateMachine fsm(state1);

void setup() {
    Serial.begin(9600);
    Serial1.begin(9600);
    ET.begin(details(uart), &Serial1);
}

//go through the entire state machine
void loop() {
    fsm.update();
    if(ET.receiveData());
       Serial.print("Rec'vd data");
}
