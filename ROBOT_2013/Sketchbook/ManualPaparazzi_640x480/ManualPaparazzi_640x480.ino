/***************************************************************************//**
* @file
* Manual Paparazzi Demo using 640 x 480 resolution.
*
* @version @n 1.0
* @date @n 8/14/2012
*
* @authors @n Kwabena W. Agyeman
* @copyright @n (c) 2012 Kwabena W. Agyeman
* @n All rights reserved - Please see the end of the file for the terms of use
*
* @par Update History:
* @n v1.0 - Initial Release - 8/14/2012
*******************************************************************************/

#include <CMUcam4.h>
#include <CMUcom4.h>

#define LED_BLINK 2 // 2 Hz
#define LED_SETUP 5 // 5 Hz
#define POSE_TIME 5000 // 5 seconds
#define WAIT_TIME 5000 // 5 seconds

CMUcam4 cam(1);

void setup()
{
  cam.begin();
  Serial.begin(9600);
  Serial.println("begin");
  
  // Wait for auto gain and auto white balance to run.

  cam.LEDOn(LED_SETUP);
  cam.autoGainControl(false);
  cam.autoWhiteBalance(false);
  cam.cameraBrightness(50);
  cam.cameraContrast(10);
  delay(WAIT_TIME);

  cam.LEDOn(CMUCAM4_LED_ON);
  serial.println("setup done");
}

void loop()
{
  
  if(digitalRead(52))
  {
    Serial.println("startingtopicture");
    // Give the user time to pose.
    cam.LEDOn(LED_BLINK);
    delay(POSE_TIME);

    // Then take the picture.
    cam.dumpFrame(CMUCAM4_HR_640, CMUCAM4_VR_480);
    cam.unmountDisk();

    cam.LEDOn(CMUCAM4_LED_ON);
    Serial.println("pictureueuresuccesss");
  }
}

/***************************************************************************//**
* @file
* @par MIT License - TERMS OF USE:
* @n Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"), to
* deal in the Software without restriction, including without limitation the
* rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
* sell copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* @n
* @n The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* @n
* @n THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*******************************************************************************/
