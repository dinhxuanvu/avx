#include <stdio.h>
#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "ImageProcessor.h"

using namespace cv;
using namespace std;

/*
 * Public constructor for image processor
 */
ImageProcessor::ImageProcessor(int width, int height)
{
  // Store local width and height
  this->width = width;
  this->height = height;

  // Instantiate empty OpenCV Mat
  this->calibrationImage = Mat(this->height,this->width,CV_8U);
}

/*
 * Give the processor the next frame and find the hazards and obstacles
 */
void ImageProcessor::nextFrame(uint16_t* dataBuffer)
{
  // Instantiate OpenCV Mat wrapper around data buffer
  Mat image(this->height, this->width, CV_16U, dataBuffer);
  Mat working;
  
  // Convert to 8 bits on working copy
  image.convertTo(working, CV_8U);
  
  // Background subtraction
  // diff = |Image - background|
  absdiff(this->calibrationImage,working,working);

  // Threshold background subtraction by 10
  threshold(working, working, 3, 256, CV_THRESH_TOZERO);

  // Reset 0 values to max (far away)
  floodFill(working, Point(2,2), 256);

  // Find discrete object
  double minVal;
  Point min_loc;
  Point add(10,10);

  // Color all distinct obstacles in image
  int i;
  for(i=0; i<10; i++)
  {
    // Get next min value
    minMaxLoc(working, &minVal, NULL, &min_loc);

    if((minVal > 200))
      break;

    // Fill it with a solid value
    floodFill(working, min_loc, 250-i, 0, 50, 50);

    rectangle(working, min_loc, min_loc+add, 201);

    imshow("thresh",working);
  }
  waitKey();
  cout << i << " total obstacles detected in frame." << endl;


  return;
}

/*
 * Calibrate the image processor with a background image
 * Stored for all future reads and used as background.
 */
void ImageProcessor::calibrate(uint16_t* dataBuffer)
{
  // Create Mat wrapper
  Mat calibrationImage(this->height,this->width, CV_16U, dataBuffer);

  // Convert bit depth to 0 to 256
  calibrationImage.convertTo(this->calibrationImage, CV_8U);

}
