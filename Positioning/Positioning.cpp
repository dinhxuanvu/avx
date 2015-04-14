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
    this->target = 180;
}

/*
 * Calibrate positionign module by setting current direction as desired heading
 */
void Positioning::setTarget()
{
    float heading = this->getHeading(5);
    this->target = heading;
    this->lastH = heading;
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
 * Get heading from magnetometer from 0 to 360 degress by average three attempts
 */
float Positioning::getHead()
{
  float heading = 0;
  heading = this->getHeading(5);
  this->lastH = heading;
  return heading;
}

/* 
 * Gets one heading value
 */
float Positioning::getHeading(int n)
{
    int X = 0; int Y = 0;
    for(int i=0; i<n; i++)
    {
      X += this->getMagX();
      Y += this->getMagY();
      this->getMagZ();
      usleep(100);
    }

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
    float current = this->getHead();
    
    printf("Heading current: %0.0f, Target: %0.0f\n",current, this->target);

    float offset = this->target - current;
    return offset;
}
