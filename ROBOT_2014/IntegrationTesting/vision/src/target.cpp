/**
 * @file target
 * @brief target recognition and location detection using OpenCV
 * @author D. Butenhoff
 */

#include <cstdio>
#include <cmath>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "target.h"
#include "config.h"

using namespace cv;

/*
 * returns true if a centroid was found, false otherwise
 */
bool locateTarget( cv::Mat *scene, cv::Point *centroid, int num )
{
  // convert the scene to HSV
  cvtColor( *scene, *scene, CV_BGR2HSV );

  // filter the scene by tuned HSV ranges
  Mat upr_scene, lwr_scene;
  inRange( *scene, Scalar( UPPER_HUE, LOWER_SAT, LOWER_VAL ), Scalar( MAX_HUE, UPPER_SAT, UPPER_VAL ), upr_scene );
  inRange( *scene, Scalar( MIN_HUE, LOWER_SAT, LOWER_VAL ), Scalar( LOWER_HUE, UPPER_SAT, UPPER_VAL ), lwr_scene );
  add( upr_scene, lwr_scene, *scene );
  #ifdef DEBUG
    char buffer[10];
    sprintf( buffer, "%d", num );
    string filename = "./snapshot" + string(buffer) + "_stage1.png";
    imwrite( filename, *scene );
  #endif

  // erode to remove small noise clusters
  erode( *scene, *scene, getStructuringElement( MORPH_ELLIPSE, Size ( ERODE_SIZE, ERODE_SIZE ) ) );
  #ifdef DEBUG
    filename = "./snapshot" + string(buffer) + "_stage2.png";
    imwrite( filename, *scene );
  #endif

  // dilate to restore original feature size
  dilate( *scene, *scene, getStructuringElement( MORPH_ELLIPSE, Size ( DILATE_SIZE, DILATE_SIZE ) ) );
  #ifdef DEBUG
    filename = "./snapshot" + string(buffer) + "_stage3.png";
    imwrite( filename, *scene );
  #endif

  // detect the edges of the remaining blobs using Canny
  Canny( *scene, *scene, CANNY_LOW_THRESHOLD, CANNY_HIGH_THRESHOLD, CANNY_KERNEL_SIZE );

  // get contours from edges
  vector< vector<Point> > contours;
  vector<Vec4i> hierarchy;
  findContours( *scene, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
  #ifdef DEBUG
    // draw the contours on the scene and write it to a file
    Mat scene_copy = (*scene).clone();
    for( unsigned int j = 0; j < contours.size(); j++ )
    {
      drawContours( scene_copy, contours, j, Scalar(((10*j) % 255), 255, 255), 1, 8, vector<Vec4i>(), 0, Point() );
    }
    filename = "./snapshot" + string(buffer) + "_stage4.png";
    imwrite( filename, scene_copy );
  #endif

  // normalize contours into convex hulls
  vector< vector<Point> > hulls( contours.size() );
  for( unsigned int i = 0; i < contours.size(); i++ )
  {
    convexHull( Mat(contours[i]), hulls[i] );
  }

  // calculate area for each hull
  vector<double> areas( hulls.size() );
  for( int i = hulls.size() - 1; i >= 0; i-- )
  {
    areas[i] = contourArea( hulls[i] );
  }

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

  // filter hulls more thoroughly for proximity
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
  #ifdef DEBUG
    // draw the remaining hulls on the scene and write it to a file
    scene_copy = (*scene).clone();
    for( unsigned int j = 0; j < hulls.size(); j++ )
    {
      drawContours( scene_copy, hulls, j, Scalar(((10*j) % 255), 255, 255), 1, 8, vector<Vec4i>(), 0, Point() );
    }
    filename = "./snapshot" + string(buffer) + "_stage5.png";
    imwrite( filename, scene_copy );
  #endif

  // calculate vertex centroid of top four centers of mass
  // REVISIT: how can we select these more intelligently?
  if( hulls.size() >= 4 )
  {
    *centroid = Point( (cms[0].x + cms[1].x + cms[2].x + cms[3].x) / 4,
                      (cms[0].y + cms[1].y + cms[2].y + cms[3].y) / 4 );
    #ifdef DEBUG
      // draw the detected centroid on the scene and write it to a file
      scene_copy = (*scene).clone();
      circle( scene_copy, centroid, 3, Scalar(255, 255, 255) );
      filename = "./snapshot" + string(buffer) + "_stage6.png";
      imwrite( filename, scene_copy );
      num++;
    #endif

    return true;
  }

  return false;
}

/*
 * comparison function object for sorting in decreasing order
 */
bool compareContourAreas( std::vector<cv::Point> contour1, std::vector<cv::Point> contour2 )
{
  double i = fabs( contourArea(cv::Mat(contour1)) );
  double j = fabs( contourArea(cv::Mat(contour2)) );
  return ( i > j );
}
