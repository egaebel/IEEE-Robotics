/**
 * @file contour_shapes
 * @brief Canny + findContours + approxPoly
 * @author D. Butenhoff
 */

#include <cstdio>
#include <cmath>
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
  { printf(" --(!) Error reading image\n"); return -1; }

  // high-level denoising
  Mat dns_scene;
  fastNlMeansDenoisingColored( bgr_scene, dns_scene );
  #ifdef DEBUG
    imwrite ( "./dns_scene.png", dns_scene );
  #endif

  // filter the scene by hue
  Mat hsv_scene, upr_scene, lwr_scene, sum_scene;
  cvtColor( dns_scene, hsv_scene, CV_BGR2HSV );
  inRange( hsv_scene, Scalar( 172, 120, 70 ), Scalar( 179, 150, 150 ), upr_scene );
  inRange( hsv_scene, Scalar( 0, 120, 70 ), Scalar( 5, 150, 150 ), lwr_scene );
  add( upr_scene, lwr_scene, sum_scene );
  #ifdef DEBUG
    imwrite( "./upr_scene.png", upr_scene );
    imwrite( "./lwr_scene.png", lwr_scene );
    imwrite( "./sum_scene.png", sum_scene );
  #endif

  // erode/dilate to remove any remaining small noise clusters
  Mat erd_scene, dil_scene;
  erode( sum_scene, erd_scene, getStructuringElement( MORPH_ELLIPSE, Size ( 3, 3 ) ) );
  dilate( erd_scene, dil_scene, getStructuringElement( MORPH_ELLIPSE, Size ( 5, 5 ) ) );
  #ifdef DEBUG
    imwrite( "./erd_scene.png", erd_scene );
    imwrite( "./dil_scene.png", dil_scene );
  #endif

  // detect the edges using Canny
  Mat cny_scene;
  Canny( dil_scene, cny_scene, 100, 200, 3 );
  #ifdef DEBUG
    imwrite( "./cny_scene.png", cny_scene );
  #endif

  // find contours
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;
  findContours( cny_scene, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
  #ifdef DEBUG
    Mat cnt_scene = Mat::zeros( cny_scene.size(), CV_8UC3 );
    for( unsigned int i = 0; i < contours.size(); i++ )
    {
      Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255) );
      drawContours( cnt_scene, contours, i, color, 1, 8, hierarchy, 0, Point() );
    }
    imwrite( "./cnt_scene.png", cnt_scene );
  #endif

  // match shapes
  Mat shp_scene = Mat::zeros( cny_scene.size(), CV_8UC3 );
  Mat hul_scene = Mat::zeros( cny_scene.size(), CV_8UC3 );
  vector<vector<Point> > shapes( contours.size() );
  vector<vector<Point> > hulls( contours.size() );
  for( unsigned int shape = 0; shape < contours.size(); shape++ )
  {
    // fit contours to 5% of arc length to approximate polygons
    approxPolyDP( contours[shape], shapes[shape], arcLength(Mat(contours[shape]), true)*0.05, true);

    // fit convex hulls around shapes
    convexHull( Mat(shapes[shape]), hulls[shape] );

    #ifdef DEBUG
      drawContours( shp_scene, shapes, shape, Scalar(255, 0, 0), 1, 8, vector<Vec4i>(), 0, Point() );
      drawContours( hul_scene, hulls, shape, Scalar(255, 0, 0), 1, 8, vector<Vec4i>(), 0, Point() );
    #endif
  }
  #ifdef DEBUG
    imwrite( "./shp_scene.png", shp_scene );
    imwrite( "./hul_scene.png", hul_scene );
  #endif

  // sort by largest contours
  std::sort(hulls.begin(), hulls.end(), compareContourAreas);
  std::sort(shapes.begin(), shapes.end(), compareContourAreas);

  // filter shapes if out of size range, overlapping, or far apart
  for( int i = hulls.size() - 1; i >= 0; i-- )
  {
    double area_i = contourArea(hulls[i]);
    if( area_i < 10 || area_i > 10000 )
    {
      #ifdef DEBUG
        printf("throwing out %f because of size\n", area_i);
      #endif
      hulls.erase(hulls.begin()+i);
    }
    else
    {
      Moments mu_i = moments( hulls[i] );
      Point cm_i = Point( mu_i.m10/mu_i.m00 , mu_i.m01/mu_i.m00 );
      printf("checking %d:%f\n", i, contourArea(hulls[i]));
      for( int j = hulls.size() - 1; j > i; j-- )
      {
        Moments mu_j = moments( hulls[j] );
        Point cm_j = Point( mu_j.m10/mu_j.m00 , mu_j.m01/mu_j.m00 );
        double cm_dist = sqrt( pow( (cm_i.x - cm_j.x), 2 ) + pow( (cm_i.y - cm_j.y), 2 ) );
        double root_area = sqrt( area_i );
        printf("%d: %f @ (%d, %d)\n", j, contourArea(hulls[j]), cm_j.x, cm_j.y);
        if( cm_dist < root_area || cm_dist > 4 * root_area )
        {
          #ifdef DEBUG
            printf("throwing out %f because %f < %f\n", contourArea(hulls[j]), cm_dist, root_area);
          #endif
          hulls.erase(hulls.begin()+j);
        }
        else
        {
          printf("%f at (%d, %d) is %f from (%d, %d)\n", area_i, cm_i.x, cm_i.y, cm_dist, cm_j.x, cm_j.y);
        }
      }
    }
  }

  // center of mass of top contours
  Mat cms_scene = bgr_scene.clone();
  vector<Point> cms(hulls.size());
  #ifdef DEBUG
    printf("%d corners found\n", hulls.size());
  #endif
  for( unsigned int i = 0; i < hulls.size(); i++ )
  {
    Moments mu = moments( hulls[i] );
    cms[i] = Point( mu.m10/mu.m00 , mu.m01/mu.m00 );

    #ifdef DEBUG
      // plot the center of mass
      circle( cms_scene, cms[i], 3, Scalar(255, 255, 255) );
      printf("%f at (%d, %d)\n", contourArea(hulls[i]), cms[i].x, cms[i].y);
    #endif
  }
  #ifdef DEBUG
    imwrite( "./cms_scene.png", cms_scene );
  #endif

  // vertex centroid of top four centers of mass
  if( hulls.size() >= 4 )
  {
    Point centroid = Point( (cms[0].x + cms[1].x + cms[2].x + cms[3].x) / 4,
                            (cms[0].y + cms[1].y + cms[2].y + cms[3].y) / 4 );
    Mat ctd_scene = bgr_scene.clone();
    circle( ctd_scene, centroid, 3, Scalar(255, 255, 255) );
    printf("%d, %d\n", centroid.x, centroid.y);
    imwrite( "./ctd_scene.png", ctd_scene );
  }

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
  printf(" Usage: ./contour_shapes <scene_image>\n");
}

/*
 * comparison function object for sorting in decreasing order
 */
bool compareContourAreas ( std::vector<cv::Point> contour1, std::vector<cv::Point> contour2 ) {
    double i = fabs( contourArea(cv::Mat(contour1)) );
    double j = fabs( contourArea(cv::Mat(contour2)) );
    return ( i > j );
}
