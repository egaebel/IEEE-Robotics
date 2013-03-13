#ifndef STATES_H_
#define STATES_H_

#include "FiniteStateMachine.h"
#include "Movement.h"
#include "WallFollower.h"
#include "common.h"
#include "SimpleFunctions.h"
#include "EmptyColorSensor.h"

//Prototypes
void initEnter();
void initUpdate();
void initExit();

void scanEnter();
void scanUpdate();
void scanExit();

void moveToEnter();
void moveToUpdate();
void moveToExit();

void pickUpEnter();
void pickUpUpdate();
void pickUpExit();

void dropEnter();
void dropUpdate();
void dropExit();

void centerEnter();
void centerUpdate();
void centerExit();
//end prototpyes

#endif