#include <stdio.h>
#include <iostream>

#include "PathPlanner.h"
#include "../macros.h"

using namespace std;

int main(int argc, char** argv)
{
  
  HazardList* haz_p = new HazardList;

  PathPlanner pp(haz_p,100.0f);

  int i = 0;
  for(i=0; i< 100; i++)
  {
	// Create and add into hazards list
	Hazard thisHaz = {
	  i++,       // id
	  4+(double)i, 10,     // center location
	  2, 13,     // width and height
	  890        // depth
	};
	haz_p->push_back( thisHaz );
	// Create and add into hazards list
	Hazard thisHaz2 = {
	  i++,        // id
	  -3-(double)i, 8,       // center location
	  11, 29,     // width and height
	  1109        // depth
	};
	haz_p->push_back( thisHaz2 );
	cout << "Direction: " << pp.getDirection() << endl;
	usleep(100000);
	haz_p->pop_back();
	haz_p->pop_back();
  }
  return 0;
}

