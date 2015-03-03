#include "PathPlanner.h"

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

void PathPlanner::localMapping(HazXYList& hazXY)
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

float PathPlanner::bestPath(HazXYList& hazXY)
{
  //Calcuate the potential of each obsticle.
  for(HazXYList::iterator it= this->hazards->begin(); it != this->hazards->end(); ++it)
  {
    double x = it->x;
    double y = it->y;
    double r = it->r;
    double distanceToHazard = sqrt(x*x + y*y) - r;
    
    

  }
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

  HazXYList hazXY;
  this->localMapping(hazXY);
  dir = this->bestPath(hazXY);

  dir += camAngle;

  return dir;
}