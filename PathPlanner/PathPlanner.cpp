#include "PathPlanner.h"
#include <boost/thread.hpp>
#include <math.h>

using namespace std;
#define CAM_VIEW_W   56.0f
#define HALF_CAM_VIEW_W  (CAM_VIEW_W/2.0f)
#define CAM_RATIO  2
#define COMP_RATIO 1
#define CAM_WEIGHT (CAM_RATIO*200000)
#define COMP_WEIGHT (COMP_RATIO*150)
#define OBSTACLE_SIZE 350

/*
 * Public constructor for path planning module
 */
PathPlanner::PathPlanner(HazardList* haz_p)
{
  // Hold pointer to hazards list
  this->hazards = haz_p;
#if DISPLAY_WINDOWS==0
  this->position = new Positioning();
  this->position->setTarget();
#endif
}

float PathPlanner::bestPath(float servoCompensatedCompasHeading)
{
  int numRays = 30;
  int targetIndex = floor((servoCompensatedCompasHeading + HALF_CAM_VIEW_W)*numRays/CAM_VIEW_W);

  printf("TargetIndex:%d Angle:%0.0f\n",targetIndex, targetIndex*CAM_VIEW_W/numRays - HALF_CAM_VIEW_W);

  vector<double> histogram(numRays,0);
  vector<double> histogram_compass(numRays,0);

  for(int angleIndex = 0 ; angleIndex < numRays ; ++angleIndex)
  {
    double angle = angleIndex*CAM_VIEW_W/numRays - HALF_CAM_VIEW_W;
    double carWidthMM = OBSTACLE_SIZE;
    //Calcuate the potential of each obsticle.
    for(HazardList::iterator it= this->hazards->begin(); it != this->hazards->end(); ++it)
    {
      double theta = it->theta;
      double angularWidth = it->width;
      double depth = it->depth;
      double carAngularWidthAtDepth = 180 / M_PI * atan(carWidthMM/(2*depth));
      //if (theta - 0.5*angularWidth <= angle && theta + 0.5*angularWidth >= angle){
      //   histogram[angleIndex] = histogram[angleIndex] - 10000*1.0f/depth;
      //} else 

      //Remove score for being with a car width of the obsticle
      if (theta - 0.5*angularWidth - carAngularWidthAtDepth <= angle && theta + 0.5*angularWidth + carAngularWidthAtDepth >= angle){
         histogram[angleIndex] = histogram[angleIndex] - CAM_WEIGHT*(angularWidth/2.0f + carAngularWidthAtDepth  - abs(theta - angle))/(depth*CAM_VIEW_W);
      } else

      {
        //printf("Neg:%0.0f Angle:%0.0f Pos:%0.0f\n" ,theta - 0.5*angularWidth, angle ,theta + 0.5*angularWidth);
      }
    }
    //Add some score for being close to the target
    double compassScore = CAM_VIEW_W - COMP_WEIGHT*abs(angleIndex - targetIndex)/CAM_VIEW_W;
    histogram[angleIndex] += compassScore;
    histogram_compass[angleIndex] += compassScore; 
  }
  int maxIndex = 0;
  for(int histIndex = 0 ; histIndex < numRays ; ++histIndex)
  {
    if (histogram[histIndex] > histogram[maxIndex]){
      maxIndex = histIndex;
    }
    double angle = histIndex*CAM_VIEW_W/((float)numRays) - HALF_CAM_VIEW_W;
    int counter = 0;
    for(int compareIndex = 0 ; compareIndex < numRays ; ++compareIndex)
    {
      if (histogram[histIndex] <= histogram[compareIndex])
      {
        counter++;
      }
    }
    //printf("Angle %-5.1f=%d\n",angle,counter);//histogram[angleIndex]);
    printf("Angle %-5.1f Camera =%-3.0f Compass = %-3.0f\n",angle,histogram[histIndex]-histogram_compass[histIndex],histogram_compass[histIndex]);
  }
  float maxAngle = ((float)maxIndex*CAM_VIEW_W)/((float)numRays) - HALF_CAM_VIEW_W;
  //boost::this_thread::sleep(boost::posix_time::milliseconds(150));
  // Paul here
  return maxAngle;
}



/*
 * Get the best direction to go based on hazards.
 * Returns turn angle which can be passed to GPIO
 */ 
Path PathPlanner::getDirection()
{
  float dir;
  float compass;

  // Get camera angle
  float camAngle = 0.0f;

  system("clear");


  #if DISPLAY_WINDOWS==0
    compass = -this->position->getHeadingOffset();
  #else
    compass = 0;
  #endif

  // Check for stop hazard
  if(!(this->hazards->empty()))
  {
    if(this->hazards->front().type == BLOCK)
    {
      PRINT_LCD("REVERSE\n");
      Path p = {-compass, REVERSE}; 
      return p;
    }
  }
  Command cmd;
  if(!(this->hazards->empty()))
  {
    if(this->hazards->front().type == HAZARD)
    {
      if((this->hazards->front().depth < 600))
      {
        PRINT_LCD("CAUTIOUS\n");
        cmd = CAUTIOUS;
      }
      else
      {
        PRINT_LCD("GO\n");
        cmd = GO;
      }
    }
  }

  //HazXYList hazXY;
  dir = this->bestPath(compass);

  dir += camAngle;

  printf("Turn %0.0f\n",dir);
  printHazards(this->hazards);
  PRINT_LCD("Turn %0.0f\n",dir);
  cmd = CAUTIOUS;
  Path p = {dir, cmd};
  return p;
}
