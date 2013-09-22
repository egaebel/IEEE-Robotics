#include "cam.h"

cam cmuCam = cam(0);

void setup() {

	
}

void loop() {

	cmuCam.getTrackingData(RED);
}