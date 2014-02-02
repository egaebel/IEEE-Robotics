/**
 * @file snapshot
 * @brief simple snapshot taker for test dataset generation
 * @author D. Butenhoff
 */

#include <cstdio>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

/**
 * @function main
 * @brief Main function
 */
int main()
{
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

  // retrieve a frame from the camera
  Mat scene;
  cap >> scene;

  if( scene.data )
    imwrite( "./snapshot.png", scene );
  else
  {
    printf( "failed to take snapshot\n" );
    return -1;
  }

  return 0;
}
