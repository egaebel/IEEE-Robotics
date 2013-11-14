/**
 * @file SURF_Homography
 * @brief SURF detector + descriptor + FLANN Matcher + FindHomography
 * @author A. Huaman
 */

#include <stdio.h>
#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/photo/photo.hpp"

using namespace std;
using namespace cv;

void readme();

/**
 * @function main
 * @brief Main function
 */
int main( int argc, char** argv )
{
  if( argc != 3 )
  { readme(); return -1; }

  Mat img_object = imread( argv[1], CV_LOAD_IMAGE_GRAYSCALE );
  // Old object filtering code
/*
  Mat bgr_object = imread( argv[1], CV_LOAD_IMAGE_COLOR );

  // construct an object mask for our hue range
  Mat hsv_object, upr_object, lwr_object, sum_object, obj_erd, obj_mask;
  cvtColor( bgr_object, hsv_object, CV_BGR2HSV );
  inRange( hsv_object, Scalar( 170, 20, 70 ), Scalar( 179, 255, 255 ), upr_object );
  inRange( hsv_object, Scalar( 0, 20, 70 ), Scalar( 9, 255, 255 ), lwr_object );
  add( upr_object, lwr_object, sum_object );
   imwrite( "./sum_object.png", sum_object );
//  blur( sum_object, obj_mask, Size( 5, 5 ) );
  erode( sum_object, obj_erd, getStructuringElement( MORPH_ELLIPSE, Size ( 3, 3 ) ) );
  dilate( obj_erd, obj_mask, getStructuringElement( MORPH_ELLIPSE, Size ( 3, 3 ) ) );
    imwrite( "./obj_mask.png", obj_mask );

  // apply the hue mask and convert to grayscale for feature detection
  Mat gry_object, eq_object, img_object;
  cvtColor( bgr_object, gry_object, CV_BGR2GRAY );
  equalizeHist( gry_object, eq_object );
    imwrite( "./eq_object.png", eq_object );
//  img_object = eq_object & obj_mask;
  img_object = obj_mask;
    imwrite( "./img_object.png", img_object );
*/

  Mat bgr_scene = imread( argv[2], CV_LOAD_IMAGE_COLOR );

  Mat dns_scene;
  fastNlMeansDenoisingColored( bgr_scene, dns_scene );
   imwrite ( "./dns_scene.png", dns_scene );

  // construct a scene mask for our hue range
  Mat hsv_scene, upr_scene, lwr_scene, sum_scene, scn_erd, scn_mask;
  cvtColor( dns_scene, hsv_scene, CV_BGR2HSV );
  inRange( hsv_scene, Scalar( 172, 120, 70 ), Scalar( 179, 150, 150 ), upr_scene );
  inRange( hsv_scene, Scalar( 0, 120, 70 ), Scalar( 5, 150, 150 ), lwr_scene );
  add( upr_scene, lwr_scene, sum_scene );
   imwrite( "./sum_scene.png", sum_scene );
//  blur( sum_scene, scn_mask, Size ( 5, 5 ) );
  erode( sum_scene, scn_erd, getStructuringElement( MORPH_ELLIPSE, Size ( 3, 3 ) ) );
  dilate( scn_erd, scn_mask, getStructuringElement( MORPH_ELLIPSE, Size ( 5, 5 ) ) );
   imwrite( "./scn_mask.png", scn_mask );

  // apply the hue mask and convert to grayscale for feature detection
  Mat gry_scene, eq_scene, img_scene;
  cvtColor( bgr_scene, gry_scene, CV_BGR2GRAY );
  equalizeHist( gry_scene, eq_scene );
    imwrite( "./eq_scene.png", eq_scene );
//  img_scene = eq_scene & scn_mask;
  img_scene = scn_mask;
    imwrite( "./img_scene.png", img_scene );

  if( !img_object.data || !img_scene.data )
  { std::cout<< " --(!) Error reading images " << std::endl; return -1; }

  //-- Step 1: Detect the keypoints using SURF Detector
  int minHessian = 600;
  SurfFeatureDetector detector( minHessian );
  std::vector<KeyPoint> keypoints_object, keypoints_scene;

  detector.detect( img_object, keypoints_object );
  Mat kpt_object;
  drawKeypoints( img_object, keypoints_object, kpt_object );
    imwrite( "./kpt_object.png", kpt_object );

  detector.detect( img_scene, keypoints_scene );
  Mat kpt_scene;
  drawKeypoints( img_scene, keypoints_scene, kpt_scene );
    imwrite( "./kpt_scene.png", kpt_scene );

  //-- Step 2: Calculate descriptors (feature vectors)
  SurfDescriptorExtractor extractor;

  Mat descriptors_object, descriptors_scene;

  extractor.compute( img_object, keypoints_object, descriptors_object );
  extractor.compute( img_scene, keypoints_scene, descriptors_scene );

  //-- Step 3: Matching descriptor vectors using FLANN matcher
  FlannBasedMatcher matcher;
  std::vector< DMatch > matches;
  matcher.match( descriptors_object, descriptors_scene, matches );

  double max_dist = 0; double min_dist = 100;

  //-- Quick calculation of max and min distances between keypoints
  for( int i = 0; i < descriptors_object.rows; i++ )
  { double dist = matches[i].distance;
    if( dist < min_dist ) min_dist = dist;
    if( dist > max_dist ) max_dist = dist;
  }

  printf("-- Max dist : %f \n", max_dist );
  printf("-- Min dist : %f \n", min_dist );

  //-- Draw only "good" matches (i.e. whose distance is less than 3*min_dist )
  std::vector< DMatch > good_matches;

  for( int i = 0; i < descriptors_object.rows; i++ )
  { if( matches[i].distance < 30*min_dist )
    { good_matches.push_back( matches[i]); }
  }

  Mat img_matches;
  drawMatches( img_object, keypoints_object, img_scene, keypoints_scene,
               good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
               vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );


  //-- Localize the object from img_1 in img_2
  std::vector<Point2f> obj;
  std::vector<Point2f> scene;

  for( size_t i = 0; i < good_matches.size(); i++ )
  {
    //-- Get the keypoints from the good matches
    obj.push_back( keypoints_object[ good_matches[i].queryIdx ].pt );
    scene.push_back( keypoints_scene[ good_matches[i].trainIdx ].pt );
  }

  Mat H = findHomography( obj, scene, RANSAC );

  //-- Get the corners from the image_1 ( the object to be "detected" )
  std::vector<Point2f> obj_corners(4);
  obj_corners[0] = Point(0,0); obj_corners[1] = Point( img_object.cols, 0 );
  obj_corners[2] = Point( img_object.cols, img_object.rows ); obj_corners[3] = Point( 0, img_object.rows );
  std::vector<Point2f> scene_corners(4);

  perspectiveTransform( obj_corners, scene_corners, H);


  //-- Draw lines between the corners (the mapped object in the scene - image_2 )
  Point2f offset( (float)img_object.cols, 0);
  line( img_matches, scene_corners[0] + offset, scene_corners[1] + offset, Scalar(0, 255, 0), 4 );
  line( img_matches, scene_corners[1] + offset, scene_corners[2] + offset, Scalar( 0, 255, 0), 4 );
  line( img_matches, scene_corners[2] + offset, scene_corners[3] + offset, Scalar( 0, 255, 0), 4 );
  line( img_matches, scene_corners[3] + offset, scene_corners[0] + offset, Scalar( 0, 255, 0), 4 );

  //-- Show detected matches
  imwrite( "./result.png", img_matches );

  waitKey(0);

  return 0;
}

/**
 * @function readme
 */
void readme()
{ std::cout << " Usage: ./SURF_Homography <img1> <img2>" << std::endl; }
