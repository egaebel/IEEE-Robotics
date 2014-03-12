/**
 * @file snapshot
 * @brief simple snapshot taker for test dataset generation
 * @author D. Butenhoff
 */
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

/**
 * @function main
 * @brief Main function
 */
int main(int argc, char * argv[])
{
    VideoCapture cap(0);
    if( !cap.isOpened() )
    {
        printf( "failed to open camera\n" );
        return -1;
    }

    printf("%4.1f\n", cap.get(CV_CAP_PROP_FRAME_WIDTH));
    printf("%4.1f\n", cap.get(CV_CAP_PROP_FRAME_HEIGHT));

    // try to set the camera properties
    cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

    printf("%4.1f\n", cap.get(CV_CAP_PROP_FRAME_WIDTH));
    printf("%4.1f\n", cap.get(CV_CAP_PROP_FRAME_HEIGHT));

    int snapshotCount = 1;
    if (argc > 1) {
        snapshotCount = atoi(argv[1]);
    }
    printf( "Taking %d snapshots\n", snapshotCount );

    //Take multiple snapshots
    for (int i = 0; i < snapshotCount; i++) {

        printf("\n\nPreparing to take picture...you have 4 seconds\n");
        sleep(4);

        // initialize the camera
        cap.grab();

        printf( "grabbed\n" );

        // read (grab and retrieve) a frame from the camera
        Mat scene;

        cap >> scene;

        printf( "retrieved\n" );

        if( scene.data )
        {
            char filename[20];
            snprintf( filename, 20, "./snapshot_%d.png", i+1); 
	        printf("writing %s\n", filename);
            imwrite( filename, scene );
            printf( "written\n" );
        }
        else
        {
            printf( "failed to take snapshot\n" );
            return -1;
        }
    }

    return 0;
}
