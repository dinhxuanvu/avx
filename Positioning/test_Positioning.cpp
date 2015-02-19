#include <stdio.h>
#include <iostream>

#include "Positioning.h"
#include "../macros.h"

using namespace std;

int main(int argc, char** argv)
{
  Positioning* pos = new Positioning();

  cout << pos->getHeading() << endl;

  free(pos);

  return 0;
}

