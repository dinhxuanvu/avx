#include "Control.h"

using namespace std;

#define QUEUE_SIZE	20.0f

/*
 * Public constructor for control module
 */
Control::Control()
{
  this->gpio = GPIO::instance();

  this->turn = 0.0f;
  this->P = 0.8f;
  this->I = 0.9f;
  this->D = 0.0f;
  this->sum = 0.0f;
  this->gpio->enableHBridge();
  this->I = this->I/QUEUE_SIZE;
  Path p = {0, GO};
  this->lpath = p;
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
void Control::update(Path path)
{
  float turn;
  float speed;

  // Clear I term if changing direction
  if(path.cmd != this->lpath.cmd)
  {
    queue<float> empty;
    swap(this->errors,empty);
    this->sum = 0; // Reset PID I sum
  }
  // Save this path for next time
  this->lpath = path;
  // Get turn PID value
  turn = this->turnPID(path.angle);

  switch(path.cmd)
  {
    case GO:
      speed = this->speedPID(turn);
      break;
    case REVERSE:
      speed = -0.5; // 75% reverse
      break;
    case CAUTIOUS:
      speed = 0.1;
      break;
    default:
    case STOP:
      turn = 0;
      speed = 0;
      break;
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
  printf("P: %0.1f, I: %0.1f, D: %0.1f\n",this->P * error, this->I * this->sum + this->D * der);
  return turn;
}
