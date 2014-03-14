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

int main()
{
	int pan_position = INL_PAN_POSITION;
	int tilt_position = INL_TILT_POSITION;
	int locate_failures = 0;	
	int num = 0;
  int movAmt = 5000;
  int notFoundCount = 0;
  int adjAmt = 20000;
	Mat scene;

	//enum {init, fire1, fire2, fire3} mode;

	if( !enable_servo( PAN_SERVO ) || !set_servo_position( PAN_SERVO, INL_PAN_POSITION ) )
	{
    	#ifdef DEBUG
      		printf( "failed to open pan servo\n" );
    	#endif

  	return 1;
	}
  	
  	if( !enable_servo( TILT_SERVO ) || !set_servo_position( TILT_SERVO, INL_TILT_POSITION ) )
  	{
    	#ifdef DEBUG
      		printf( "failed to open tilt servo\n" );
    	#endif

    	return 1;
    }
    /*if( !GPIOExport( GPIO_P8_31 ) || 
    	!setGPIODirection( GPIO_P8_31, GPIO_P8_31_DIR ) || 
    	!setGPIOValue( GPIO_P8_31, "0" ) )
  	{
    	#ifdef DEBUG
      		printf( "failed to open GPIO P8_31 as output\n" );
    	#endif

    	return 1;
  	}
  	if( !GPIOExport( GPIO_P9_12 ) || !setGPIODirection( GPIO_P9_12, GPIO_P9_12_DIR ) )
  	{
   		#ifdef DEBUG
      		printf( "failed to open GPIO P9_12 as input\n" );
    	#endif

    	return 1;
  	}*/

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

  	while(1)
  	{
  		cap >> scene;

  		if( scene.data )
    	{
    		cv::Point centroid;
    		
    		if( locateTarget( &scene, &centroid, num ) )
      		{
      			locate_failures = 0;
            notFoundCount = 0;

            printf("located that fucking target, preparing to fuck it...\n");
      			printf("x: %d y: %d\n", centroid.x, centroid.y);
            printf("pan: %d tilt: %d\n", pan_position, tilt_position);

      			if(centroid.x < 320) //left
      				pan_position -= movAmt;
      			if(centroid.x > 320) //right
      				pan_position += movAmt;
      			if(centroid.y < 240) //up
      				tilt_position -= movAmt;
      			if(centroid.y > 240) //down
      				tilt_position += movAmt;

      			set_servo_position( PAN_SERVO, pan_position );
      			set_servo_position( TILT_SERVO, tilt_position );
      		
      		} 
      		else
      		{
        		locate_failures++;
        		#ifdef DEBUG
          			printf("target not found\n");
        		#endif

        		// we seem to have completely lost the target and ended up in a bad position so return to initial position
        		if( locate_failures > LOCATE_FAIL_THRESHOLD )
        		{
          			set_servo_position( PAN_SERVO, INL_PAN_POSITION );
          			set_servo_position( TILT_SERVO, INL_TILT_POSITION );
          			locate_failures = 0;
                notFoundCount++;

                if(notFoundCount > 20);
                {
                  set_servo_position(PAN_SERVO + adjAmt);
                }
        		}
      		}
  		}
  		else
    	{
      		#ifdef DEBUG
        		printf( "failed to capture image\n");
     		#endif
    	}

      num++;
  	}

  	// disable all servos
  	usleep( 500000 );
 	  disable_servo( PAN_SERVO );
	  disable_servo( TILT_SERVO );

	  // disconnect the GPIOs
  	//GPIOUnexport( GPIO_P8_31 );
  	//GPIOUnexport( GPIO_P9_12 );

  	// disconnect the camera
  	cap.release();

  	return 0;
}
