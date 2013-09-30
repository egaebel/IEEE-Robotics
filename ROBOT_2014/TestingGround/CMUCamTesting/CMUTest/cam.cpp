#include "cam.h"

Cam::Cam(int pin){
	cmuCam = new CMUcam4(pin);
}

//Initialize settings in the camera
//settings may need to be tweaked for our purposes
void Cam::init(){
    Serial.println("cam init CALLED");
    cmuCam->begin();
    cmuCam->autoGainControl(false);
    cmuCam->autoWhiteBalance(false);

    cmuCam->cameraBrightness(CAM_BRIGHTNESS);
    cmuCam->cameraContrast(CAM_CONTRAST);
    cmuCam->pollMode(0);

  	cmuCam->noiseFilter(NOISE_FILTER);
    trackX1 = 0;
    trackX2 = 500;
    trackY1 = 0;
    trackY2 = 500;
    cmuCam->setTrackingWindow(trackX1, trackY1, trackX2, trackY2);
    curColor = WHITE;
}

//???????????????
//I think this initializes the came as being on the left......
void Cam::initLeft(){
  init();
  cmuCam->horizontalMirror(1);
}

//Sets the area for the CMU cam to track
void Cam::setTrackingWindow(int trackX1, int trackY1, int trackX2, int trackY2) {

    cmuCam->setTrackingWindow(trackX1, trackY1, trackX2, trackY2);
}

void Cam::getTrackingData(CmuColor color){
  if(curColor != color){
    trackColor(color);
    curColor = color;
  }

  cmuCam->getTypeTDataPacket(&tData); // Get a tracking packet
}

void Cam::getTrackingData(){
  cmuCam->getTypeTDataPacket(&tData);
}

CmuColor Cam::getColorWrapper(int pixelDense) {

  getColor(pixelDense, 0);
  return getColor(pixelDense, 0);
}

//Gets the color that we are currently reading
CmuColor Cam::getColor(int pixelDense, int pixelConf){
  
  memset(&tData, '\0', sizeof(tData));
  int i;
  int highest = -1;
  int highestIndex = -1;
  int temp;
  //loop through all the colors
  for(i = ((int) WHITE) + 1;i < (int)BLACK; i++){

    getTrackingData((CmuColor) i);
    temp = tData.pixels;
    if(temp > highest) {// && tData.confidence > pixelConf) {
      highest = temp;  
      highestIndex = i;
    }

    //if we see enough pixels of that color, we found the block
    Serial.print("enum is:: "); Serial.print(i); Serial.print("\n");
    Serial.print("Pixels:: "); Serial.print(temp); Serial.print("\n");
  }
Serial.print("highest is:: "); Serial.print(highest);
Serial.print("\n");
Serial.print("pixelDense"); Serial.println(pixelDense);
  return (CmuColor) highestIndex;
  return BROWN; //devault to brown cause why not
}

//Sets the cmu cam to track the passed in color
void Cam::trackColor(CmuColor color){

  switch(color){
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

//Converts cm to pixels
int Cam::cmToPx(float cm) {
  #define CM_TO_PIX_CONV 14
  return cm * CM_TO_PIX_CONV;
}
