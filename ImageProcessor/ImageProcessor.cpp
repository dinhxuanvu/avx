#include "ImageProcessor.h"

using namespace cv;
using namespace std;

#define BACK_THRESH         20
#define CANNY_PARAM         9
#define EDGE_ERODE          5
#define EDGE_BLUR           5
#define CAM_VIEW_W          58.0f
#define CAM_VIEW_H          45.0f
#define MIN_AREA            200
#define MAX_AREA            99999999
#define MIN_DEPTH           450
#define MAX_DEPTH           1400
#define CONVERT_CONST       0.064f
#define CALIBRATION_POINTS  25000
#define SHOW_WINDOWS        0
#define MIN_PHI             -20


RNG rng(1234);

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

/*
 * Give the processor the next frame and find the hazards and obstacles
 */
void ImageProcessor::nextFrame(uint16_t* dataBuffer)
{
  // Clear hazards list for new hazards
  this->hazards->clear();

  // Instantiate OpenCV Mat wrapper around data buffer
  Mat image(this->height, this->width, CV_16U, dataBuffer);
  Mat working, working2;
  
  // Convert to 8 bits on working copy

  image *= CONVERT_CONST;
  image.convertTo(working, CV_8U);

  // First check if majority of screen is 0
  int nonzero = countNonZero(working);
  if(nonzero < (0.5*this->height*this->width))
  {
    Hazard thisHaz = {
      0,       // id
      0, 0,    // center location
      0, 0,    // width and height
      0,       // depth
      BLOCK     // Hazard type
    };
    this->hazards->push_back( thisHaz );
    return;
  }

  // Don't count any 0 or 255 values
  threshold(working, working2, 128, 1, CV_THRESH_BINARY_INV);
  working = working.mul(working2);
  threshold(working, working2, 1, 1, CV_THRESH_BINARY_INV);
  multiply(this->calibrationImage,working2,working2);
  

  working += working2;

  #if DISPLAY_WINDOWS
  namedWindow("W",0);
  imshow("W", working);
  #endif


  // Background subtraction
  // diff = |Image - background|
  absdiff(this->calibrationImage,working,working);
  
  #if DISPLAY_WINDOWS
  namedWindow("ABS",0);
  imshow("ABS", working);
  #endif
  // Find edges where objects overlap
  Mat edges;
  // Canny detector
  int ratio = 3;
  int lowThreshold = CANNY_PARAM;
  GaussianBlur( working, edges, Size( EDGE_BLUR, EDGE_BLUR ), 0, 0 );
  Canny( edges, edges, lowThreshold, lowThreshold*ratio, 3 );
  // Invert edge image for mask operation
  bitwise_not(edges,edges);
  // Dilation of edge image
  Mat element = getStructuringElement( MORPH_RECT, Size(EDGE_ERODE,EDGE_ERODE) );
  erode(edges,edges,element);

#if DISPLAY_WINDOWS
  //namedWindow("W",0);
  //imshow("W", edges);
#endif
#if DISPLAY_WINDOWS
  namedWindow("CAL",0);
  imshow("CAL", this->calibrationImage);
#endif



  // Threshold background subtraction
  threshold(working, working, BACK_THRESH, 256, CV_THRESH_BINARY);
#if DISPLAY_WINDOWS
  namedWindow("T",0);
  imshow("T", working);
  #endif
  // Split threshold image at edges for overlapping images
  working = working.mul(edges);

  // Initialize lists for contours
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;

  // Find outlines of boxes
  findContours( working, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0,0) );


  for(unsigned int i=0; i < contours.size(); i++)
  {
    // Bound shape in a rectangle
    // Make polygonal approximation of shape detected (can be skipped)
    vector<Point> anothercontour;
    approxPolyDP( Mat(contours[i]), anothercontour, 3, true );
    // Bound shape in a rectangle
    Rect anotherect = boundingRect( Mat(anothercontour) );

    if(anotherect.width > (this->width*0.5))
    {
      int center = anotherect.x + anotherect.width/2;
      vector<Point> contoursL;
      vector<Point> contoursR;
      for(int j=contours[i].size()-1; j >=0; j--)
      {
        if(contours[i][j].x < center){
          contoursL.push_back(contours[i][j]);
        } else {
          contoursR.push_back(contours[i][j]);
        }
      }
      contours.push_back(contoursL);
      contours.push_back(contoursR);
      // Remove from countors of too small or large
      contours.erase(contours.begin()+i);
    }
  }

  // Approximate contours to polygons and get bounding rects
  vector<vector<Point> > contours_poly(contours.size());
  vector<Rect> boundRect(contours.size());

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
    double phi   = -((boundRect[i].tl().y + (boundRect[i].height/2.0f)) * CAM_VIEW_H) / this->height + 0.5 * CAM_VIEW_H;
    if((area < MIN_AREA) | (area > MAX_AREA) | (depth < MIN_DEPTH) | (depth > MAX_DEPTH) | (phi < MIN_PHI))
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
    double theta = -(((boundRect[i].tl().x + (boundRect[i].width/2.0f)) * CAM_VIEW_W) / this->width - 0.5 * CAM_VIEW_W);

    // Create and add into hazards list
    Hazard thisHaz = {
      i,          // id
      theta, phi, // center location
      w, h,       // width and height
      depth,      // depth
      HAZARD      // Hazard type
    };
    this->hazards->push_back( thisHaz );
  }
  //cout << contours.size() << " total obstacles detected in frame." << endl;

