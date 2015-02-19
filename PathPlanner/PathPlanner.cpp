#include "PathPlanner.h"

using namespace std;

/*
 * Public constructor for path planning module
 */
PathPlanner::PathPlanner(HazardList* haz_p)
{
  // Hold pointer to hazards list
  this->hazards = haz_p;
  this->position = new Positioning();

}

/*
 * Get the best direction to go based on hazards.
 * Returns turn angle which can be passed to GPIO
 */ 
float PathPlanner::getDirection()
{ 
  // Always go straight for now.
  float dir = 0.0f;
  // Get camera angle
  float camAngle = 0.0f;
  // Get heading from Positioning module
  float heading = this->position->getHeading();
  // Calculate clear direction from hazards list
  float hazDir = 0.0f;

  //INSERT COMPLICATED STUFF HERE
  

  // Linear combination of those offset by angle
  dir = (0.5*heading + 0.5*(hazDir+camAngle));

  return dir;
}
