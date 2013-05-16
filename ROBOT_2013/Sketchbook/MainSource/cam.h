#ifndef _CAM_H_
#define _CAM_H_

#include "CMUcam4.h"
#include "CMUcom4.h"
#include "common.h"
#include "cmu_define.h"

//Hard cordinate values of the corners of the bays
#define UPPER_LEFT_CORNER_X	0
#define UPPER_LEFT_CORNER_Y	0
#define LOWER_RIGHT_CORNER_X	0
#define LOWER_RIGHT_CORNER_Y	0
#define UNCERTAINTY_ALLOWANCE 8


#define LED_BLINK 5 // 5 Hz
#define WAIT_TIME 5000 // 5 seconds

#define POLL_MODE false // See the command interface manual about poll mode.
#define YUV_MODE false // See the command interface manual about yuv mode.

#define NOISE_FILTER 1

#define PIXELS_THRESHOLD 1
#define CONFIDENCE_THRESHOLD 50


class cam{

	public:
		cam(int pin);
		void init();
		void initLeft();
		side inZone(bPosition pos=POS_PICK_UP);
        bool betweenZones();
        side locateZone(); 
		bColour getBlockColour();
		bColour getBayColour();
		bSize getBlockSize(bColour colour);
		void getTrackingData(bColour colour);
		void getTrackingData();
		void setWindow(bPosition pos);
		
	private:
		int trackX1;
		int trackX2;
		int trackY1;
		int trackY2;
		bColour getColour(int pixels, int pixelConf);
		bColour curColour;
		bPosition curPos;
		void trackColour(bColour colour);

        int cmToPx(float cm);
		CMUcam4* cmuCam;
		CMUcam4_tracking_data_t tData; //stands for Travis Data -- NO IT DOES NOT
};

#endif
