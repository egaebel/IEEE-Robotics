#include "cmu.h"


cam::cam(int pin){
	cmuCam = new CMUcam4(pin);
}

void cam::init(){
	cmuCam->begin();
  	// Wait for auto gain and auto white balance to run.

  	cmuCam->LEDOn(LED_BLINK);
  	delay(WAIT_TIME);

  	// Turn auto gain and auto white balance off.

  	cmuCam->autoGainControl(false);
  	cmuCam->autoWhiteBalance(false);

  	cmuCam->LEDOn(CMUCAM4_LED_ON);

  	cmuCam->pollMode(POLL_MODE);
  	cmuCam->colorTracking(YUV_MODE);

  	cmuCam->noiseFilter(NOISE_FILTER);	
}

void cam::trackBlue(){
	cmuCam->trackColor(BLUE_R_MIN,BLUE_R_MAX,BLUE_G_MIN,BLUE_G_MAX,BLUE_B_MIN,BLUE_B_MAX);
}
bool cam::inZone(){
	getTrackingData();
	#define CENTROID_X_MIN 75
	#define CENTROID_X_MAX 90
	#define CENTROID_Y_MIN 50
	#define CENTROID_Y_MAX 70
	#define BAY_AREA_MIN 14000
	#define BAY_AREA_MAX 20000
	Serial.print("X:");Serial.println(tData.mx);
    Serial.print("Y:");Serial.println(tData.my);
    int area = (tData.x2-tData.x1)*(tData.y2-tData.y1);
    Serial.print("area:");Serial.println(area);
    if(tData.mx>CENTROID_X_MIN && tData.mx<CENTROID_X_MAX && tData.my>CENTROID_Y_MIN && tData.my<CENTROID_Y_MAX && area>BAY_AREA_MIN && area<BAY_AREA_MAX){
    	Serial.println("WE FOUND MICHEAL BAY\n\n\n");
    	return 1;
    }
    return 0;
}

void cam::getTrackingData(){
	trackBlue();
	//cmuCam->trackColor(RED_MIN, RED_MAX, GREEN_MIN, GREEN_MAX, BLUE_MIN, BLUE_MAX);
    //cmuCam->trackColor(100,180,100,180,200,255);
    cmuCam->getTypeTDataPacket(&tData); // Get a tracking packet
}