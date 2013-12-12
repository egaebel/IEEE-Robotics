/**
 * @file aiming_demo
 * @brief Canny + findContours + convexHull
 * @author D. Butenhoff
 */

#include <cstdio>
#include <cmath>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

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

const int ERODE_SIZE = 3;
const int DILATE_SIZE = 5;

// hull size filter range
const int MIN_SIZE = 100; // one corner of the target at furthest distance is about 180 square pixels
const int MAX_SIZE = 1000; // one corner of the target at closest distance is about 650 square pixels

// hull proximity distance filter limit
const int PROXIMITY_LIMIT = 70; // diagonal corners of the target at closest distance is about 60 pixels

// HS-475HB range is approximately 550000 to 2550000
// center should be around 1550000
int old_pan_position;
const char PAN_SERVO[9] = "P8_13.15";
const int MIN_PAN_POSITION = 800000;
const int INL_PAN_POSITION = 1400000;
const int MAX_PAN_POSITION = 2000000;
const int PAN_DIRECTION = -1;

int old_tilt_position;
const char TILT_SERVO[9] = "P9_14.16";
const int MIN_TILT_POSITION = 1150000;
const int INL_TILT_POSITION = 1300000;
const int MAX_TILT_POSITION = 1450000;
const int TILT_DIRECTION = 1;

const char FIRE_ONE_SERVO[9] = "P9_21.17";
const int REST_FIRE_ONE_POSITION = 1290000;
const int FIRE_FIRE_ONE_POSITION = 1200000;
const int FIRE_ONE_X = 150;
const int FIRE_ONE_Y = 120;

bool enable_servo( const char* servo );
bool set_servo_position( const char* servo, int position );
bool check_servo_range( const char* servo, int min_position, int ctr_position, int max_position );
bool compareContourAreas( std::vector<cv::Point> contour1, std::vector<cv::Point> contour2 );

/**
 * @function main
 * @brief Main function
 */
