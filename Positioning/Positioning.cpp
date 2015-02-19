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
    int Z = this->getMagZ();

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
