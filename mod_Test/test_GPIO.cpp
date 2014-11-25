#include <iostream>

#include "../mod_GPIO/GPIO.h"

using namespace std;

int testGPIOunits(void);

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
  GPIO *gpio = GPIO::instance();
  cout << "Successful" << endl;
  // Status LEDs
  cout << "Testing battery LEDs" << endl;
  for(int per=10; per<100; per+=10)
  {
    gpio->setBatteryLEDs(per);
    cout << "\tBattery level: " << per << endl;
    cin.ignore();
  }
  cout << "Battery testing complete" << endl;

  // Motors currently match
  cout << "Testing H-Bridge PWM outputs" << endl;
  gpio->enableHBridge();

  gpio->setSpeed(0.5f);
  cout << "Verify P9.22 and P9.14 are at 5% duty cycle, 2000Hz" << endl;
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
  gpio->setTurn(-1.0f);
  cin.ignore();
  cout << "Verify wheels turned full right" << endl;
  gpio->setTurn(+1.0f);
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

  // ADC reading battery
  float lev = gpio->getBatteryLevel();
  cout << "Battery reading at " << lev << "%" << endl;
  
  cout << "Closing GPIO: ";
  gpio->deinitialize();
  cout << "Successful" << endl;

  return 1;
}
