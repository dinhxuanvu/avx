#include <stdio.h>
#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "ImageProcessor.h"

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{

  // Open image files for frame and background
  Mat back = imread("media/background.png", 0);
  Mat frame = imread("media/3frame.png", 0);

  int width = back.cols;
  int height = back.rows;

  // Display the frame to process
  imshow("frame",frame);

  // Instantiate an iamge processor
  ImageProcessor proc(width, height);

  // Convert to format to match Xtion feed
  back.convertTo(back, CV_16U);
  uint16_t* backgroundData = (uint16_t*)(back.data);
  frame.convertTo(frame, CV_16U);  
  uint16_t* frameData = (uint16_t*)(frame.data);

  // Pass sample background into calibration
  proc.calibrate( backgroundData );

  // Pass frame into be processed for obstacles
  proc.nextFrame( frameData );

  // Keep windows open
  waitKey();

  return 0;
}
