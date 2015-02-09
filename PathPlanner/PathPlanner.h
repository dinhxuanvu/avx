#ifndef PATHPLANNER_H
#define PATHPLANNER_H

#include "../Hazard.h"
#include "../macros.h"

class PathPlanner
{
  public:
    PathPlanner(HazardList*);
    float getDirection();
  private:
    HazardList* hazards;
};

#endif /* PATHPLANNER_H */
