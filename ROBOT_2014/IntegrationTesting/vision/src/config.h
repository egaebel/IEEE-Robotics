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
#define MIN_HUE			0
#define LOWER_HUE		10
#define UPPER_HUE		160
#define MAX_HUE			179
// FYI: saturation ranges from 0 to 255 in OpenCV
#define LOWER_SAT		160
#define UPPER_SAT		255
// FYI: value ranges from 0 to 255 in OpenCV
#define LOWER_VAL		60
#define UPPER_VAL		190


// hull size filter range
// one corner of the target at furthest distance and 320x240 is about 180 square pixels
#define MIN_SIZE		100
// one corner of the target at closest distance and 320x240 is about 650 square pixels
#define MAX_SIZE		1000

// hull proximity distance filter limit
// diagonal corners of the target at closest distance and 320x240 is about 60 pixels
#define PROXIMITY_LIMIT		70

// edge tuning
#define ERODE_SIZE		3
#define DILATE_SIZE		5
#define CANNY_LOW_THRESHOLD	100
#define CANNY_HIGH_THRESHOLD	200
#define CANNY_KERNEL_SIZE	3

//********************
// SERVO CONFIGURATION
// HS-475HB range is approximately 550000 to 2550000
// but stage range will be smaller...

// pan configuration
#define PAN_SERVO		"P8_13.15"
#define MIN_PAN_POSITION	800000
#define INL_PAN_POSITION	1400000
#define MAX_PAN_POSITION	2000000
#define PAN_DIRECTION		-1

// pan movement tuning
#define PAN_PROPORTIONAL_RATE	100
#define PAN_THRESHOLD		4

#define TILT_SERVO		"P9_14.16"
#define MIN_TILT_POSITION	1650000
#define INL_TILT_POSITION	1750000
#define MAX_TILT_POSITION	2150000
#define TILT_DIRECTION		1
#define TILT_PROPORTIONAL_RATE	100
#define TILT_THRESHOLD		4

#define FIRE_ONE_SERVO		"P9_21.17"
#define REST_FIRE_ONE_POSITION	1290000
#define FIRE_FIRE_ONE_POSITION	1200000
#define FIRE_ONE_X		140
#define FIRE_ONE_Y		120

#endif

