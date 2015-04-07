#include "Control.h"

using namespace std;

/*
 * Public constructor for control module
 */
Control::Control()
{
  this->gpio = GPIO::instance();

  this->turn = 0.0f;
  this->P = 1.0f;
  this->I = 0.3f;
  this->D = 0.0f;
  this->sum = 0.0f;
  this->gpio->enableHBridge();

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
}

/*
 * Get the speed based on the turning direction
 */
float Control::speedPID(float turn)
{
  if(turn > 36)
    turn = 36.0f;
  if(turn < -36)
    turn = -36.0f;

  float turnD = abs(turn);
  float speed = 0.01 + (1- (turnD/36.0f))*1.0;
  return speed;
}

/*
 * Get the turn direction based on best heading angle
 */
float Control::turnPID(float angle)
{
  float turn;
  float error = angle;

  this->errors.push(error);

  float der = error;
  
  this->sum += error;

  if(this->errors.size() > 30)
  {
    this->sum -= this->errors.front();
    this->errors.pop();
  }
  
  turn = this->P * error + this->I * this->sum + this->D * der;
  return turn;
}
