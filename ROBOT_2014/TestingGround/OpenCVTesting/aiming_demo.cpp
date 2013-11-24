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
const unsigned int LOWER_HUE = 13;
const unsigned int UPPER_HUE = 157;
const unsigned int MAX_HUE = 179;
// FYI: saturation ranges from 0 to 255 in OpenCV
const unsigned int LOWER_SAT = 163;
const unsigned int UPPER_SAT = 255;
// FYI: value ranges from 0 to 255 in OpenCV
const unsigned int LOWER_VAL = 58;
const unsigned int UPPER_VAL = 122;

// hull size filter range
const int MIN_SIZE = 50; // about 1/4 the size of one corner of the target
const int MAX_SIZE = 5000; // about 4 times the size of one corner of the target

bool compareContourAreas ( std::vector<cv::Point> contour1, std::vector<cv::Point> contour2 );

/**
 * @function main
 * @brief Main function
 */
int main()
{
  // initialize the pan/tilt servo output pins
  int GPIOPin50 = 50; // GPIO1_18 or pin 14 on the P9 header
  int GPIOPin60 = 60; // GPIO1_28 or pin 12 on the P9 header
  FILE *myOutputHandle = NULL;
  char setValue[4];
  char GPIO50String[4], GPIO50Value[64], GPIO50Direction[64];
  sprintf(GPIO50String, "%d", GPIOPin50);
  sprintf(GPIO50Value, "/sys/class/gpio/gpio%d/value", GPIOPin50);
  sprintf(GPIO50Direction, "/sys/class/gpio/gpio%d/direction", GPIOPin50);
  char GPIO60String[4], GPIO60Value[64], GPIO60Direction[64];
  sprintf(GPIO60String, "%d", GPIOPin60);
  sprintf(GPIO60Value, "/sys/class/gpio/gpio%d/value", GPIOPin60);
  sprintf(GPIO60Direction, "/sys/class/gpio/gpio%d/direction", GPIOPin60);
  // Export the pins
  if ((myOutputHandle = fopen("/sys/class/gpio/export", "ab")) == NULL){
      printf("Unable to export GPIO pin\n");
      return 1;
  }
  strcpy(setValue, GPIO50String);
  fwrite(&setValue, sizeof(char), 2, myOutputHandle);
  fclose(myOutputHandle);
  if ((myOutputHandle = fopen("/sys/class/gpio/export", "ab")) == NULL){
      printf("Unable to export GPIO pin\n");
      return 1;
  }
  strcpy(setValue, GPIO60String);
  fwrite(&setValue, sizeof(char), 2, myOutputHandle);
  fclose(myOutputHandle);
  // Set direction of the pins to output
  if ((myOutputHandle = fopen(GPIO50Direction, "rb+")) == NULL){
      printf("Unable to open direction 50 handle\n");
      return 1;
  }
  strcpy(setValue,"out");
  fwrite(&setValue, sizeof(char), 3, myOutputHandle);
  fclose(myOutputHandle);
  if ((myOutputHandle = fopen(GPIO60Direction, "rb+")) == NULL){
      printf("Unable to open direction 60 handle\n");
      return 1;
  }
  strcpy(setValue,"out");
  fwrite(&setValue, sizeof(char), 3, myOutputHandle);
  fclose(myOutputHandle);

  // initialize the camera
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

  Mat scene;
  while(1)
  {
    cap >> scene;
    if( scene.data )
    {
      // convert the scene to HSV
      cvtColor( scene, scene, CV_BGR2HSV );

      // filter the scene by hue
      Mat upr_scene, lwr_scene;
      inRange( scene, Scalar( UPPER_HUE, LOWER_SAT, LOWER_VAL ), Scalar( MAX_HUE, UPPER_SAT, UPPER_VAL ), upr_scene );
      inRange( scene, Scalar( MIN_HUE, LOWER_SAT, LOWER_VAL ), Scalar( LOWER_HUE, UPPER_SAT, UPPER_VAL ), lwr_scene );
      add( upr_scene, lwr_scene, scene );

      // erode/dilate to remove small noise clusters
      erode( scene, scene, getStructuringElement( MORPH_ELLIPSE, Size ( 3, 3 ) ) );
      dilate( scene, scene, getStructuringElement( MORPH_ELLIPSE, Size ( 5, 5 ) ) );

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

      // TODO: implement this
      // filter remaining hulls for proximity to at least three other hulls
      //const int PROXIMITY_LIMIT = 100;
      bool removed_something = true;
      while( removed_something )
      {
        removed_something = false;
        for( int i = hulls.size() - 1; i >= 0; i-- )
        {
          //int proximate = 0;
          // calculate distance to every other center and increment counter if less than limit
          // if counter less than three, remove hull from list and set removal flag to true
        }
      }

      // calculate vertex centroid of top four centers of mass
      Point centroid;
      if( hulls.size() >= 4 )
      {
        centroid = Point( (cms[0].x + cms[1].x + cms[2].x + cms[3].x) / 4,
                          (cms[0].y + cms[1].y + cms[2].y + cms[3].y) / 4 );
        int margin = 2; // allowance for number of pixels from nominal position

        // pan control
        if( centroid.x < ( scene.size().width / 2 ) - margin )
        {
          // enable pan advance pin, disable retreat pin
          // Set output to 50 high, 60 low
          if ((myOutputHandle = fopen(GPIO50Value, "rb+")) == NULL){
            printf("Unable to open value handle\n");
            return 1;
          }
          strcpy(setValue, "1"); // Set value high
          fwrite(&setValue, sizeof(char), 1, myOutputHandle);
          fclose(myOutputHandle);
          if ((myOutputHandle = fopen(GPIO60Value, "rb+")) == NULL){
            printf("Unable to open value handle\n");
            return 1;
          }
          strcpy(setValue, "0"); // Set value low
          fwrite(&setValue, sizeof(char), 1, myOutputHandle);
          fclose(myOutputHandle);
        }
        else if( centroid.x > ( scene.size().width / 2 ) - margin )
        {
          // enable pan retreat pin, disable advance pin
          // Set output to 50 low, 60 high
          if ((myOutputHandle = fopen(GPIO50Value, "rb+")) == NULL){
            printf("Unable to open value handle\n");
            return 1;
          }
          strcpy(setValue, "0"); // Set value low
          fwrite(&setValue, sizeof(char), 1, myOutputHandle);
          fclose(myOutputHandle);
          if ((myOutputHandle = fopen(GPIO60Value, "rb+")) == NULL){
            printf("Unable to open value handle\n");
            return 1;
          }
          strcpy(setValue, "1"); // Set value high
          fwrite(&setValue, sizeof(char), 1, myOutputHandle);
          fclose(myOutputHandle);
        }
        else
        {
          // disable both pan pins
          // Set output to 50 low, 60 low
          if ((myOutputHandle = fopen(GPIO50Value, "rb+")) == NULL){
            printf("Unable to open value handle\n");
            return 1;
          }
          strcpy(setValue, "0"); // Set value low
          fwrite(&setValue, sizeof(char), 1, myOutputHandle);
          fclose(myOutputHandle);
          if ((myOutputHandle = fopen(GPIO60Value, "rb+")) == NULL){
            printf("Unable to open value handle\n");
            return 1;
          }
          strcpy(setValue, "0"); // Set value low
          fwrite(&setValue, sizeof(char), 1, myOutputHandle);
          fclose(myOutputHandle);
        }

        // tilt control
        if( centroid.y < ( scene.size().height / 2 ) - margin )
        {
          // enable tilt advance pin, disable retreat pin
        }
        else if( centroid.y > ( scene.size().height / 2 ) - margin )
        {
          // enable tilt retreat pin, disable advance pin
        }
        else
        {
          // disable both tilt pins
        }
      }
    }
  }

  // Unexport the pin
/*
  if ((myOutputHandle = fopen("/sys/class/gpio/unexport", "ab")) == NULL) {
      printf("Unable to unexport GPIO pin\n");
      return 1;
  }
  strcpy(setValue, GPIOString);
  fwrite(&setValue, sizeof(char), 2, myOutputHandle);
  fclose(myOutputHandle);
*/

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
