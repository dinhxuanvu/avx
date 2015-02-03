#include <stdio.h>
#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "ImageProcessor.h"
#include "Hazard.h"
#include "../macros.h"

using namespace cv;
using namespace std;

void printHazards(HazardList* h_p)
{
  LOG_ERROR("Current hazards: (%lu):\n", h_p->size());
  for(HazardList::iterator it= h_p->begin(); it != h_p->end(); ++it)
  {
    LOG_MESSAGE("Haz %d = center (%f,%f), depth: %dmm width: %f, height: %f\n", \
              it->id, it->theta, it->phi, it->depth, it->width, it->height);
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

  LOG_WARNING("Size: %lu\n",hazards_p->size());
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

