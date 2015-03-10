#include <iostream>

#include "GPIO.h"

using namespace std;

int testGPIOunits(void);

int main(void)
{
  int status = 1;
  cout << "Welcome to WALLE testing." << endl;

  //status &= testGPIOunits();
  cout << "Initializing GPIO: ";
  GPIO *gpio = GPIO::instance();

  gpio->setTurn(-36.0f);
  cin.ignore();
  cout << "Verify wheels turned full right" << endl;
  gpio->setTurn(+36.0f);
  cin.ignore();
  cout << "Verify wheels centered" << endl;
  gpio->setTurn(0.0f);
  cin.ignore();
  
  gpio->enableHBridge();

  gpio->setSpeed(0.0f);
  cin.ignore();
  gpio->setSpeed(0.2f);
  cin.ignore();

  gpio->disableHBridge();

  // Status LEDs
  if(status)
    cout << "All tests were successful" << endl;

  return 0;
}

int testGPIOunits(void)
{
  cout << "Testing GPIO module" << endl << endl;
  cout << "Initializing GPIO: ";
  GPIO *gpio = GPIO::instance();
  cout << "Successful" << endl;
  cin.ignore();
  // Status LEDs
  cout << "Testing battery LEDs" << endl;
  gpio->updateBattery();
  cin.ignore();
  cout << "Battery testing complete" << endl;

  // Motors currently match
  cout << "Testing H-Bridge PWM outputs" << endl;
  gpio->enableHBridge();

  gpio->setSpeed(0.05f);
  cout << "Verify P9.22 and P9.14 are at 5% duty cycle, 2000Hz" << endl;
  cin.ignore();
  gpio->setSpeed(0.50f);
  cin.ignore();
  cout << "Verify P9.22 and P9.14 are at 50% duty cycle, 2000Hz" << endl;
  gpio->setSpeed(0.95f);
  cin.ignore();
  cout << "Verify P9.22 and P9.14 are at 95% duty cycle, 2000Hz" << endl;
  gpio->setSpeed(0.95f);
  cin.ignore();
  cout << "Verify P9.22 and P9.14 are at 95% duty cycle, 2000Hz" << endl;

  gpio->disableHBridge();
  
  // Servo 1
  cout << "Testing PWM outputs" << endl;

  gpio->enableHBridge();

  cout << "Verify wheels turned full left" << endl;
  gpio->setTurn(-90.0f);
  cin.ignore();
  cout << "Verify wheels turned full right" << endl;
  gpio->setTurn(+90.0f);
  cin.ignore();
  cout << "Verify wheels centered" << endl;
  gpio->setTurn(0.0f);
  cin.ignore();

  // Servo 2
  cout << "Verify camera turned full left" << endl;
  gpio->setCamera(-90.0f);
  cin.ignore();
  cout << "Verify camera turned full right" << endl;
  gpio->setCamera(+90.0f);
  cin.ignore();
  cout << "Verify camera centered" << endl;
  gpio->setCamera(0);
  cin.ignore();
  
  gpio->disableHBridge();
  
  cout << "Closing GPIO: ";
  gpio->deinitialize();
  cout << "Successful" << endl;

  return 1;
}
