#include <Arduino.h>
#include "FiniteStateMachine.h"
#include "States.cpp"

FiniteStateMachine fsm(initState);

void setup()
{

}

void loop()
{
	fsm.update();
}
