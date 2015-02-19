#ifndef PATHPLANNER_H
#define PATHPLANNER_H

#include "../Hazard.h"
#include "../Positioning/Positioning.h"
#include "../macros.h"

class PathPlanner
{
  public:
    PathPlanner(HazardList*);
    float getDirection();
    
  private:
    HazardList* hazards;
    Positioning* position;
};

#endif /* PATHPLANNER_H */
