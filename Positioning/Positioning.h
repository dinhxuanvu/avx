#ifndef POSITIONING_H
#define POSITIONING_H

#include "../macros.h"
#include "HMC5883L.h"

class Positioning
{
  public:
    Positioning();
	/*
	 * Get's the displacement between the current heading from magno 
	 * and the desired heading during calibration.
	 */
    float getHeadingOffset();
	/*
	 * Calibrate positionign module by setting current direction as desired heading
	 */
    void setTarget();
    void calibrate();
    int getMagX();
    int getMagY();
    int getMagZ();

  private:
    BBB_I2C i2c;
    HMC5883L hmc;
    float getHead();
    float getHeading(int);
    float target;
    float lastH;
    int calX, calY;
};

#endif /* POSITIONING_H */
