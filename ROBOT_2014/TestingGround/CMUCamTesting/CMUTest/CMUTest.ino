#include "cam.h"

Cam cmuCam = Cam(0);

void setup() {

	Serial.begin(19200);
    Serial.println("Starting....");
    delay(1000);
    Serial.println("Start up camera");
    cmuCam.init();    //start CMUCam4 library to use the other functions
    Serial.println("Camera inited");
    delay(3000);
    cmuCam.getTrackingData(RED);
    Serial.println("tracking red!");
}


void loop() {
	Serial.println("looping...\n\n");
	
	cmuCam.getTrackingData();
}