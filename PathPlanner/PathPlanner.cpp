#include "PathPlanner.h"
#include <boost/thread.hpp>
#include <math.h>

using namespace std;
#define CAM_VIEW_W   56.0f
#define HALF_CAM_VIEW_W  (CAM_VIEW_W/2.0f)
#define CAM_WEIGHT  200000
#define COMP_WEIGHT 100
#define OBSTACLE_SIZE 150

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
/*
void PathPlanner::localMapping()
{
  for(HazardList::iterator it= this->hazards->begin(); it != this->hazards->end(); ++it)
  {
    double r = (double)it->depth;
    double theta = it->theta-90;
    double width = 2*tan(it->width * M_PI / 360.0)*r;
    // Center point
    double x = r*cos(theta * M_PI / 180.0);
    double y = r*sin(theta * M_PI / 180.0);
    HazXY thisHaz = { x,y,width };
    hazXY.push_back( thisHaz );
  }
}
*/

float PathPlanner::bestPath()
{
  system("clear");
  int numRays = 30;
  float servoCompensatedCompasHeading;
  #if DISPLAY_WINDOWS==0
    servoCompensatedCompasHeading = 0; //this->position->getHeadingOffset();
  #else
    servoCompensatedCompasHeading = 0;
  #endif

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
    printf("Angle %-5.1f Camera =%-03.0f Compass = %-03.0f\n",angle,histogram[histIndex]-histogram_compass[histIndex],histogram_compass[histIndex]);
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
float PathPlanner::getDirection()
{
  // Always go straight for now.
  float dir;

  // Get camera angle
  float camAngle = 0.0f;

  // Check for stop hazard
  if(this->hazards->front().type == STOP)
  {
    return -100;
  }


  //HazXYList hazXY;
  dir = this->bestPath();

  dir += camAngle;

  printf("Turn %0.0f\n",dir);
  printHazards(this->hazards);

  return dir;
}
