#include "Control.h"

using namespace std;

/*
 * Public constructor for control module
 */
Control::Control()
{
  this->gpio = GPIO::instance();

  gpio->enableHBridge();
}

/*
 * Destructor for control module
 */
Control::~Control()
{
  this->gpio->disableHBridge();
  this->gpio->deinitialize();
}

/*
 * Update GPIO for new angle
 * Called every loop
 */
void Control::update(float angle)
{
  // Set both servos to same angle
  this->gpio->setTurn(angle);
  this->gpio->setCamera(angle);

  this->gpio->setSpeed(0.5f);
  this->gpio->updateBattery();
}
