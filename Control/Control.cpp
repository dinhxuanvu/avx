#include "Control.h"

using namespace std;

/*
 * Public constructor for control module
 */
Control::Control()
{
  this->gpio = GPIO::instance();

  this->gpio->enableHBridge();
  this->turn = 0.0f;
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
 * Get the current turn angle.
 */
float Control::getTurn()
{
  return this->turn;
}

/*
 * Update GPIO for new angle
 * Called every loop
 */
void Control::update(float angle)
{
	float turn = this->turnPID(angle);
  this->turn = turn;
	float speed = this->speedPID(turn);

  // Set both servos to same angle
  this->gpio->setTurn(turn);
  this->gpio->setCamera(turn);
  // Set speed based on turn
  this->gpio->setSpeed(speed);
  // Update battery status
	this->gpio->updateBattery();
}

/*
 * Get the speed based on the turning direction
 */
float Control::speedPID(float turn)
{
  float speed;
  if(abs(turn) < 10)
  	speed = 1.0f;
  else if(abs(turn) < 20)
  	speed = 0.6f;
  else
  	speed = 0.3f;

  return speed;
}

/*
 * Get the turn direction based on best heading angle
 */
float Control::turnPID(float angle)
{
  float turn = angle;
  return turn;
}
