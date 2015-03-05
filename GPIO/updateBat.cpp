#include <iostream>

#include "GPIO.h"

using namespace std;


int main(void)
{
  GPIO *gpio = GPIO::instance();

  // Status LEDs
  gpio->updateBattery();

  return 0;
}

