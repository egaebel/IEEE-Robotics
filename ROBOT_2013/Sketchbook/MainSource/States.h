#ifndef STATES_H_
#define STATES_H_

//FOR NOW THIS IS EMPTY...MAY NEED TO UPDATE NAME WHEN TRANSITIONING TO REAL CAM class
#include "cam.h"

#include "common.h"
#include "FiniteStateMachine.h"
#include "Movement.h"
#include "Claw.h"
#include "WallFollower.h"
#include "SimpleFunctions.h"

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
//end prototpyes

#endif