#include <stdio.h>
#include <iostream>
#include <cstdlib>

#include "Positioning.h"
#include "../macros.h"

using namespace std;

int main(int argc, char** argv)
{
  Positioning pos;

  while(1)
  {
    cout << "Val: " << pos.getHeading() << endl;
    usleep(100000);
  }

  return 0;
}

