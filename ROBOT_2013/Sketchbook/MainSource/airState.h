#ifndef AIRSTATE_H_
#define AIRSTATE_H_

#include "cam.h"

#include "common.h"
#include "FiniteStateMachine.h"
#include "Movement.h"
#include "Sonar.h"
#include "SimpleFunctions.h"
#include "IRlib.h"

void moveToPlatformEnter();
void moveToAirPlatform();
void scanAirPlatformEnter();
void scanAirPlatform();
void dropAirBlocksEnter();
void dropAirBlocks();
void moveAP_cleanUp();
void scanAP_cleanUp();
void dropAP_cleanUp();
#endif

