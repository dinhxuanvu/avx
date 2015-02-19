#include <stdio.h>
#include <iostream>
#include <cstdlib>

#include "Positioning.h"
#include "../macros.h"

using namespace std;
using namespace avx_bbb_i2c;
using namespace avx_hmc5883l;

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

