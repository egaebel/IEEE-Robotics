#ifndef _CMU_H_
#define _CMU_H_

#include "CMUcam4.h"
#include "CMUcom4.h"
#include "common.h"
#include "cmu_define.h"

//Hard cordinate values of the corners of the bays
#define UPPER_LEFT_CORNER_X	0
#define UPPER_LEFT_CORNER_Y	0
#define LOWER_RIGHT_CORNER_X	0
#define LOWER_RIGHT_CORNER_Y	0
#define UNCERTAINTY_ALLOWANCE 5


#define LED_BLINK 5 // 5 Hz
#define WAIT_TIME 5000 // 5 seconds

#define POLL_MODE true // See the command interface manual about poll mode.
#define YUV_MODE false // See the command interface manual about yuv mode.

#define NOISE_FILTER 2

#define PIXELS_THRESHOLD 1
#define CONFIDENCE_THRESHOLD 50


class cam{

	public:
		cam(int pin);
		void init();
		bool inZone();
		bColour getBlockColour();
		bColour getBayColour();
		bSize getBlockSize(bColour colour);
	
	private:
		void trackColour(bColour colour);
		void getTrackingData(bColour colour);
		CMUcam4* cmuCam;
		CMUcam4_tracking_data_t tData; //stands for Travis Data
};

#endif
