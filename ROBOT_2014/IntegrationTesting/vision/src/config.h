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
#define LOWER_HUE		1
#define UPPER_HUE		7
#define MAX_HUE			179
// FYI: saturation ranges from 0 to 255 in OpenCV
#define LOWER_SAT		242
#define UPPER_SAT		255
// FYI: value ranges from 0 to 255 in OpenCV
#define LOWER_VAL		214
#define UPPER_VAL		249

// hull size filter range
// one corner of the target at furthest distance and 320x240 is about 180 square pixels
#define MIN_SIZE		1040
// one corner of the target at closest distance and 320x240 is about 650 square pixels
#define MAX_SIZE		20250

// hull proximity distance filter limit
// diagonal corners of the target at closest distance and 320x240 is about 60 pixels
#define PROXIMITY_LIMIT		300

// edge tuning
#define ERODE_SIZE		3
#define DILATE_SIZE		5
#define CANNY_LOW_THRESHOLD	100
#define CANNY_HIGH_THRESHOLD	200
#define CANNY_KERNEL_SIZE	3

// camera capture settings, note: if these are changed, they must also be changed in the startup script
#define CAM_WIDTH		640
#define CAM_HEIGHT		480
#define CAM_FPS			5

// GPIO pin 50: GPIO1_18 or pin 14 on the P9 header
#define GPIO_P9_14		50
#define GPIO_P9_14_DIR	"out"

// GPIO pin 60: GPIO1_28 or pin 12 on the P9 header
#define GPIO_P9_12		60
#define GPIO_P9_12_DIR	"in"

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

// tilt configuration
#define TILT_SERVO		"P9_14.16"
#define MIN_TILT_POSITION	1650000
#define INL_TILT_POSITION	1750000
#define MAX_TILT_POSITION	2150000
#define TILT_DIRECTION		1

// tilt movement tuning
#define TILT_PROPORTIONAL_RATE	100
#define TILT_THRESHOLD		4

// config for direct control of firing servo
#define FIRE_ONE_SERVO		"P9_21.17"
#define REST_FIRE_ONE_POSITION	1290000
#define FIRE_FIRE_ONE_POSITION	1200000

// barrel centering coordinates and error margin
// centroid may shift in multiple photos of the same position
#define FIRE_MARGIN		2
#define FIRE_ONE_X		140
#define FIRE_ONE_Y		120
#define FIRE_TWO_X		140
#define FIRE_TWO_Y		120
#define FIRE_THREE_X		140
#define FIRE_THREE_Y		120

// how many frames we should wait after not finding the target at all before returning to initial position
#define LOCATE_FAIL_THRESHOLD	20

//********************
// inertia/acceleration and PID control related servo attributes

// camera is 75deg/320px at 320x240
#define DEGREES_PER_PIXEL	0.234375

// duty cycle should be about 10000 per degree but that is too high for smooth movement
#define PAN_CYCLES_PER_DEGREE	6000
#define TILT_CYCLES_PER_DEGREE	3000


// time to travel 60 degrees. HS485HB is 0.23sec/60deg at 4.8V with no load, with load it should be higher
#define SIXTY_SPEED		0.60

// time to travel 1 degree: SIXTY_SPEED / 60
#define SECONDS_PER_DEGREE	0.01

// microseconds in one second
#define US_PER_SECOND		1000000

// degrees_per_pixel * seconds_per_degree * us_per_second
#define US_PER_PIXEL 		2343.75

#define FRAME_RATE		5.0

// more precisely, us per shot: us_per_second / frame_rate
#define SHOT_TIME_US		200000

// (int)round( ( ( pan_cycles_per_degree / seconds_per_degree ) / us_per_second ) * shot_time_us )
#define PAN_CYCLES_PER_SHOT	120000

// total movement time should be about one clock pulse plus approximate microseconds needed for servo movement
// 20000 + ( fmax( abs( pan_difference ), abs( tilt_difference ) ) * us_per_pixel )
// frame shots processed per movement unit: (int)ceil( movement_time_us / shot_time_us );

#endif

