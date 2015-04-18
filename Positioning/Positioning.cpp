#include "Positioning.h"
#include "BBB_I2C.h"
#include "HMC5883L.h"
#include "../GPIO/GPIO.h"

using namespace std;

/*
 * Public constructor for positioning module
 */
Positioning::Positioning():
hmc(i2c)
{
    this->hmc.initialize();
    this->target = 180;
    this->calX = -15;
    this->calY = 153;
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

void Positioning::calibrate()
{
    PRINT_LCD("Calibrating compass\n");
    int maxX = 0; int maxY = 0;
    int minX = 0; int minY = 0;
    GPIO *gpio = GPIO::instance();
    gpio->startCircle();
    for(int i=0; i<500; i++)
    {
      int X = this->getMagX();
      int Y = this->getMagY();
      this->getMagZ();
      if(X > maxX) { maxX = X; } 
      if(Y > maxY) { maxY = Y; } 
      if(X < minX) { minX = X; } 
      if(Y < minY) { minY = Y; }
      usleep(200000);
    }
    gpio->stopCircle();
    // ENsure range is big enough to calibrate off of
    if(((maxX - minX) > 300) & ((maxY-minY) > 300))
    {
      this->calX = (float)(maxX + minX) / -2.0f;
      this->calY = (float)(maxY + minY) / -2.0f;
      printf("Compass clibrated.\n X-offset: %d Y-offset: %d\n",this->calX,this->calY);
      PRINT_LCD("Compass calibrated.\n X=%d Y=%d",this->calX, this->calY);
    } else {
      PRINT_LCD("Calibration failed.\n");
    }
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
  return (int)this->hmc.getMagnitudeY();
}


/*
 * Get Magnitude in Z direction from i2c
 */
int Positioning::getMagZ()
{
  return (int)this->hmc.getMagnitudeZ();
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
    X = this->getMagX()+this->calX;
    Y = this->getMagY()+this->calY;
    this->getMagZ();

    // Calculate heading
    float heading = atan2(X, Y);
    
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
    
    //printf("Heading current: %0.0f, Target: %0.0f\n",current, this->target);

    float offset = this->target - current;
    if(offset > 180)
      offset -= 360;
    if(offset < -180)
      offset += 360;
    return offset;
}
