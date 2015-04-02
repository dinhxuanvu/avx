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
  private:
    BBB_I2C i2c;
    HMC5883L hmc;
    int getMagX();
    int getMagY();
    int getMagZ();
    float getHeading();
    float getHeading1();
    float target;
};

#endif /* POSITIONING_H */
