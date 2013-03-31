#ifndef STATES_H_
#define STATES_H_

//FOR NOW THIS IS EMPTY...MAY NEED TO UPDATE NAME WHEN TRANSITIONING TO REAL CAM class
#include "cam.h"

#include "common.h"
#include "FiniteStateMachine.h"
#include "Movement.h"
#include "Sonar.h"
#include "SimpleFunctions.h"

//Prototypes
void initEnter();
void initUpdate();
void defExit();

void scanEnter();
void scanUpdate();

void moveToEnter();
void moveToUpdate();

void pickUpEnter();
void pickUpUpdate();

void dropEnter();
void dropUpdate();
//end prototpyes

#endif
