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

#define NOISE_FILTER 2

#define PIXELS_THRESHOLD 1
#define CONFIDENCE_THRESHOLD 50

typedef enum bay_t{
	LOADING, 
	AIR, 
	RAIL,
	SEA,
	NONE
} bay;

class cam{

	public:
		cam(int pin);
		void init();
		side inZone();
        bool betweenZones();
        side locateZone(); 
		bColour getBlockColour();
		bColour getBayColour();
		bSize getBlockSize(bColour colour);
		void getTrackingData(bColour colour);
		void getTrackingData();
	private:
		int trackX1;
		int trackX2;
		int trackY1;
		int trackY2;

		bColour curColour;
		bay curBay;
		void trackColour(bColour colour);
		void setWindow(bay b);
        int cmToPx(float cm);
		CMUcam4* cmuCam;
		CMUcam4_tracking_data_t tData; //stands for Travis Data
};

#endif
