/**
 * @file main
 * @brief main vision loop with hardware pwm pan/tilt/fire control
 * @author D. Butenhoff
 */

#include <cstdio>
#include <cmath>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "target.h"
#include "servo.h"
#include "gpio.h"
#include "config.h"

using namespace std;
using namespace cv;

int old_pan_position;
int old_tilt_position;

/**
 * @function main
 * @brief Main function
 */
int main()
{
	// initialize the servos
	int pan_position = INL_PAN_POSITION;
	old_pan_position = INL_PAN_POSITION;
	if( !enable_servo( PAN_SERVO ) || !set_servo_position( PAN_SERVO, INL_PAN_POSITION ) )
		return 1;
	int tilt_position = INL_TILT_POSITION;
	old_tilt_position = INL_TILT_POSITION;
	if( !enable_servo( TILT_SERVO ) || !set_servo_position( TILT_SERVO, tilt_position ) )
		return 1;
	if( !enable_servo( FIRE_ONE_SERVO ) || !set_servo_position( FIRE_ONE_SERVO, REST_FIRE_ONE_POSITION ) )
		return 1;

	// open the camera
	VideoCapture cap(0);
	if( !cap.isOpened() )
	{
		printf( "failed to open camera\n" );
		return -1;
	}

	// try to set the camera properties
	cap.set(CV_CAP_PROP_FRAME_WIDTH, 320);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 240);
	cap.set(CV_CAP_PROP_FPS, 30);

	// initialize the camera
	cap.grab();

	int num = 0;
	bool target_found = false, fired = false;
	Mat scene;
	while(!fired)
	{
		cap >> scene;
		if( scene.data )
		{
			Mat scene_copy = scene.clone();
			cv::Point centroid;
			if( locateTarget( &scene, &centroid ) )
			{
				#ifdef DEBUG
					// draw the detected centroid on the scene and write it to a file
			        circle( scene_copy, centroid, 3, Scalar(255, 255, 255) );
			        char buffer[10];
			        sprintf( buffer, "%d", num );
			        string filename = "./snapshot" + string(buffer) + ".png";
			        imwrite( filename, scene_copy );
					num++;
		        #endif

				/*
				// inertia/acceleration and PID control related servo attributes

				// camera is 75deg/320px at 320x240
				double degrees_per_pixel = 75.0 / 320;

				// duty cycle should be about 10000 per degree but that is too high
				double pan_cycles_per_degree = 6000;
				double tilt_cycles_per_degree = 3000;

				// allow an error margin since detected centroid may shift slightly in multiple photos at the same position
				int margin = 2;

				// HS485HB is 0.23sec/60deg at 4.8V with no load, with load it should be higher
				double sixty_speed = 0.60;
				double seconds_per_degree = sixty_speed / 60;
				double us_per_second = 1000000;
				double us_per_pixel = degrees_per_pixel * seconds_per_degree * us_per_second;
				double frame_rate = 30.0;

				// more precisely, us per shot
				double shot_time_us = us_per_second / frame_rate;
				int pan_cycles_per_shot = (int)round( ( ( pan_cycles_per_degree / seconds_per_degree ) / us_per_second ) * shot_time_us );

				// difference of pan and tilt from center in pixels
				int pan_difference = ( scene.size().width / 2 ) - centroid.x;
				int tilt_difference = ( scene.size().height / 2 ) - centroid.y;

				// total movement time should be about one clock pulse plus approximate microseconds needed for servo movement
				double movement_time_us = 20000 + ( fmax( abs( pan_difference ), abs( tilt_difference ) ) * us_per_pixel );
				int shots = (int)ceil( movement_time_us / shot_time_us );

				// pan_remaining is in cycles
				int pan_remaining = 0, pan_steps = 0;
				int tilt_remaining = 0, tilt_steps = 0;

				int goal_x = scene.size().width / 2;
				int goal_y = scene.size().height / 2;
				*/

				int pan_difference = FIRE_ONE_X - centroid.x;
				// hard-coded proportional control for now
				pan_position += PAN_PROPORTIONAL_RATE * pan_difference * PAN_DIRECTION;
				if( pan_position < MIN_PAN_POSITION )
					pan_position = MIN_PAN_POSITION;
				else if( pan_position > MAX_PAN_POSITION )
					pan_position = MAX_PAN_POSITION;

				int tilt_difference = FIRE_ONE_Y - centroid.y;
				// hard-coded proportional control for now
				tilt_position += TILT_PROPORTIONAL_RATE * tilt_difference * TILT_DIRECTION;
				if( tilt_position < MIN_TILT_POSITION )
					tilt_position = MIN_TILT_POSITION;
				else if( tilt_position > MAX_TILT_POSITION )
					tilt_position = MAX_TILT_POSITION;

				if( abs( pan_difference ) < PAN_THRESHOLD && abs( tilt_difference ) < TILT_THRESHOLD )
				{
					fired = true;
					printf( "FIRE!\n" );
					set_servo_position( FIRE_ONE_SERVO, FIRE_FIRE_ONE_POSITION );
				}
				else
				{
					// update servo positions
					if( pan_position != old_pan_position )
					{
						printf( "moving %s: %d to %d (%d)\n", PAN_SERVO, old_pan_position, pan_position, (int)abs( pan_position - old_pan_position ) );
						set_servo_position( PAN_SERVO, pan_position );
						old_pan_position = pan_position;
					}
					if( tilt_position != old_tilt_position )
					{
						printf( "moving %s: %d to %d (%d)\n", TILT_SERVO, old_tilt_position, tilt_position, (int)abs( tilt_position - old_tilt_position ) );
						set_servo_position( TILT_SERVO, tilt_position );
						old_tilt_position = tilt_position;
					}
				}
			}
			else
			{
		        #ifdef DEBUG
					if( target_found )
					{
						target_found = false;
						printf("target not found\n");
					}
		        #endif
			}
		}
	}

	// wait for firing servo to trigger, then back it off
	usleep( 500000 );
	set_servo_position( FIRE_ONE_SERVO, REST_FIRE_ONE_POSITION );

	// disable all servos
	usleep( 500000 );
	disable_servo( PAN_SERVO );
	disable_servo( TILT_SERVO );
	disable_servo( FIRE_ONE_SERVO );

	return 0;
}
