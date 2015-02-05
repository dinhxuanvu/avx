#include <stdio.h>
#include <iostream>
#include <stdlib.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "ImageProcessor.h"
#include "Hazard.h"
#include "../macros.h"

using namespace cv;
using namespace std;

#define MIN_DEPTH     400
#define MIN_AREA      200
#define MAX_AREA      ((this->width-4)*(this->height-4))
#define BACK_THRESH   10
#define CANNY_PARAM   35
#define EDGE_ERODE    6
#define CAM_VIEW_W    58.0f
#define CAM_VIEW_H    45.0f
#define CONVERT_CONST 0.064f

RNG rng(12345);

/*
 * Public constructor for image processor
 */
ImageProcessor::ImageProcessor(int width, int height, HazardList* haz_p)
{
  // Store local width and height
  this->width = width;
  this->height = height;
  this->calCount = 0;
  // Hold pointer to hazards list
  this->hazards = haz_p;

  // Instantiate empty OpenCV Mat
  this->calibrationImage = Mat(this->height,this->width,CV_8U);
}

bool compareByLength(const Hazard &a, const Hazard &b)
{
    return a.depth < b.depth;
}

/*
 * Give the processor the next frame and find the hazards and obstacles
 */
void ImageProcessor::nextFrame(uint16_t* dataBuffer)
{
  // Clear hazards list for new hazards
  this->hazards->clear();

  // Instantiate OpenCV Mat wrapper around data buffer
  Mat image(this->height, this->width, CV_16U, dataBuffer);
  Mat working;
  
  // Convert to 8 bits on working copy

  image *= 0.064;
  image.convertTo(working, CV_8U);

  // Background subtraction
  // diff = |Image - background|
  absdiff(this->calibrationImage,working,working);
  
  // Find edges where objects overlap
  Mat edges;
  // Canny detector
  int ratio = 3;
  int lowThreshold = CANNY_PARAM;
  Canny( working, edges, lowThreshold, lowThreshold*ratio, 3 );
  // Invert edge image for mask operation
  bitwise_not(edges,edges);
  // Dilation of edge image
  Mat element = getStructuringElement( MORPH_RECT, Size(EDGE_ERODE,EDGE_ERODE) );
  erode(edges,edges,element);

  // Threshold background subtraction
  threshold(working, working, BACK_THRESH, 256, CV_THRESH_BINARY);

  // Split threshold image at edges for overlapping images
  working = working.mul(edges);

  // Initialize lists for contours
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;

  // Find outlines of boxes
  findContours( working, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0,0) );

  // Approximate contours to polygons and get bounding rects
  vector<vector<Point> > contours_poly( contours.size() );
  vector<Rect> boundRect( contours.size() );

  // Loop through contours that are found
  // Remove too small or too large
  for(int i=contours.size()-1; i >=0; i--)
  {
    // Make polygonal approximation of shape detected (can be skipped)
    approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
    // Bound shape in a rectangle
    boundRect[i] = boundingRect( Mat(contours_poly[i]) );
    // Calculate area of contour
    double area = contourArea(contours_poly[i]);

    // Depth calculator for box
    Scalar ddepth = mean(image(boundRect[i]));
    int depth = (int)ddepth[0]/CONVERT_CONST;

    if((area < MIN_AREA) | (area > MAX_AREA) | (depth < MIN_DEPTH))
    {
      // Remove from countors of too small or large
      contours.erase(contours.begin()+i);
      contours_poly.erase(contours_poly.begin()+i);
      boundRect.erase(boundRect.begin()+i);
      continue;
    }
    // Print out area and stats
    //cout << i << ": Area (" << area << ") ";
    //cout << "Top (" << boundRect[i].tl().y << ") Left (" << boundRect[i].tl().x << ")";
    //cout << " Bottom (" << boundRect[i].br().y << ") Right (" << boundRect[i].br().x << ")" << endl;

    // Set up parameters for hazard
    double w     = (boundRect[i].width * CAM_VIEW_W / this->width);
    double h     = (boundRect[i].height * CAM_VIEW_H / this->height);
    double theta = ((boundRect[i].tl().x + (boundRect[i].width/2.0f)) * CAM_VIEW_W) / this->width - 0.5 * CAM_VIEW_W;
    double phi   = -((boundRect[i].tl().y + (boundRect[i].height/2.0f)) * CAM_VIEW_H) / this->height + 0.5 * CAM_VIEW_H;

    // Create and add into hazards list
    Hazard thisHaz = {
      i,          // id
      theta, phi, // center location
      w, h,       // width and height
      depth       // depth
    };
    this->hazards->push_back( thisHaz );
  }
  //cout << contours.size() << " total obstacles detected in frame." << endl;

  // Create a new drawing with the original image and the countors in color
  Mat drawing;
  image.convertTo(drawing, CV_8U);
  cvtColor(drawing,drawing, CV_GRAY2RGB);
  for(unsigned int i=0; i< contours.size(); i++)
  {
    Scalar color = Scalar( rng.uniform(0,255), rng.uniform(0,255), rng.uniform(0,255) );
    drawContours( drawing, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
    rectangle( drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );
  }

  namedWindow("Con",0);
  imshow("Con", drawing);

  waitKey(20);

  CLEAR_SCREEN;
  LOG_MESSAGE("Hazards: %lu\n",this->hazards->size());
  sort(this->hazards->begin(), this->hazards->end(), compareByLength);
  ImageProcessor::printHazards(this->hazards);

  return;
}

/*
 * Calibrate the image processor with a background image
 * Stored for all future reads and used as background.
 */
void ImageProcessor::calibrate(uint16_t* dataBuffer)
{
  // Create Mat wrapper
  Mat nextCalibration(this->height,this->width, CV_16U, dataBuffer);

  // Convert bit depth to 0 to 256
  nextCalibration *= CONVERT_CONST;
  nextCalibration.convertTo(nextCalibration, CV_8U);

  // Average calibration image into existing calibration
  this->calCount++;
  this->calibrationImage = this->calibrationImage*(1.0f-1.0f/this->calCount) + nextCalibration*(1.0f/this->calCount);
}