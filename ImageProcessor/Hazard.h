#ifndef HAZARD_H
#define HAZARD_H

#include <vector>

struct Hazard {
  int id;
  double theta;
  double phi;
  double width;
  double height;
  int depth;
};

typedef std::vector<Hazard> HazardList;

#endif /* HAZARD_H */
