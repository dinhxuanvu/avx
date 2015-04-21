#ifndef PATHPLANNER_H
#define PATHPLANNER_H

#include "../Hazard.h"
#include "../Positioning/Positioning.h"
#include "../macros.h"

class PathPlanner
{
  public:
    PathPlanner(HazardList*);
    Path getDirection();
    
  private:
    HazardList* hazards;
    Positioning* position;
    Command previousCmd;
    int lastHazCount;
    //void localMapping();
    float bestPath(float);
};

#endif /* PATHPLANNER_H */
