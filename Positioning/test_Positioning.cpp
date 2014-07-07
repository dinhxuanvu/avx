#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <iomanip>

#include "Positioning.h"
#include "../macros.h"

using namespace std;

int main(int argc, char** argv)
{
  Positioning pos;

  while(1)
  {
    cout << "Heading: " << fixed << setw(0) << setprecision(0) << pos.getHeadingOffset() << endl;
    usleep(100000);
  }

  return 0;
}

