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

  Mat back = imread("media/background.png", 0);
  Mat frame = imread("media/frame.png", 0);

  int width = back.cols;
  int height = back.rows;

  cout << width << endl << height << endl;

  imshow("back",back);
  imshow("frame",frame);

  ImageProcessor proc(width, height);

  back.convertTo(back, CV_16U);
  frame.convertTo(frame, CV_16U);
  
  uint16_t* backgroundData = (uint16_t*)(back.data);

  proc.calibrate( backgroundData );

  uint16_t* frameData = (uint16_t*)(frame.data);
  proc.nextFrame( frameData );
  cout << "test" << endl;

  waitKey();

  return 0;
}
