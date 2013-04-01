#include "cam.h"

cam::cam(int pin){
	cmuCam = new CMUcam4(pin);
}

void cam::init(){
    cmuCam->autoGainControl(false);
    cmuCam->autoWhiteBalance(false);
    cmuCam->begin();
  	// Wait for auto gain and auto white balance to run.
    cmuCam->autoGainControl(false);
    cmuCam->autoWhiteBalance(false);
    cmuCam->cameraBrightness(50);
    cmuCam->cameraContrast(31);
  	cmuCam->LEDOn(LED_BLINK);
  	//delay(WAIT_TIME);

  	// Turn auto gain and auto white balance off.

  	//cmuCam->autoGainControl(false);
  	//cmuCam->autoWhiteBalance(false);

  	cmuCam->LEDOn(CMUCAM4_LED_ON);

  	cmuCam->pollMode(POLL_MODE);
  	cmuCam->colorTracking(YUV_MODE);

  	cmuCam->noiseFilter(NOISE_FILTER);
    curColour = BLACK;
    curBay = NONE;
}

bool cam::inZone(){
    setWindow(LOADING);
  	getTrackingData(WHITE);
  	#define CENTROID_X_MIN 70
  	#define CENTROID_X_MAX 95
  	#define CENTROID_Y_MIN 45`
  	#define CENTROID_Y_MAX 75
  	#define BAY_AREA_MIN 14000
  	#define BAY_AREA_MAX 20000

    #define BOX_X1 14
    #define BOX_X2 137
    #define BOX_Y1 30
    #define BOX_Y2 90
    #define BOX_UNC 10
    //Serial.print("X:");Serial.println(tData.mx);
    //Serial.print("Y:");Serial.println(tData.my);
    int area = (tData.x2-tData.x1)*(tData.y2-tData.y1);
    //Serial.print("area:");Serial.println(area);
    Serial.print("X1:");Serial.println(tData.x1);
    Serial.print("X2:");Serial.println(tData.x2);
    Serial.print("Y1:");Serial.println(tData.y1);
    Serial.print("Y2:");Serial.println(tData.y2);
    
    /*if(tData.mx>CENTROID_X_MIN && tData.mx<CENTROID_X_MAX && tData.my>CENTROID_Y_MIN && tData.my<CENTROID_Y_MAX && area>BAY_AREA_MIN && area<BAY_AREA_MAX)  {
		//tdata1 is point at upper left corner and tdata2 is point at lower right corner
		if(abs(tData.x1 - UPPER_LEFT_CORNER_X) < UNCERTAINTY_ALLOWANCE && abs(tData.x2 - LOWER_RIGHT_CORNER_X) < UNCERTAINTY_ALLOWANCE
			&& abs(tData.y1 - UPPER_LEFT_CORNER_Y) < UNCERTAINTY_ALLOWANCE && abs(tData.y2 - LOWER_RIGHT_CORNER_Y) < UNCERTAINTY_ALLOWANCE)  {
				Serial.println("WE FOUND MICHEAL BAY\n\n\n");
				return 1;
		}
		return 1;
    }*/
    if(tData.x1 < (BOX_X1+BOX_UNC) && (tData.x2+BOX_UNC) > BOX_X1 && tData.x2 < (BOX_X2+BOX_UNC) && (tData.x2+BOX_UNC) > BOX_X2){
        if(tData.y1 < (BOX_Y1+BOX_UNC) && (tData.y2+BOX_UNC) > BOX_Y1 && tData.y2 < (BOX_Y2+BOX_UNC) && (tData.y2+BOX_UNC) > BOX_Y2){
            Serial.println("BILLY BAYS HERE");
            return 1;
        }
    }
	
    return 0;
}

//true if there is only one line on the screen
side cam::betweenZones()  {
	getTrackingData(WHITE);
	
	if((tData.x2-tData.x1) < cmToPx(LINE_WIDTH) + UNCERTAINTY_ALLOWANCE)  {
		if(tData.mx > (trackX2 - trackX1) / 2)  {
      return RIGHT;
    }
    else  {
      return LEFT;
    }
	}
	else  {
		return CENTER;
	}
	
}

//Returns positive if need to move right and negative if need to move left
side cam::locateZone()  {
	getTrackingData(WHITE);

	if(((tData.x2-tData.x1)> cmToPx(BAY_WIDTH + 2*LINE_WIDTH)-UNCERTAINTY_ALLOWANCE) && 
    ((tData.x2-tData.x1)< cmToPx(BAY_WIDTH + 2)+UNCERTAINTY_ALLOWANCE))  {
		if (tData.x2-LOWER_RIGHT_CORNER > 0)
			return RIGHT;
		else if (tdata.x2-LOWER_RIGHT_CORNER < 0)
			return LEFT;
		else
			return CENTER;
	}
}

void cam::getTrackingData(bColour colour){
  if(curColour != colour){
    trackColour(colour);
  }
  cmuCam->getTypeTDataPacket(&tData); // Get a tracking packet
  //Serial.print("pixels");Serial.println(tData.pixels);
  //Serial.print("confidence");Serial.println(tData.confidence);
}
void cam::getTrackingData(){
  cmuCam->getTypeTDataPacket(&tData);
}

bColour cam::getBlockColour(){
  int i;
  //loop through all the colours
  for(i=((int)WHITE)+1;i<(int)BLACK;i++){
    getTrackingData((bColour)i);
    //if we see enough pixels of that colour, we found the block
    if(tData.pixels>cmToPx(BLOCK_WIDTH)){
      return (bColour)i;
    }
  }
}
void cam::setWindow(bay b){
  if(curBay != b){
    switch(b){
      case LOADING:
          cmuCam->setTrackingWindow(0,20,140,100);
      break;
    }
  }  
}
/**
 * Finds absolute value of the param
 */
/*int cam::abs(int a)  {
	if(a < 0) return (-1 * a);
	return a;
}*/
bColour cam::getBayColour(){

}

bSize cam::getBlockSize(bColour colour){

}
void cam::trackColour(bColour colour){
  if(!YUV_MODE){
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
  else{
    switch(colour){
      case YELLOW:
        //cmuCam->trackColor(YELLOW_CR_MIN,YELLOW_CR_MAX,YELLOW_Y_MIN,YELLOW_Y_MAX,YELLOW_CB_MIN,YELLOW_CB_MAX);
      break;
    }
  }
}

int cam::cmToPx(float cm){
  #define CM_TO_PIX_CONV 17
  return cm*CM_TO_PIX_CONV;
}
