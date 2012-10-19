#include <Arduino.h>
#include "FiniteStateMachine.h"
#include "States.h"

extern State initState;
FiniteStateMachine fsm(initState);

void setup()
{

}

void loop()
{
	fsm.update();
}
