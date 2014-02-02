/**
 * @file canny
 * @brief testing different tuning parameters on individual image channels
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

  Mat scene_orig;
  for( int scene_num = 1; scene_num < argc; scene_num++ )
  {
    if( argv[scene_num] != NULL )
    {
      scene_orig = imread( argv[scene_num], CV_LOAD_IMAGE_COLOR );

      vector<Mat> channels, channels_hsv;
      split( scene_orig, channels );
      for( int i = 0; i < 3; i++ )
      {
        char chnl_buffer[10];
        sprintf( chnl_buffer, "%d", i );
        string filename = "./" + string(argv[scene_num]) + "_bgr" + string(chnl_buffer) + "_orig.png";
        imwrite( filename, channels[i] );
      }

      Mat scene_hsv;
      cvtColor( scene_orig, scene_hsv, CV_BGR2HSV );
      split( scene_hsv, channels_hsv );
      for( int i = 0; i < 3; i++ )
      {
        char chnl_buffer[10];
        sprintf( chnl_buffer, "%d", i );
        string filename = "./" + string(argv[scene_num]) + "_hsv" + string(chnl_buffer) + "_orig.png";
        imwrite( filename, channels_hsv[i] );
      }

      if( scene_orig.data )
      {
        for( int lower_threshold = 0; lower_threshold < 100; lower_threshold += 20 )
        {
        for( int upper_threshold = lower_threshold; upper_threshold < lower_threshold * 4; upper_threshold += lower_threshold )
        {
          char lt_buffer[10];
          sprintf( lt_buffer, "%d", lower_threshold );
          char ut_buffer[10];
          sprintf( ut_buffer, "%d", upper_threshold );

          vector<Mat> channels, channels_hsv;
          split( scene_orig, channels );

          for( int i = 0; i < 3; i++ )
          {
            Canny( channels[i], channels[i], (double)lower_threshold, (double)upper_threshold, 3, true );
            vector<vector<Point> > contours;
            vector<Vec4i> hierarchy;
            findContours( channels[i], contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
            Mat scene_imwrite = scene_orig.clone();
            for( unsigned int j = 0; j < contours.size(); j++ )
            {
              drawContours( scene_imwrite, contours, j, Scalar(((10*j) % 255), 255, 255), 1, 8, vector<Vec4i>(), 0, Point() );
            }
            char chnl_buffer[10];
            sprintf( chnl_buffer, "%d", i );
            string new_filename = "./" + string(argv[scene_num]) + "_" + string(lt_buffer) + "_" + string(ut_buffer)
                                       + "_bgr" + string(chnl_buffer) + ".png";
            imwrite( new_filename, scene_imwrite );
          }

          Mat scene_hsv;
          cvtColor( scene_orig, scene_hsv, CV_BGR2HSV );
          split( scene_hsv, channels_hsv );

          for( int i = 0; i < 3; i++ )
          {
            Canny( channels_hsv[i], channels_hsv[i], (double)lower_threshold, (double)upper_threshold, 3, true );
            vector<vector<Point> > contours;
            vector<Vec4i> hierarchy;
            findContours( channels_hsv[i], contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
            Mat scene_imwrite = scene_orig.clone();
            for( unsigned int j = 0; j < contours.size(); j++ )
            {
              drawContours( scene_imwrite, contours, j, Scalar(((10*j) % 255), 255, 255), 1, 8, vector<Vec4i>(), 0, Point() );
            }
            char chnl_buffer[10];
            sprintf( chnl_buffer, "%d", i );
            string new_filename = "./" + string(argv[scene_num]) + "_" + string(lt_buffer) + "_" + string(ut_buffer)
                                       + "_hsv" + string(chnl_buffer) + ".png";
            imwrite( new_filename, scene_imwrite );
          }
        }
        }
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
