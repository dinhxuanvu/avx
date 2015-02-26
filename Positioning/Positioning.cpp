#include "Positioning.h"
#include "BBB_I2C.h"
#include "HMC5883L.h"

using namespace std;

/*
 * Public constructor for positioning module
 */
Positioning::Positioning():
hmc(i2c)
{
    this->hmc.initialize();
    this->target = 0;
}

/*
 * Calibrate positionign module by setting current direction as desired heading
 */
void Positioning::setTarget()
{
    float heading = this->getHeading();
    this->target = heading;
}

/*
 * Get Magnitude in X direction from i2c
 */
int Positioning::getMagX()
{
  return this->hmc.getMagnitudeX();
}


/*
 * Get Magnitude in Y direction from i2c
 */
int Positioning::getMagY()
{
  return this->hmc.getMagnitudeY();
}


/*
 * Get Magnitude in Z direction from i2c
 */
int Positioning::getMagZ()
{
  return this->hmc.getMagnitudeZ();
}


/*
 * Get heading from magnetometer from 0 to 360 degress
 */
float Positioning::getHeading()
{
    int X = this->getMagX();
    int Y = this->getMagY();
    this->getMagZ();

    // Calculate heading
    float heading = atan2(Y, X);
    
    // Declineation Angle at Rochester (-11o30')
    float declinationAngle = -0.200712864;
    heading += declinationAngle;
    
    // Correct for when signs are reversed->
    if(heading < 0)
        heading += 2*M_PI;
    
    // Check for wrap due to addition of declination->
    if(heading > 2*M_PI)
        heading -= 2*M_PI;
    
    // Convert radians to degrees
    heading = heading * 180/M_PI;
    
    return heading;
}

/*
 * Get's the displacement between the current heading from magno 
 * and the desired heading during calibration.
 */
float Positioning::getHeadingOffset()
{
    float current = this->getHeading();
    return current - this->target;
}
