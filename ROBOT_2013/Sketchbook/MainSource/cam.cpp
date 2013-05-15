#include "cam.h"

cam::cam(int pin){
	cmuCam = new CMUcam4(pin);
}

void cam::init(){
    Serial.println("cam init CALLED");
    cmuCam->begin();
    cmuCam->autoGainControl(false);
    cmuCam->autoWhiteBalance(false);

    cmuCam->cameraBrightness(CAM_BRIGHTNESS);
    cmuCam->cameraContrast(CAM_CONTRAST);
    cmuCam->pollMode(0);
  	//cmuCam->colorTracking(YUV_MODE);

  	cmuCam->noiseFilter(NOISE_FILTER);
    trackX1 = 0;
    trackX2 = 159;
    trackY1 = 0;
    trackY2 = 119;
    curColour = BLACK;
    curPos = POS_START;
}
void cam::initLeft(){
  init();
  cmuCam->horizontalMirror(1);
}
side cam::inZone(bPosition pos){
    setWindow(pos);
    getTrackingData(WHITE);
    
    //Serial.print("X:");Serial.println(tData.mx);
    //Serial.print("Y:");Serial.println(tData.my);
    int area = (tData.x2-tData.x1)*(tData.y2-tData.y1);
    //Serial.print("area:");Serial.println(area);
/*    Serial.print("X1:");Serial.println(tData.x1);
    Serial.print("X2:");Serial.println(tData.x2);
    Serial.print("Y1:");Serial.println(tData.y1);
    Serial.print("Y2:");Serial.println(tData.y2);*/

    int boundingCentroidX = ((tData.x2 - tData.x1) / 2)+tData.x1;
    int windowCentroidX = (trackX2 - trackX1) / 2;
    
    Serial.print("bounding:");Serial.println(boundingCentroidX);
    Serial.print("window:");Serial.println(windowCentroidX);
    Serial.print("ACTUAL WINDOW:: ");Serial.println(tData.x2 - tData.x1);
    Serial.print("BAY SIZE:: ");Serial.println(cmToPx(BAY_WIDTH+LINE_WIDTH*2));
    if((tData.x2-tData.x1)+UNCERTAINTY_ALLOWANCE >= cmToPx(BAY_WIDTH+LINE_WIDTH*2))  {

        if(boundingCentroidX > windowCentroidX + UNCERTAINTY_ALLOWANCE)  {
            return RIGHT;
        }
        else if(boundingCentroidX < windowCentroidX - UNCERTAINTY_ALLOWANCE)  {
            return LEFT;
        }
        else {
            return CENTER;
        }
    }
    else {
        return NO_SIDE;
    }
}

//true if there is only one line on the screen
bool cam::betweenZones()  {
	getTrackingData(WHITE);
	
	if((tData.x2-tData.x1) < cmToPx(LINE_WIDTH) + UNCERTAINTY_ALLOWANCE)  {
    return true;
	}
	else  {
    return false;
	}
	
}

//Returns positive if need to move right and negative if need to move left
side cam::locateZone()  {
	/**getTrackingData(WHITE);

if(((tData.x2-tData.x1)> cmToPx(BAY_WIDTH + 2*LINE_WIDTH)-UNCERTAINTY_ALLOWANCE) && 
    ((tData.x2-tData.x1)< cmToPx(BAY_WIDTH + 2*LINE_WIDTH)+UNCERTAINTY_ALLOWANCE))  {
		if (tData.x2 - LOWER_RIGHT_CORNER_X + UNCERTAINTY_ALLOWANCE > 0)
			return RIGHT;
		else if (tData.x2 - LOWER_RIGHT_CORNER_X - UNCERTAINTY_ALLOWANCE < 0)
			return LEFT;
		else
			return CENTER;
	}
    return CENTER;*/
}

void cam::getTrackingData(bColour colour){
  if(curColour != colour){
    trackColour(colour);
    curColour = colour;
  }
  cmuCam->getTypeTDataPacket(&tData); // Get a tracking packet
  //Serial.print("pixels");Serial.println(tData.pixels);
  //Serial.print("confidence");Serial.println(tData.confidence);
}
void cam::getTrackingData(){
  cmuCam->getTypeTDataPacket(&tData);
}


void cam::setWindow(bPosition pos){
  if(curPos != pos){
    switch(pos){
      case POS_PICK_UP:
          trackX1 = 0;
          trackY1 = 20;
          trackX2 = cmToPx(BAY_WIDTH+LINE_WIDTH*4);
          trackY2 = 100;
          break;
      case POS_SEA:
          trackX1 = 0;
          trackX2 = 159;
          trackY1 = 0;
          trackY2 = 50;
          break;
    }
    cmuCam->setTrackingWindow(trackX1+PIXEL_X_OFFSET,trackY1,trackX2+PIXEL_X_OFFSET,trackY2);
  }  
}

bColour cam::getBlockColour(){
  return getColour(30);
}

bColour cam::getBayColour(){
  return getColour(20);
}

bColour cam::getColour(int pixelDense){
  int i;
  //loop through all the colours
  for(i=((int)WHITE)+1;i<(int)BLACK;i++){
    getTrackingData((bColour)i);
    //if we see enough pixels of that colour, we found the block
    if(tData.pixels>pixelDense){
      return (bColour)i;
    }
  }
  return BROWN; //devault to brown cause why not
}

//chunk up that view
bSize cam::getBlockSize(bColour colour){
  //check the top part of the block, if we see enough its a rail!
  cmuCam->setTrackingWindow(40+PIXEL_X_OFFSET,0,120+PIXEL_X_OFFSET,30);
  getTrackingData(colour);
  if(tData.pixels>25){
    return LARGE;
  }
  //check the top+ a little more, if we see neough its a sea!
  cmuCam->setTrackingWindow(40+PIXEL_X_OFFSET,0,120+PIXEL_X_OFFSET,80);
  getTrackingData(colour);
  if(tData.pixels>25){
    return MED;
  }
  //else it's an air!
  else{
    return SMALL;
  }
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
  #define CM_TO_PIX_CONV 14
  return cm*CM_TO_PIX_CONV;
}
