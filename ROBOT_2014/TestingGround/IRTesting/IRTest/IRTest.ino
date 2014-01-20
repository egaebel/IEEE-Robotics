#include "Arduino.h"
#include "IRLib.h"

IRAverager ir(0);
int val;

void setup() {
  
    Serial.begin(9600);
    Serial.println("Serial has started\n");    
}

void loop() {

	ir.updateIR();
    val = ir.getIR();
    Serial.print("The val is:: ");
    Serial.println(val);
    delay(100);
}

