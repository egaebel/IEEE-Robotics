/**
 * @file config
 * @brief configuration and tuning constants
 * @author D. Butenhoff
 */

#ifndef CONFIG_H
#define CONFIG_H

//****************************
// TUNING FOR IMAGE PROCESSING

// FYI: hue ranges from 0 to 179 in OpenCV
const unsigned int MIN_HUE = 0;
const unsigned int LOWER_HUE = 10;
const unsigned int UPPER_HUE = 160;
const unsigned int MAX_HUE = 179;
// FYI: saturation ranges from 0 to 255 in OpenCV
const unsigned int LOWER_SAT = 160;
const unsigned int UPPER_SAT = 255;
// FYI: value ranges from 0 to 255 in OpenCV
const unsigned int LOWER_VAL = 60;
const unsigned int UPPER_VAL = 190;

// hull size filter range
const int MIN_SIZE = 100; // one corner of the target at furthest distance is about 180 square pixels
const int MAX_SIZE = 1000; // one corner of the target at closest distance is about 650 square pixels

// hull proximity distance filter limit
const int PROXIMITY_LIMIT = 70; // diagonal corners of the target at closest distance is about 60 pixels

const int ERODE_SIZE = 3;
const int DILATE_SIZE = 5;

const int CANNY_LOW_THRESHOLD = 100;
const int CANNY_HIGH_THRESHOLD = 200;
const int CANNY_KERNEL_SIZE = 3;

//********************
// SERVO CONFIGURATION
// HS-475HB range is approximately 550000 to 2550000
// but stage range will be smaller...

const char PAN_SERVO[9] = "P8_13.15";
const int MIN_PAN_POSITION = 800000;
const int INL_PAN_POSITION = 1400000;
const int MAX_PAN_POSITION = 2000000;
const int PAN_DIRECTION = -1;
const int PAN_PROPORTIONAL_RATE = 100;
const int PAN_THRESHOLD = 4;

const char TILT_SERVO[9] = "P9_14.16";
const int MIN_TILT_POSITION = 1650000;
const int INL_TILT_POSITION = 1750000;
const int MAX_TILT_POSITION = 2150000;
const int TILT_DIRECTION = 1;
const int TILT_PROPORTIONAL_RATE = 100;
const int TILT_THRESHOLD = 4;

const char FIRE_ONE_SERVO[9] = "P9_21.17";
const int REST_FIRE_ONE_POSITION = 1290000;
const int FIRE_FIRE_ONE_POSITION = 1200000;
const int FIRE_ONE_X = 140;
const int FIRE_ONE_Y = 120;

#endif

