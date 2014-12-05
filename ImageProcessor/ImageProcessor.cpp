#include <stdio.h>
#include <iostream>
#include <stdlib.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "ImageProcessor.h"

using namespace cv;
using namespace std;

#define MIN_AREA      25
#define MAX_AREA      ((this->width-4)*(this->height-4))
#define BACK_THRESH   3

RNG rng(12345);

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
  threshold(working, working, BACK_THRESH, 256, CV_THRESH_BINARY_INV);

  // Initialize lists for contours
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;

  // Find outlines of boxes
  findContours( working, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0,0) );

  // Approximate contours to polygons and get bounding rects
  vector<vector<Point> > contours_poly( contours.size() );
  vector<Rect> boundRect( contours.size() );

  
  for(int i=contours.size()-1; i >=0; i--)
  {
    approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
    boundRect[i] = boundingRect( Mat(contours_poly[i]) );
    int area = boundRect[i].area();
    if((area < MIN_AREA) | (area > MAX_AREA))
    {
       contours.erase(contours.begin()+i);
       contours_poly.erase(contours_poly.begin()+i);
       boundRect.erase(boundRect.begin()+i);
       continue;
    }
    cout << i << ": Area (" << area << ") ";
    cout << "Top (" << boundRect[i].tl().y << ") Left (" << boundRect[i].tl().x << ")";
    cout << " Bottom (" << boundRect[i].br().y << ") Right (" << boundRect[i].br().x << ")" << endl;
  }

  cout << contours.size() << " total obstacles detected in frame." << endl;

  Mat drawing;
  image.convertTo(drawing, CV_8U);
  cvtColor(drawing,drawing, CV_GRAY2RGB);
  for(unsigned int i=0; i< contours.size(); i++)
  {
    Scalar color = Scalar( rng.uniform(0,255), rng.uniform(0,255), rng.uniform(0,255) );
    drawContours( drawing, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
    rectangle( drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );
  }

  namedWindow("Contours", CV_WINDOW_AUTOSIZE );
  imshow("Contours", drawing);


  waitKey();

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
