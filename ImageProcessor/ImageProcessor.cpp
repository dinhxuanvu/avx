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
  threshold(working, working, 3, 0, CV_THRESH_TOZERO);

  // Reset 0 values to max (far away)
  floodFill(working, Point(2,2), 256);

  // Draw a test rectangle
  //rectangle(working, Point(40,40), Point(200,200), 128);

  imshow("thresh",working);

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
