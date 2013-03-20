#include "cmu.h"

cam::cam(int pin){
	cmuCam = new CMUcam4(pin);
}

void cam::init(){
	cmuCam->begin();
  	// Wait for auto gain and auto white balance to run.
        cmuCam->autoGainControl(false);
  	cmuCam->autoWhiteBalance(false);
        cmuCam->cameraBrightness(50);
        cmuCam->cameraContrast(10);
  	cmuCam->LEDOn(LED_BLINK);
  	//delay(WAIT_TIME);

  	// Turn auto gain and auto white balance off.

  	//cmuCam->autoGainControl(false);
  	//cmuCam->autoWhiteBalance(false);

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
    if(tData.mx>CENTROID_X_MIN && tData.mx<CENTROID_X_MAX && tData.my>CENTROID_Y_MIN && tData.my<CENTROID_Y_MAX && area>BAY_AREA_MIN && area<BAY_AREA_MAX)  {
		//tdata1 is point at upper left corner and tdata2 is point at lower right corner
		if(abs(tData.x1 - UPPER_LEFT_CORNER_X) < UNCERTAINTY_ALLOWANCE && abs(tData.x2 - LOWER_RIGHT_CORNER_X) < UNCERTAINTY_ALLOWANCE
			&& abs(tData.y1 - UPPER_LEFT_CORNER_Y) < UNCERTAINTY_ALLOWANCE && abs(tData.y2 == LOWER_RIGHT_CORNER_Y) < UNCERTAINTY_ALLOWANCE) { 
				Serial.println("WE FOUND MICHEAL BAY\n\n\n");
				return 1;
		}
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
      cmuCam->trackColor(RED_R_MIN,RED_R_MAX,RED_G_MIN,RED_G_MAX,RED_B_MIN,RED_B_MAX);
    break;
    case BROWN:
      cmuCam->trackColor(BROWN_R_MIN,BROWN_R_MAX,BROWN_G_MIN,BROWN_G_MAX,BROWN_B_MIN,BROWN_B_MAX);
    break;
    case PURPLE:
      cmuCam->trackColor(PURPLE_R_MIN,PURPLE_R_MAX,PURPLE_G_MIN,PURPLE_G_MAX,PURPLE_B_MIN,PURPLE_B_MAX);
    break;
    case YELLOW:
      cmuCam->trackColor(YELLOW_R_MIN,YELLOW_R_MAX,YELLOW_G_MIN,YELLOW_G_MAX,YELLOW_B_MIN,YELLOW_B_MAX);
    break;
    case WHITE:
      cmuCam->trackColor(WHITE_R_MIN,WHITE_R_MAX,WHITE_G_MIN,WHITE_G_MAX,WHITE_B_MIN,WHITE_B_MAX);
    break;
    case GREEN:
      cmuCam->trackColor(GREEN_R_MIN,GREEN_R_MAX,GREEN_G_MIN,GREEN_G_MAX,GREEN_B_MIN,GREEN_B_MAX);
    break;
    default:
    abort();
  }
}