#if DISPLAY_WINDOWS
  // Create a new drawing with the original image and the countors in color
  Mat drawing;
  image.convertTo(drawing, CV_8U);
  cvtColor(drawing, drawing, CV_GRAY2RGB);
  for(unsigned int i=0; i< contours.size(); i++)
  {
    Scalar color = Scalar( 0, 0, 255 );
    drawContours( drawing, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
    rectangle( drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );
  }
  namedWindow("Con",0);
  imshow("Con", drawing);
  waitKey(1);
#endif
  //CLEAR_SCREEN;
  if (0){
  LOG_MESSAGE("Hazards: %d\n",(int)this->hazards->size());
  sort(this->hazards->begin(), this->hazards->end(), compareByLength);
  printHazards(this->hazards);
  }
  return;
}

/*
 * Calibrate the image processor with a plane estimatoin
 * based off of the background image.
 */
void ImageProcessor::calibrate(uint16_t* dataBuffer)
{
  // Create Mat wrapper
  Mat nextCalibration(this->height,this->width, CV_16U, dataBuffer);
  Mat working;
  // Convert bit depth to 0 to 256
  nextCalibration *= CONVERT_CONST;
  nextCalibration.convertTo(nextCalibration, CV_8U);

  // Prepare matrices for the data
  int max = CALIBRATION_POINTS;
  Mat res(3, 1, DataType<float>::type);
  Mat matX(max, 3, DataType<float>::type);
  Mat matZ(max, 1, DataType<float>::type);
  int row, col;

  // Fill in the matrix with the data
  for(int i=0; i<max; i++)
  {
    row = rng.uniform(0.5*this->height,0.9*this->height);
    col = rng.uniform(0.1*this->width,0.9*this->width);
    unsigned char val = nextCalibration.at<unsigned char>(row,col);
    if (val !=0){
      matX.at<float>(i,0) = col;
      matX.at<float>(i,1) = row;
      matX.at<float>(i,2) = val;
      matZ.at<float>(i,0) = 1;
    } else{
      i--;
    }
  }

  // Solve the equation
  solve(matX, matZ, res, DECOMP_SVD);

  float A = res.at<float>(0,0);
  float B = res.at<float>(1,0);
  float C = res.at<float>(2,0);

  LOG_MESSAGE("Plane Params: %f, %f, %f", A, B, C);

  // Plane generation
  for(int row=0; row < this->height; row++)
  {
    for(int col=0; col < this->width; col++)
    {
      int val = (-col*A - row*B + 1)/C;
      if(val > 255) val = 255;
      if(val < 0) val = 0;
      this->calibrationImage.at<unsigned char>(row, col) = (unsigned char)val;
    }
  }


  threshold(nextCalibration, working, 1, 1, CV_THRESH_BINARY_INV);
  multiply(working,this->calibrationImage,working);
  nextCalibration += working;
#if DISPLAY_WINDOWS
  LOG_MESSAGE("About to start calibration display");
  //namedWindow("Calibration2",0);
  //imshow("Calibration2", nextCalibration);
  waitKey(20);
#endif
}
