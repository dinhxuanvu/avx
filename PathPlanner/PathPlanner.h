#ifndef PATHPLANNER_H
#define PATHPLANNER_H

#include "../Hazard.h"
#include "../macros.h"
#include "HMC5883L.h"

class PathPlanner
{
  public:
    PathPlanner(HazardList*);
    int getMagX();
    int getMagY();
    int getMagY();
    int getHeading();
    float getDirection();
    
  private:
    HazardList* hazards;
    BBB_I2C i2c;
    HMC5883L hmc(i2c);
};

#endif /* PATHPLANNER_H */
