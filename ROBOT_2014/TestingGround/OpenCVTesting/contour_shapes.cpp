/**
 * @file contour_shapes
 * @brief Canny + findContours + approxPoly
 * @author D. Butenhoff
 */

#include <stdio.h>
#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/photo/photo.hpp"

using namespace std;
using namespace cv;

RNG rng(12345);

void readme();
bool compareContourAreas ( std::vector<cv::Point> contour1, std::vector<cv::Point> contour2 );

/**
 * @function main
 * @brief Main function
 */
int main( int argc, char** argv )
{
  if( argc != 2 )
  { readme(); return -1; }

  Mat bgr_scene = imread( argv[1], CV_LOAD_IMAGE_COLOR );

  if( !bgr_scene.data )
  { std::cout<< " --(!) Error reading image " << std::endl; return -1; }

  // high-level denoising
  Mat dns_scene;
  fastNlMeansDenoisingColored( bgr_scene, dns_scene );
   imwrite ( "./dns_scene.png", dns_scene );

  // filter the scene by hue
  Mat hsv_scene, upr_scene, lwr_scene, sum_scene;
  cvtColor( dns_scene, hsv_scene, CV_BGR2HSV );
  inRange( hsv_scene, Scalar( 172, 120, 70 ), Scalar( 179, 150, 150 ), upr_scene );
   imwrite( "./upr_scene.png", upr_scene );
  inRange( hsv_scene, Scalar( 0, 120, 70 ), Scalar( 5, 150, 150 ), lwr_scene );
   imwrite( "./lwr_scene.png", lwr_scene );
  add( upr_scene, lwr_scene, sum_scene );
   imwrite( "./sum_scene.png", sum_scene );

  // erode/dilate to remove any remaining small noise clusters
  Mat erd_scene, dil_scene;
  erode( sum_scene, erd_scene, getStructuringElement( MORPH_ELLIPSE, Size ( 3, 3 ) ) );
   imwrite( "./erd_scene.png", erd_scene );
  dilate( erd_scene, dil_scene, getStructuringElement( MORPH_ELLIPSE, Size ( 5, 5 ) ) );
   imwrite( "./dil_scene.png", dil_scene );

  // detect the edges using Canny
  Mat cny_scene;
  Canny( dil_scene, cny_scene, 100, 200, 3 );
   imwrite( "./cny_scene.png", cny_scene );

  // find contours
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;
  findContours( cny_scene, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
  Mat cnt_scene = Mat::zeros( cny_scene.size(), CV_8UC3 );
  for( unsigned int i = 0; i < contours.size(); i++ )
  {
    Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255) );
    drawContours( cnt_scene, contours, i, color, 2, 8, hierarchy, 0, Point() );
  }
   imwrite( "./cnt_scene.png", cnt_scene );

  // match shapes
  Mat shp_scene = Mat::zeros( cny_scene.size(), CV_8UC3 );
  Mat hul_scene = Mat::zeros( cny_scene.size(), CV_8UC3 );
  vector<vector<Point> > shapes( contours.size() );
  vector<vector<Point> > hulls( contours.size() );
  for( unsigned int shape = 0; shape < contours.size(); shape++ )
  {
    // filter contours to 10% of arc length to approximate polygons
    approxPolyDP( contours[shape], shapes[shape], arcLength(Mat(contours[shape]), true)*0.1, true);

    // filter shapes to convex hulls
    convexHull( Mat(shapes[shape]), hulls[shape] );

    drawContours( shp_scene, shapes, shape, Scalar(255, 0, 0), 1, 8, vector<Vec4i>(), 0, Point() );

    drawContours( hul_scene, hulls, shape, Scalar(255, 0, 0), 1, 8, vector<Vec4i>(), 0, Point() );
  }
   imwrite( "./shp_scene.png", shp_scene );
   imwrite( "./hul_scene.png", hul_scene );

  // sort by largest contours, top four of which should be our target corners
  std::sort(hulls.begin(), hulls.end(), compareContourAreas);

  // center of mass of convex hulls
  Mat cms_scene = Mat::zeros( cny_scene.size(), CV_8UC3 );
  vector<Point> cms(4);
  cout << "corners found: " << endl;
  for( unsigned int i = 0; i < 4; i++ )
  {
    Moments mu = moments( hulls[i] );
    cms[i] = Point( mu.m10/mu.m00 , mu.m01/mu.m00 );

    // filter out duplicate contours
    unsigned int k = i;
    for( unsigned int j = 0; j < i; j++ )
    {
      while( (cms[j] == cms[i] || contourArea(hulls[j]) == contourArea(hulls[k])) && k < (hulls.size() - 1) )
      {
        k++;
        mu = moments( hulls[k] );
        cms[i] = Point( mu.m10/mu.m00 , mu.m01/mu.m00 );
      }
    }

    // plot the center of mass
    circle( cms_scene, cms[i], 3, Scalar(255, 255, 255) );
    cout << contourArea(hulls[k]) << " at (" << cms[i].x << ", " << cms[i].y << ")" << endl;
  }
   imwrite( "./cms_scene.png", cms_scene );

  // vertex centroid of centers of mass
  Point centroid = Point( (cms[0].x + cms[1].x + cms[2].x + cms[3].x) / 4,
                          (cms[0].y + cms[1].y + cms[2].y + cms[3].y) / 4 );

  Mat ctd_scene = bgr_scene.clone();
  circle( ctd_scene, centroid, 3, Scalar(255, 255, 255) );
  cout << centroid.x << ", " << centroid.y << endl;
   imwrite( "./ctd_scene.png", ctd_scene );

  // convert coordinates to angle

  // send angle to servos

  waitKey(0);

  return 0;
}

/**
 * @function readme
 */
void readme()
{
  std::cout << " Usage: ./contour_shapes <scene_image>" << std::endl;
}

/*
 * comparison function object for sorting in decreasing order
 */
bool compareContourAreas ( std::vector<cv::Point> contour1, std::vector<cv::Point> contour2 ) {
    double i = fabs( contourArea(cv::Mat(contour1)) );
    double j = fabs( contourArea(cv::Mat(contour2)) );
    return ( i > j );
}
