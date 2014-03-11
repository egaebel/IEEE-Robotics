#include "cam.h"

Cam::Cam(int pin){
	cmuCam = new CMUcam4(pin);
}

//Initialize settings in the camera
//settings may need to be tweaked for our purposes
void Cam::init(){

    cmuCam->begin();/*
    //Serial.println("monitorOn called in init");*/
    cmuCam->autoGainControl(true);
    //Serial.println("autoGainControl called in init");
    cmuCam->autoWhiteBalance(true);
    //Serial.println("autoWhiteBalance called in init");

    cmuCam->cameraBrightness(CAM_BRIGHTNESS);
    //Serial.println("cameraBrightness called in init");
    cmuCam->cameraContrast(CAM_CONTRAST);
    //Serial.println("camContrast called in init");
    cmuCam->pollMode(0);
    //Serial.println("pollMode called in init");

    cmuCam->noiseFilter(NOISE_FILTER);
    /*trackX1 = 0;
    trackX2 = 500;
    trackY1 = 0;
    trackY2 = 500;
    cmuCam->setTrackingWindow(trackX1, trackY1, trackX2, trackY2);
    curColor = RED;//;WHITE;
    //Serial.println("ending init");
    delay(5000);*/
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
  for(i = ((int) WHITE) + 1; i < (int)BLACK; i++){

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
  return RED; //devault to red cause why not
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
    case WHITE:
      cmuCam->trackColor(WHITE_R_MIN,WHITE_R_MAX,WHITE_G_MIN,WHITE_G_MAX,WHITE_B_MIN,WHITE_B_MAX);
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
