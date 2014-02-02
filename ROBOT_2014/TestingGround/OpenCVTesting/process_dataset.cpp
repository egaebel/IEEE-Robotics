/**
 * @file aiming_demo
 * @brief Canny + findContours + convexHull
 * @author D. Butenhoff
 */

#include <cstdio>
#include <cmath>
#include <string>
#include <iostream>
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

// hull size filter range
const int MIN_SIZE = 100; // one corner of the target at furthest distance is about 180 square pixels
const int MAX_SIZE = 1000; // one corner of the target at closest distance is about 650 square pixels

// hull proximity distance filter limit
const int PROXIMITY_LIMIT = 70; // diagonal corners of the target at closest distance is about 60 pixels

bool compareContourAreas ( std::vector<cv::Point> contour1, std::vector<cv::Point> contour2 );

/**
 * @function main
 * @brief Main function
 */
int main( int argc, char** argv )
{
  if( argc < 2 )
  { cout << "no dataset was specified" << endl; return -1; }

  Mat scene;
  for( int scene_num = 1; scene_num < argc; scene_num++ )
  {
    if( argv[scene_num] != NULL )
    {
    scene = imread( argv[scene_num], CV_LOAD_IMAGE_COLOR );
    if( scene.data )
    {
      Mat scene_original = scene.clone();

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

      bool removed_something = true;
      while( removed_something )
      {
        removed_something = false;
        for( int i = hulls.size() - 1; i >= 0; i-- )
        {
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
            cout << "removing hull at (" << cms[i].x << ", " << cms[i].y << ")" << endl;
            hulls.erase( hulls.begin() + i );
            cms.erase( cms.begin() + i );
            areas.erase( areas.begin() + i );
            removed_something = true;
          }
        }
      }

      // calculate vertex centroid of top four centers of mass
      Point centroid;
      for( unsigned int i = 0; i < hulls.size(); i++ )
      {
        drawContours( scene_original, hulls, i, Scalar(((10*i) % 255), 255, 255), 1, 8, vector<Vec4i>(), 0, Point() );
      }

      string new_filename = "./" + string(argv[scene_num]) + "_processed.png";
      if( hulls.size() >= 4 )
      {
        centroid = Point( (cms[0].x + cms[1].x + cms[2].x + cms[3].x) / 4,
                          (cms[0].y + cms[1].y + cms[2].y + cms[3].y) / 4 );
        circle( scene_original, centroid, 3, Scalar(0, 0, 255) );
        cout << "centroid found at (" << centroid.x << ", " << centroid.y << ") based on hulls: ";
        cout << areas[0] << ", " << areas[1] << ", " << areas[2] << ", " << areas[3] << endl;
      }
      else
      {
        cout << "centroid not found in " << new_filename << endl;
      }
      imwrite( new_filename, scene_original );
    }
    else
      cout << "could not open file" << endl;
    }
    else
      cout << "invalid parameter" << endl;
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
