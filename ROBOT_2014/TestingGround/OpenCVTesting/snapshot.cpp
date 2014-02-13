/**
 * @file snapshot
 * @brief simple snapshot taker for test dataset generation
 * @author D. Butenhoff
 */

#include <cstdio>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

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

  printf("%4.1f\n", cap.get(CAP_PROP_FRAME_WIDTH));
  printf("%4.1f\n", cap.get(CAP_PROP_FRAME_HEIGHT));

  // try to set the camera properties
  cap.set(CAP_PROP_FRAME_WIDTH, 640);
  cap.set(CAP_PROP_FRAME_HEIGHT, 480);

  printf("%4.1f\n", cap.get(CAP_PROP_FRAME_WIDTH));
  printf("%4.1f\n", cap.get(CAP_PROP_FRAME_HEIGHT));

  // initialize the camera
  cap.grab();

  printf( "grabbed\n" );

  // read (grab and retrieve) a frame from the camera
  Mat scene;
  cap >> scene;

  printf( "retrieved\n" );

  if( scene.data )
  {
    imwrite( "./snapshot.png", scene );
    printf( "written\n" );
  }
  else
  {
    printf( "failed to take snapshot\n" );
    return -1;
  }

  return 0;
}
