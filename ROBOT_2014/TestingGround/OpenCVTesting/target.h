#ifndef TARGET_H
#define TARGET_H

#include "opencv2/core/core.hpp"

bool locateTarget( cv::Mat scene, cv::Point &centroid );
bool compareContourAreas( std::vector<cv::Point> contour1, std::vector<cv::Point> contour2 );

#endif

