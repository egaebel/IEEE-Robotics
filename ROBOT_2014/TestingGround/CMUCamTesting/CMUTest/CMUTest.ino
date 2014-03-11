#include "cam.h"

Cam cmuCam(0);

void setup() {

//#define CMUCOM4_SLOW_BAUD_RATE      19200

//#define CMUCOM4_MEDIUM_BAUD_RATE    115200

//#define CMUCOM4_FAST_BAUD_RATE      250000


	Serial.begin(9600);
    //Serial.println("Starting....");
    delay(1000);
    //Serial.println("Start up camera");
    cmuCam.init();    //start CMUCam4 library to use the other functions
    //Serial.println("Camera inited");
    delay(3000);
    cmuCam.getTrackingData(BLUE);
    //cmuCam.setTrackingWindow(0, 0, 50000, 50000);
    //Serial.println("tracking red");
    //HEY LOOK I CHANGED THIS THING RIGHT HERE!
}


void loop() {
	Serial.println("looping...\n\n");
    
    switch (cmuCam.getColorWrapper(10)) {

        case WHITE:
            Serial.println("WHITE");
            break;
        case RED:
            Serial.println("RED");
            break;
        case BLUE:
            Serial.println("BLUE");
            break;
        case BLACK:
            Serial.println("BLACK");
            break;
        case EVERYTHING:
            Serial.println("EVERYTHING");
            break;
        default:
            Serial.println("WAT>>>");
            break;
    }
    //cmuCam.setTrackingWindow(0, 0, 50000, 50000);
	//cmuCam.getTrackingData(RED);
}
