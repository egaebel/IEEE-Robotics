#ifndef _CMU_H_
#define _CMU_H_

#include "CMUcam4.h"
#include "CMUcom4.h"

#define WHITE_R_MIN 200
#define WHITE_R_MAX 255
#define WHITE_G_MIN 200
#define WHITE_G_MAX 255
#define WHITE_B_MIN 235
#define WHITE_B_MAX 255

#define BLUE_R_MIN 70
#define BLUE_R_MAX 140
#define BLUE_G_MIN 100
#define BLUE_G_MAX 170
#define BLUE_B_MIN 165
#define BLUE_B_MAX 255



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
	private:
		void getTrackingData();
		void trackBlue();
		CMUcam4* cmuCam;
		CMUcam4_tracking_data_t tData; //stands for Travis Data
};

#endif