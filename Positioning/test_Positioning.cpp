#include <stdio.h>
#include <iostream>
#include <cstdlib>

#include "Positioning.h"
#include "../macros.h"

using namespace std;

int main(int argc, char** argv)
{
  Positioning* pos = new Positioning();
  while(1)
  {
    cout << pos->getHeading() << endl;
    std::system("sleep 1");
  }

  free(pos);

  return 0;
}

