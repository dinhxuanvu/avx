#include <iostream>
#include <fstream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "../GPIO/GPIO.h"
#include "../Positioning/Positioning.h"

using namespace std;
using namespace cv;

#define CAL_SIZE   4000

int testGPIOunits(void);

int main(void)
{
  GPIO *gpio = GPIO::instance();
  Positioning pos;

  gpio->setTurn(+40.0f);
  gpio->enableHBridge();
  gpio->setSpeed(0.75f);
  cout << "Calibrating. Press enter when 360 complete" << endl;
  int X,Y;
  Mat cal;
  cal = Mat::zeros(CAL_SIZE, CAL_SIZE, CV_8U);
  Point lastP;
  for(int i=0;i<100;i++)
  {
    X = pos.getMagX();
    Y = pos.getMagY();
    pos.getMagZ();
    X += CAL_SIZE/2.0f;
    Y += CAL_SIZE/2.0f;
    Point thisP(Y, X);
    if(i != 0)
    {
      line( cal, thisP, lastP, Scalar(255), 32, 8 );
    }
    lastP = thisP;
    usleep(33000);
  }
  gpio->setSpeed(0.0f);
  gpio->disableHBridge();
  cout << "Finished calibrating, calculating circle" << endl;
  vector<Vec3f> circles;
  HoughCircles( cal, circles, CV_HOUGH_GRADIENT, 1, cal.rows/4, 200, 10, 500, 1200);
  if(circles.size() > 0)
  {
    int Ycen = cvRound(circles[0][0]); // - CAL_SIZE/2.0f;
    int Xcen = cvRound(circles[0][1]); // - CAL_SIZE/2.0f;
    Point center(Ycen,Xcen);
    int radius = cvRound(circles[0][2]);
    circle( cal, center, 3, Scalar(128), -1, 8, 0 );
    circle( cal, center, radius, Scalar(128), 3, 8, 0 );
    imwrite( "generated.jpg", cal );
    cout << "Center: (" << Xcen << "," << Ycen << ") Radius: " << radius << endl;
  } else {
    cout << "No circles found :(" << endl;
  }
}

