#ifndef POSITIONING_H
#define POSITIONING_H

#include "../Hazard.h"
#include "../macros.h"
#include "HMC5883L.h"

class Positioning
{
  public:
    Positioning();
    float getHeading();
  private:
    BBB_I2C i2c;
    HMC5883L hmc;
    int getMagX();
    int getMagY();
    int getMagZ();
};

#endif /* POSITIONING_H */
