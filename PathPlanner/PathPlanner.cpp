#include "PathPlanner.h"
#include <boost/thread.hpp>
#include <math.h>

using namespace std;

/*
 * Public constructor for path planning module
 */
PathPlanner::PathPlanner(HazardList* haz_p)
{
  // Hold pointer to hazards list
  this->hazards = haz_p;
#if DISPLAY_WINDOWS==0
  this->position = new Positioning();
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
  int numRays = 30;
  vector<double> histogram(numRays,0); 
  system("clear");
  for(int angleIndex = 0 ; angleIndex < numRays ; ++angleIndex)
  {
    double angle = angleIndex*90/numRays - 45;
    //Calcuate the potential of each obsticle.
    for(HazardList::iterator it= this->hazards->begin(); it != this->hazards->end(); ++it)
    {
      double theta = it->theta;
      double angularWidth = it->width;
      if (theta - 0.5*angularWidth <= angle && theta + 0.5*angularWidth >= angle){
         double depth = it->depth;
         histogram[angleIndex] = histogram[angleIndex] + depth*depth;
      } else
      {
        //printf("Neg:%0.0f Angle:%0.0f Pos:%0.0f\n" ,theta - 0.5*angularWidth, angle ,theta + 0.5*angularWidth);
      }
    }
    printf("Angle %0.0f=%0.0f\n",angle,histogram[angleIndex]);

  }
  boost::this_thread::sleep(boost::posix_time::milliseconds(150));
  // Paul here
  return 0.0f;
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

  // Get heading from Positioning module
#if DISPLAY_WINDOWS
  float headingDisp = -26.0f;
#else
  float headingDisp = this->position->getHeadingOffset();
#endif
  headingDisp -= camAngle;

  //HazXYList hazXY;
  //this->localMapping();
  dir = this->bestPath();

  dir += camAngle;

  return dir;
}