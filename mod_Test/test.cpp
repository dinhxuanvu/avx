#include <stdio>
#include <iostream>

#include "../mod_GPIO/GPIO.h"

using namespace std;

int main(void)
{
  int status = 1;
  cout << "Welcome to WALLE testing." << endl;

  status &= testGPIOunits();

  if(status)
    cout << "All tests were successful" << endl;

  return 0;
}

int testGPIOunits(void)
{
  cout << "Testing GPIO module" << endl << endl;
  cout << "Initializing GPIO: ";
  GPIO::initialize();
  cout << "Successful" << endl;
  // Status LEDs
  cout << "Testing battery LEDs" << endl;
  for(int per=10; per<100; per+=10)
  {
    GPIO::setBatteryLEDs(per);
    cout << "\tBattery level: " << per << endl;
    cin.ignore();
  }
  cout << "Battery testing complete" << endl;

  // Motors currently match
  cout << "Testing H-Bridge PWM outputs" << endl;
  GPIO::enableHBridge();

  GPIO::setSpeed(5);
  cout << "Verify P9.22 and P9.14 are at 5% duty cycle, 2000Hz" << endl;
  GPIO::setSpeed(50);
  cin.ignore();
  cout << "Verify P9.22 and P9.14 are at 50% duty cycle, 2000Hz" << endl;
  GPIO::setSpeed(95);
  cin.ignore();
  cout << "Verify P9.22 and P9.14 are at 95% duty cycle, 2000Hz" << endl;
  GPIO::setSpeed(95);
  cin.ignore();
  cout << "Verify P9.22 and P9.14 are at 95% duty cycle, 2000Hz" << endl;

  GPIO::disableHBridge();
  
  // Servo 1
  cout << "Testing PWM outputs" << endl;

  GPIO::enableHBridge();

  cout << "Verify wheels turned full left" << endl;
  GPIO::setTurn(-100);
  cin.ignore();
  cout << "Verify wheels turned full right" << endl;
  GPIO::setTurn(+100);
  cin.ignore();
  cout << "Verify wheels centered" << endl;
  GPIO::setTurn(+100);
  cin.ignore();

  // Servo 2
  cout << "Verify camera turned full left" << endl;
  GPIO::setCamera(-100);
  cin.ignore();
  cout << "Verify camera turned full right" << endl;
  GPIO::setCamera(+100);
  cin.ignore();
  cout << "Verify camera centered" << endl;
  GPIO::setCamera(+100);
  cin.ignore();
  
  GPIO::disableHBridge();
  
  cout << "Closing GPIO: ";
  GPIO::deinitialize();
  cout << "Successful" << endl;

  return 1;
}