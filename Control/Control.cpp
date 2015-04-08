#include "Control.h"

using namespace std;

#define QUEUE_SIZE	30

/*
 * Public constructor for control module
 */
Control::Control()
{
  this->gpio = GPIO::instance();

  this->turn = 0.0f;
  this->P = 1.2f;
  this->I = 0.2f;
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
  float turn;
  float speed;
  // If stop hazard, go in reverse straight
  if(angle == -100)
  {
    turn = this->turn*1.5;
    speed = -1.0; // 75% reverse
    this->sum = 0; // Reset PID I sum
  }
  // Otherwise do PID
  else
  {
    turn = this->turnPID(angle);
    speed = this->speedPID(turn);
  }

  // Save current turn value
  this->turn = turn;
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
  // Cap speed based on maximum angles
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

  if(this->errors.size() > QUEUE_SIZE)
  {
    this->sum -= this->errors.front();
    this->errors.pop();
  }
  
  turn = this->P * error + this->I * this->sum + this->D * der;
  return turn;
}
