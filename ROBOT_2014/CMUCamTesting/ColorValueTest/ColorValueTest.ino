
/**
  * FOR TESTING: Just using CMUcam4 class (which cam was a wrapper of)
  */
  
#include "CMUcam4.h"
#include "CMUcom4.h"

CMUcam4 cmuCam(CMUCOM4_SERIAL1);

#define PIXELS_THRESHOLD 1
#define CONFIDENCE_THRESHOLD 50

#define RED_MIN 160
#define RED_MAX 245
#define GREEN_MIN 50
#define GREEN_MAX 110
#define BLUE_MIN 70
#define BLUE_MAX 120



void setup() {
	Serial.begin(19200);
	cmuCam.begin();
}

void loop() {
  
  CMUcam4_tracking_data_t data;
  
  cmuCam.trackColor(RED_MIN, RED_MAX, GREEN_MIN, GREEN_MAX, BLUE_MIN, BLUE_MAX); //Streaming...

  if(data.pixels > PIXELS_THRESHOLD) { // We see the color to track.
    if(data.confidence > CONFIDENCE_THRESHOLD) { // The pixels we are tracking are in a dense clump - so maybe they are a single object.
      // Okay, so we now know we are looking at a clump of pixels that are in the color bounds we wanted to track.

      //**************PROBLEM CODE********************

      Serial.println(data.mx);
      Serial.println(data.my);

      // Might want to make the above functions non-linear as the brightness function of an LED is a non-linear... the effect will look better.
    }
  }
}
