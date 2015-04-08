#ifndef HAZARD_H
#define HAZARD_H

#include <vector>
#include "macros.h"

enum Type { HAZARD, STOP };

struct Hazard {
  int id;
  double theta;
  double phi;
  double width;
  double height;
  int depth;
  Type type;
};

typedef std::vector<Hazard> HazardList;

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
    if(it->type == HAZARD)
    {
      LOG_MESSAGE("Haz %d = center (%0.0f,%0.0f), depth: %dmm width: %0.0f, height: %0.0f\n", \
                i, it->theta, it->phi, it->depth, it->width, it->height);
    } else if(it->type == STOP) {
      LOG_MESSAGE("STOP HAZARD\n");
    }
    i++;
  }
}

#endif /* HAZARD_H */