int main()
{
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

  printf( "checking tilt servo\n" );

  check_servo_range( TILT_SERVO, MIN_TILT_POSITION, INL_TILT_POSITION, MAX_TILT_POSITION );

  printf( "checking pan servo\n" );

  check_servo_range( PAN_SERVO, MIN_PAN_POSITION, INL_PAN_POSITION, MAX_PAN_POSITION );

  printf( "initializing camera\n" );

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

  bool target_found = false;
  Mat scene;
  while(1)
  {
    cap >> scene;
    if( scene.data )
    {
      Mat scene_copy = scene.clone();

      // convert the scene to HSV
      cvtColor( scene, scene, CV_BGR2HSV );

      // filter the scene by hue
      Mat upr_scene, lwr_scene;
      inRange( scene, Scalar( UPPER_HUE, LOWER_SAT, LOWER_VAL ), Scalar( MAX_HUE, UPPER_SAT, UPPER_VAL ), upr_scene );
      inRange( scene, Scalar( MIN_HUE, LOWER_SAT, LOWER_VAL ), Scalar( LOWER_HUE, UPPER_SAT, UPPER_VAL ), lwr_scene );
      add( upr_scene, lwr_scene, scene );

      // erode/dilate to remove small noise clusters
      erode( scene, scene, getStructuringElement( MORPH_ELLIPSE, Size ( ERODE_SIZE, ERODE_SIZE ) ) );
      dilate( scene, scene, getStructuringElement( MORPH_ELLIPSE, Size ( DILATE_SIZE, DILATE_SIZE ) ) );

      // detect the edges using Canny
      Canny( scene, scene, 100, 200, 3 );

      // find contours
      vector<vector<Point> > contours;
      vector<Vec4i> hierarchy;
      findContours( scene, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

      // normalize contours into convex hulls
      vector<vector<Point> > hulls( contours.size() );
      for( unsigned int i = 0; i < contours.size(); i++ )
        convexHull( Mat(contours[i]), hulls[i] );

      // calculate area for each hull
      vector<double> areas( hulls.size() );
      for( int i = hulls.size() - 1; i >= 0; i-- )
        areas[i] = contourArea( hulls[i] );

      // filter hulls quickly for size
      for( int i = hulls.size() - 1; i >= 0; i-- )
      {
        if( areas[i] < MIN_SIZE || areas[i] > MAX_SIZE )
        {
          hulls.erase( hulls.begin() + i );
          areas.erase( areas.begin() + i );
        }
      }

      // sort by largest contours
      std::sort(hulls.begin(), hulls.end(), compareContourAreas);

      // calculate center of mass for each remaining hull
      vector<Point> cms( hulls.size() );
      for( int i = hulls.size() - 1; i >= 0; i-- )
      {
        Moments mu_i = moments( hulls[i] );
        cms[i] = Point( mu_i.m10/mu_i.m00 , mu_i.m01/mu_i.m00 );
      }

      // filter hulls quickly for overlap, approximated by centers vs root area of larger hull
      for( int i = hulls.size() - 1; i >= 0; i-- )
      {
        for( int j = hulls.size() - 1; j > i; j-- )
        {
          double cm_dist = sqrt( pow( (cms[i].x - cms[j].x), 2 ) + pow( (cms[i].y - cms[j].y), 2 ) );
          double root_area = sqrt( fmax( areas[i], areas[j] ) );
          if( cm_dist < root_area )
          {
            hulls.erase( hulls.begin() + j );
            cms.erase( cms.begin() + j );
            areas.erase( areas.begin() + j );
          }
        }
      }

      bool removed_something = true;
      while( removed_something )
      {
        removed_something = false;
        for( int i = hulls.size() - 1; i >= 0; i-- )
        {
          // calculate distance to every other center and increment counter if less than limit
          // if counter less than three, remove hull from list and set removal flag to true
          int proximate = 0;
          for( int j = hulls.size() - 1; j >= 0; j-- )
          {
            double cm_dist = sqrt( pow( (cms[i].x - cms[j].x), 2 ) + pow( (cms[i].y - cms[j].y), 2 ) );
            // calculate distance to every other center and increment counter if less than limit
            if( cm_dist < PROXIMITY_LIMIT )
            {
              // filter for proximity to at least three other hulls
              proximate++;
            }
          }
          // if fewer than three proximate hulls besides itself, remove hull from list and set removal flag to true
          if( proximate < 4 )
          {
            hulls.erase( hulls.begin() + i );
            cms.erase( cms.begin() + i );
            areas.erase( areas.begin() + i );
            removed_something = true;
          }
        }
      }

      // calculate vertex centroid of top four centers of mass
      Point centroid;
      if( hulls.size() >= 4 )
      {
        centroid = Point( (cms[0].x + cms[1].x + cms[2].x + cms[3].x) / 4,
                          (cms[0].y + cms[1].y + cms[2].y + cms[3].y) / 4 );

        #ifdef DEBUG
          if( !target_found )
          {
            target_found = true;
            printf( "target reaquired at (%d, %d)\n", centroid.x, centroid.y );
          }
          circle( scene_copy, centroid, 3, Scalar(255, 255, 255) );
          char buffer[10];
          sprintf( buffer, "%d", num );
          string filename = "./snapshot" + string(buffer) + ".png";
          imwrite( filename, scene_copy );
          num++;
        #endif

        // camera is 75deg/320px at 320x240
//        double degrees_per_pixel = 75.0 / 320;

        // duty cycle should be about 10000 per degree but that is too high
//        double pan_cycles_per_degree = 6000;
//        double tilt_cycles_per_degree = 3000;

        // allow an error margin since detected centroid may shift slightly in multiple photos at the same position
//        int margin = 2;

        // HS485HB is 0.23sec/60deg at 4.8V with no load, with load it should be higher
//        double sixty_speed = 0.60;

//        double seconds_per_degree = sixty_speed / 60;
//        double us_per_second = 1000000;
//        double us_per_pixel = degrees_per_pixel * seconds_per_degree * us_per_second;
//        double frame_rate = 30.0;

        // more precisely, us per shot
//        double shot_time_us = us_per_second / frame_rate;

//        int pan_cycles_per_shot = (int)round( ( ( pan_cycles_per_degree / seconds_per_degree ) / us_per_second ) * shot_time_us );

        // difference of pan and tilt from center in pixels
//        int pan_difference = ( scene.size().width / 2 ) - centroid.x;
//        int tilt_difference = ( scene.size().height / 2 ) - centroid.y;

        // total movement time should be about one clock pulse plus approximate microseconds needed for servo movement
//        double movement_time_us = 20000 + ( fmax( abs( pan_difference ), abs( tilt_difference ) ) * us_per_pixel );
//        int shots = (int)ceil( movement_time_us / shot_time_us );

        // pan_remaining is in cycles
//        int pan_remaining = 0, pan_steps = 0;
//        int tilt_remaining = 0, tilt_steps = 0;

//        int goal_x = scene.size().width / 2;
        int goal_x = FIRE_ONE_X;
//        int goal_y = scene.size().height / 2;
        int goal_y = FIRE_ONE_Y;

        int pan_difference = goal_x - centroid.x;
        pan_position += 100 * pan_difference * PAN_DIRECTION;
        if( pan_position < MIN_PAN_POSITION )
          pan_position = MIN_PAN_POSITION;
        else if( pan_position > MAX_PAN_POSITION )
          pan_position = MAX_PAN_POSITION;

        int tilt_difference = goal_y - centroid.y;
        tilt_position += 100 * tilt_difference * TILT_DIRECTION;
        if( tilt_position < MIN_TILT_POSITION )
          tilt_position = MIN_TILT_POSITION;
        else if( tilt_position > MAX_TILT_POSITION )
          tilt_position = MAX_TILT_POSITION;

        if( abs( pan_difference ) < 4 && abs( tilt_difference ) < 4 )
        {
          printf( "FIRE!\n" );
          set_servo_position( FIRE_ONE_SERVO, FIRE_FIRE_ONE_POSITION );
        }

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

        // wait by pulling shots from the camera buffer while servo is moving
//        for( int i = 0; i < shots; i++ )
//        {
          cap >> scene;
//        }
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

  return 0;
}

/*
 * comparison function object for sorting in decreasing order
 */
bool compareContourAreas ( std::vector<cv::Point> contour1, std::vector<cv::Point> contour2 ) {
    double i = fabs( contourArea(cv::Mat(contour1)) );
    double j = fabs( contourArea(cv::Mat(contour2)) );
    return ( i > j );
}

bool enable_servo( const char* servo )
{
  FILE *myOutputHandle = NULL;

  // enable pwm hardware
  if( ( myOutputHandle = fopen( "/sys/devices/bone_capemgr.9/slots", "rb+" ) ) == NULL ) {
    printf( "Unable to open cape manager to enable pwm hardware\n" );
    return false;
  }
  fwrite( "am33xx_pwm", sizeof(char), 10, myOutputHandle );
  fflush( myOutputHandle );
  char pin[6];
  strncpy( pin, servo, 5 );
  char setValue[64];
  int length = sprintf( setValue, "bone_pwm_%s", pin );
  fwrite( &setValue, sizeof(char), length, myOutputHandle );
  fclose( myOutputHandle );

  // revisit: the pin takes some time to show up
  //usleep( 500000 );

  // set positive polarity
  char polarity_handle[64];
  sprintf( polarity_handle, "/sys/devices/ocp.3/pwm_test_%s/polarity", servo );
  if( ( myOutputHandle = fopen( polarity_handle, "rb+" ) ) == NULL ) {
    printf( "Unable to open polarity handle %s\n", polarity_handle );
    return false;
  }
  fwrite( "0", sizeof(char), 1, myOutputHandle );
  fclose( myOutputHandle );

  // set 20ms period for 50Hz frequency
  char period_handle[64];
  sprintf( period_handle, "/sys/devices/ocp.3/pwm_test_%s/period", servo );
  if( ( myOutputHandle = fopen( period_handle, "rb+" ) ) == NULL ) {
    printf( "Unable to open period handle\n" );
    return false;
  }
  fwrite( "20000000", sizeof(char), 8, myOutputHandle );
  fclose( myOutputHandle );

  // activate pwm output pin
  char run_handle[64];
  sprintf( run_handle, "/sys/devices/ocp.3/pwm_test_%s/run", servo );
  if( ( myOutputHandle = fopen( run_handle, "rb+" ) ) == NULL ) {
    printf( "Unable to open run handle\n" );
    return false;
  }
  fwrite( "1", sizeof(char), 1, myOutputHandle );
  fclose( myOutputHandle );
  return true;
}

bool set_servo_position( const char* servo, int position )
{
  FILE *myOutputHandle = NULL;
  char duty_handle[64];
  sprintf( duty_handle, "/sys/devices/ocp.3/pwm_test_%s/duty", servo );
  if( ( myOutputHandle = fopen( duty_handle, "rb+" ) ) == NULL ) {
    printf( "Unable to open duty handle\n" );
    return false;
  }
  char setValue[8];
  int length = sprintf( setValue, "%d", position );
  fwrite( &setValue, sizeof(char), length, myOutputHandle );
  fclose( myOutputHandle );
  return true;
}

bool check_servo_range( const char* servo, int min_position, int ctr_position, int max_position )
{
  set_servo_position( servo, min_position );
  usleep( 500000 );
  set_servo_position( servo, max_position );
  usleep( 500000 );
  set_servo_position( servo, ctr_position );
  return true;
}

bool GPIOExport( int gpio_pin )
{
  FILE *myOutputHandle = NULL;
  char export_handle[4];
  sprintf( export_handle, "%d", gpio_pin );
  if ((myOutputHandle = fopen("/sys/class/gpio/export", "ab")) == NULL){
      printf("Unable to export GPIO pin\n");
      return 1;
  }
  char setValue[4];
  strcpy( setValue, export_handle );
  fwrite( &setValue, sizeof(char), 2, myOutputHandle );
  fclose( myOutputHandle );
  return true;
}

bool setGPIODirection( int gpio_pin, const char *setValue )
{
  FILE *myOutputHandle = NULL;
  char direction_handle[64];
  sprintf( direction_handle, "/sys/class/gpio/gpio%d/direction", gpio_pin );
  if ( ( myOutputHandle = fopen( direction_handle, "rb+" ) ) == NULL ) {
      printf( "Unable to open direction handle\n" );
      return false;
  }
  fwrite( setValue, sizeof(char), 3, myOutputHandle );
  fclose( myOutputHandle );
  return true;
}

bool setGPIOValue( int gpio_pin, const char *setValue )
{
  FILE *myOutputHandle = NULL;
  char value_handle[64];
  sprintf( value_handle, "/sys/class/gpio/gpio%d/value", gpio_pin );
  if( ( myOutputHandle = fopen( value_handle, "rb+" ) ) == NULL ) {
    printf( "Unable to open value handle\n" );
    return false;
  }
  fwrite( setValue, sizeof(char), 1, myOutputHandle );
  fclose( myOutputHandle );
  return true;
}
