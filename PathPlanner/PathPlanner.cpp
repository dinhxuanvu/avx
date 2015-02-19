#include "PathPlanner.h"

using namespace std;
using namespace avx_bbb_i2c;
using namespace avx_hmc5883l;

/*
 * Public constructor for path planning module
 */
PathPlanner::PathPlanner(HazardList* haz_p)
{
  // Hold pointer to hazards list
  this->hazards = haz_p;
  // Initialize HMC5883L module
  hmc.initialize();
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
  float heading = 0.0f;
  // Calculate clear direction from hazards list
  float hazDir = 0.0f;

  //INSERT COMPLICATED STUFF HERE
  

  // Linear combination of those offset by angle
  dir = (0.5*heading + 0.5*(hazDir+camAngle));

  return dir;
}

/*
 *
 *
 */
int PathPlanner::getMagX()
{
    return hmc.getMagnitudeX();
}

/*
 *
 *
 */
int PathPlanner::getMagY()
{
    return hmc.getMagnitudeY();
}

/*
 *
 *
 */
int PathPlanner::getMagZ()
{
    return hmc.getMagnitudeZ();
}

/*
 *
 *
 */
int PathPlanner::getHeading()
{
    // Calculate heading
    float heading = atan2((float)hmc.getMagnitudeY(), (float)hmc.getMagnitudeX());
    
    // Declineation Angle at Rochester (-11o30')
    float declinationAngle = -0.200712864;
    heading += declinationAngle;
    
    // Correct for when signs are reversed.
    if(heading < 0)
        heading += 2*M_PI;
    
    // Check for wrap due to addition of declination.
    if(heading > 2*M_PI)
        heading -= 2*M_PI;
    
    // Convert radians to degrees
    heading = heading * 180/M_PI;
    
    return (int)heading;
}

