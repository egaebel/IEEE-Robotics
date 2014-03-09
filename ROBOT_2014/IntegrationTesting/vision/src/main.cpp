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
  {
    #ifdef DEBUG
      printf( "failed to open pan servo\n" );
    #endif

    return 1;
  }
  int tilt_position = INL_TILT_POSITION;
  old_tilt_position = INL_TILT_POSITION;
  if( !enable_servo( TILT_SERVO ) || !set_servo_position( TILT_SERVO, tilt_position ) )
  {
    #ifdef DEBUG
      printf( "failed to open tilt servo\n" );
    #endif

    return 1;
  }

  // open the camera
  VideoCapture cap(0);
  while( !cap.isOpened() )
  {
    #ifdef DEBUG
      printf( "failed to open camera\n" );
    #endif

    cap.open(0);
  }

  // try to set the camera properties
  cap.set(CV_CAP_PROP_FRAME_WIDTH, CAM_WIDTH);
  cap.set(CV_CAP_PROP_FRAME_HEIGHT, CAM_HEIGHT);
  cap.set(CV_CAP_PROP_FPS, CAM_FPS);

  // initialize the camera
  cap.grab();

  int num = 0;
  bool target_found = false;
  int num_fired = 0;
  Mat scene;
  while(num_fired < 3)
  {
    cap >> scene;
    if( scene.data )
    {
      // duplicate scene for later file output
      #ifdef DEBUG
        Mat scene_copy = scene.clone();
      #endif

      cv::Point centroid;
      if( locateTarget( &scene, &centroid ) )
      {
        // draw the detected centroid on the scene and write it to a file
        #ifdef DEBUG
          circle( scene_copy, centroid, 3, Scalar(255, 255, 255) );
          char buffer[10];
          sprintf( buffer, "%d", num );
          string filename = "./snapshot" + string(buffer) + ".png";
          imwrite( filename, scene_copy );
          num++;
        #endif

        // point we are aiming at differs depending on which barrel we are firing
        int pan_difference = 0;
        int tilt_difference = 0;
        switch( fired )
        {
          case 0:
            pan_difference = FIRE_ONE_X - centroid.x;
            tilt_difference = FIRE_ONE_Y - centroid.y;
            break;
          case 1:
            pan_difference = FIRE_TWO_X - centroid.x;
            tilt_difference = FIRE_TWO_Y - centroid.y;
            break;
          case 2:
            pan_difference = FIRE_THREE_X - centroid.x;
            tilt_difference = FIRE_THREE_Y - centroid.y;
            break;
          default:
        }

        // have we aimed close enough yet?
        if( abs( pan_difference ) < PAN_THRESHOLD && abs( tilt_difference ) < TILT_THRESHOLD )
        {
          #ifdef DEBUG
            printf( "FIRE!\n" );
          #endif

          // TODO: toggle ready to fire pin to notify arduino
          usleep( 1000000 );
          // reset ready to fire pin

          fired++;
        }
        else
        {
          // update servo positions (with hard-coded proportional control for now)
          pan_position += PAN_PROPORTIONAL_RATE * pan_difference * PAN_DIRECTION;
          tilt_position += TILT_PROPORTIONAL_RATE * tilt_difference * TILT_DIRECTION;

          // pan/tilt position limits
          if( pan_position < MIN_PAN_POSITION )
            pan_position = MIN_PAN_POSITION;
          else if( pan_position > MAX_PAN_POSITION )
            pan_position = MAX_PAN_POSITION;
          if( tilt_position < MIN_TILT_POSITION )
            tilt_position = MIN_TILT_POSITION;
          else if( tilt_position > MAX_TILT_POSITION )
            tilt_position = MAX_TILT_POSITION;


          if( pan_position != old_pan_position )
          {
            #ifdef DEBUG
              printf( "moving %s: %d to %d (%d)\n", PAN_SERVO, old_pan_position, pan_position, (int)abs( pan_position - old_pan_position ) );
            #endif

            set_servo_position( PAN_SERVO, pan_position );
            old_pan_position = pan_position;
          }
          if( tilt_position != old_tilt_position )
          {
            #ifdef DEBUG
              printf( "moving %s: %d to %d (%d)\n", TILT_SERVO, old_tilt_position, tilt_position, (int)abs( tilt_position - old_tilt_position ) );
            #endif

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

  // disable all servos
  usleep( 500000 );
  disable_servo( PAN_SERVO );
  disable_servo( TILT_SERVO );

  // disconnect the camera
  cap.release();

  return 0;
}
