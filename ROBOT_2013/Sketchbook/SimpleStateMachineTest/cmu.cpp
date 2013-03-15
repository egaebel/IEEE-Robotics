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

bool cam::inZone(){
	getTrackingData(WHITE);
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

void cam::getTrackingData(bColour colour){
  trackColour(colour);
  cmuCam->getTypeTDataPacket(&tData); // Get a tracking packet
}

bColour cam::getBlockColour(){
  int i;
  //loop through all the colours
  for(i=((int)WHITE)+1;i<(int)BLACK;i++){
    getTrackingData((bColour)i);
    //if we see enough pixels of that colour, we found the block
    if(tData.pixels>PIXEL_BLOCK_MIN){
      return (bColour)i;
    }
  }
}

bColour cam::getBayColour(){

}

bSize cam::getBlockSize(bColour colour){

}

void cam::trackColour(bColour colour){
  switch(colour){
    case BLUE:
      cmuCam->trackColor(BLUE_R_MIN,BLUE_R_MAX,BLUE_G_MIN,BLUE_G_MAX,BLUE_B_MIN,BLUE_B_MAX);
    break;
    case RED:
      cmuCam->trackColor(BLUE_R_MIN,BLUE_R_MAX,BLUE_G_MIN,BLUE_G_MAX,BLUE_B_MIN,BLUE_B_MAX);
    break;
    case BROWN:
      cmuCam->trackColor(BLUE_R_MIN,BLUE_R_MAX,BLUE_G_MIN,BLUE_G_MAX,BLUE_B_MIN,BLUE_B_MAX);
    break;
    case PURPLE:
      cmuCam->trackColor(BLUE_R_MIN,BLUE_R_MAX,BLUE_G_MIN,BLUE_G_MAX,BLUE_B_MIN,BLUE_B_MAX);
    break;
    case YELLOW:
      cmuCam->trackColor(BLUE_R_MIN,BLUE_R_MAX,BLUE_G_MIN,BLUE_G_MAX,BLUE_B_MIN,BLUE_B_MAX);
    break;
    case WHITE:
      cmuCam->trackColor(WHITE_R_MIN,WHITE_R_MAX,WHITE_G_MIN,WHITE_G_MAX,WHITE_B_MIN,WHITE_B_MAX);
    break;
    case GREEN:
      cmuCam->trackColor(BLUE_R_MIN,BLUE_R_MAX,BLUE_G_MIN,BLUE_G_MAX,BLUE_B_MIN,BLUE_B_MAX);
    break;
    default:
    abort();
  }
}