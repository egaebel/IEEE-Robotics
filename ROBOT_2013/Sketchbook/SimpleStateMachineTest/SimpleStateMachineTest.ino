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
cam camR(1);
void setup() {
    Serial.begin(9600);
    pinMode(53, INPUT);
    Serial2.begin(9600);
    ET.begin(details(uart), &Serial2);
    camR.init();
}

//go through the entire state machine
void loop() {
    //fsm.update();
    camR.inZone();
    if(ET.receiveData()){
       int i;
       for(i=0;i<16;i++){
       		Serial.print(uart.line[i]);
       }
       Serial.print('\n');
    }
    delay(100);
}
