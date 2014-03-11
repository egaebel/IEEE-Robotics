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


class Cam{

	public:
		Cam(int pin);
		void init();
		void initLeft();
		void getTrackingData(CmuColor color);
		void getTrackingData();
		void setTrackingWindow(int trackX1, int trackY1, int trackX2, int trackY2);
		CmuColor getColor(int pixels, int pixelConf);
		CmuColor getColorWrapper(int pixels);
	private:
		int trackX1;
		int trackX2;
		int trackY1;
		int trackY2;
		CmuColor curColor;
		void trackColor(CmuColor color);

        int cmToPx(float cm);
		CMUcam4 * cmuCam;
		CMUcam4_tracking_data_t tData; //stands for Travis Data -- NO IT DOES NOT
};

#endif
