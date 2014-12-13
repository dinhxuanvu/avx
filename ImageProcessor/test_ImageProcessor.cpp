#include <stdio.h>
#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "ImageProcessor.h"
#include "Hazard.h"

using namespace cv;
using namespace std;

void printHazards(HazardList* h_p)
{
  cout << "Current hazards (" << h_p->size() << "):" << endl;
  for(HazardList::iterator it= h_p->begin(); it != h_p->end(); ++it)
  {
    cout << "Haz " << it->id << "= center (" << it->theta << "," << it->phi << ")" << " depth " << it->depth << "mm";
    cout << " width: " << it->width << " height: " << it->height << endl;
  }
}

int main(int argc, char** argv)
{

  // Open image files for frame and background
  Mat back = imread("media/background.png", 0);
  Mat frame = imread("media/2frame.png", 0);
  Mat frame3 = imread("media/overlap_frame.png",0);

  int width = back.cols;
  int height = back.rows;

  HazardList* hazards_p = new HazardList;

  cout << "Size: " << hazards_p->size() << endl;
  // Instantiate an iamge processor
  ImageProcessor proc(width, height, hazards_p);

  // Convert to format to match Xtion feed
  back.convertTo(back, CV_16U);
  uint16_t* backgroundData = (uint16_t*)(back.data);
  frame.convertTo(frame, CV_16U);  
  uint16_t* frameData = (uint16_t*)(frame.data);
  
  frame3.convertTo(frame3, CV_16U);  
  uint16_t* frame3Data = (uint16_t*)(frame3.data);

  // Pass sample background into calibration
  proc.calibrate( backgroundData );

  // Pass frame into be processed for obstacles
  proc.nextFrame( frameData );
  printHazards(hazards_p);
  waitKey();

  proc.nextFrame( frame3Data );
  printHazards(hazards_p);
  waitKey();

  return 0;
}

