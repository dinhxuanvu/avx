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
    PRINT_LCD("Heading: %0.0f\n",  pos.getHeadingOffset());
    usleep(100000);
  }

  return 0;
}

