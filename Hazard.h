#ifndef HAZARD_H
#define HAZARD_H

#include <vector>
#include "macros.h"

struct Hazard {
  int id;
  double theta;
  double phi;
  double width;
  double height;
  int depth;
};

typedef std::vector<Hazard> HazardList;

struct HazXY {
  double x;
  double y;
  double r;
};

typedef std::vector<HazXY> HazXYList;

/* 
 * Custom comparison method for sorting Hazards list by depth
 */
inline bool compareByLength(const Hazard &a, const Hazard &b)
{
    return a.depth < b.depth;
}

/*
 * Print out hazard list in depth order
 */
inline void printHazards(HazardList* h_p)
{
  LOG_ERROR("Current hazards: (%lu):\n", h_p->size());
  int i=1;
  for(HazardList::iterator it= h_p->begin(); it != h_p->end(); ++it)
  {
    LOG_MESSAGE("Haz %d = center (%0.0f,%0.0f), depth: %dmm width: %0.0f, height: %0.0f\n", \
              i, it->theta, it->phi, it->depth, it->width, it->height);
    i++;
  }
}

#endif /* HAZARD_H */
