#include "GPIO.h"
#include <stdlib.h>
#include "../macros.h"

#define BBBIO_PWMSS2 1
#define BBBIO_ADC_AIN1 1
#define BBBIO_PWMSS0 1
#define BBBIO_PWMSS1 1

using namespace std;

// Private instance for singleton
GPIO * GPIO::s_instance = NULL;

// Private constructor
GPIO::GPIO(void)
{

  duty_wheels = TURN_CENTER;
  duty_camera = CAM_CENTER;
  this->setBatteryLEDs(1.0);
}

// Disable the hbridge and free the IO library
int GPIO::deinitialize(void)
{
  return 1;
}

// Set a single motor PWM value, used by setSpeed.
int GPIO::setMotor(int motor, float duty1, float duty2)
{
  printf("Motor set: %d, %lf, %f, %f\n",motor, MOTOR_FREQ, duty1, duty2);
  return 1;
}

// Get an instance of the GPIO, used instead of contructor for 
// singleton pattern
GPIO * GPIO::instance()
{
  if(!s_instance)
    s_instance = new GPIO;
  return s_instance;
}

/* Enable the H-Bridge on the motor board */
int GPIO::enableHBridge(void)
{
  printf("HBridge Enabled\n");
  return 1;
}

/* Disable the H-Bridge on the motor board */
int GPIO::disableHBridge(void)
{
  printf("HBridge Disabled\n");
  return 1;
}

/* setBattery - sets battery level by 4 LEDs on motor-board
 * Param: percent (float 0.0 to 1.0)
 */
int GPIO::setBatteryLEDs(float percent)
{

  printf("Battery: ");
  // Based on level light up LEDs
  if(percent > 0.05)
    printf("*");
  if(percent > 0.2)
    printf("*");
  if(percent > 0.4)
    printf("*");
  if(percent > 0.65)
    printf("*");
  printf("\n");
  return 1;
}

/* getBatteryLevel - gets the battery level as a decimal (0.0 to 1.0)*/
float GPIO::getBatteryLevel(void)
{
  float level = 0.5f;

  // Restrict to [0,1]
  if(level > 1)
    level = 1;
  if(level < 0)
    level = 0;

  return level;
}

/* Get battery level from sensor and update leds */
int GPIO::updateBattery(void)
{
  float bat = this->getBatteryLevel();
  this->setBatteryLEDs(bat);
  return 1;
}

/* setTurn - sets the turning servo angle
 * param: float angle - [-90,90] range */
int GPIO::setTurn(float angle)
{
  float new_duty;

  // Update the battery whenever we turn
  this->updateBattery();

  // Scale by degress
  angle = angle/90;

  // Check bounds
  if(angle > 1)
    angle = 1;
  if(angle < -1)
    angle = -1;
  
  // Calculate duty % linearly
  if(angle > 0)
    new_duty = (TURN_RIGHT-TURN_CENTER)*angle + TURN_CENTER;
  else
    new_duty = (TURN_CENTER-TURN_LEFT)*angle + TURN_CENTER;

  // Update class var
  duty_wheels = new_duty;
  return 1;
}

/* setCamera - sets the camera angle
 * param: angle - [-90,90] degrees from center */
int GPIO::setCamera(float angle)
{
  float new_duty;

  // Scale by degress
  angle = angle/90;

  // Check bounds
  if(angle > 1)
    angle = 1;
  if(angle < -1)
    angle = -1;

  // Calculate duty % linearly
  if(angle > 0)
    new_duty = (CAM_RIGHT-CAM_CENTER)*angle + CAM_CENTER;
  else
    new_duty = (CAM_CENTER-CAM_LEFT)*angle + CAM_CENTER;
  
  // Update class var
  duty_camera = new_duty;
  return 1;
}

/* setSpeed - sets motor speed based on value.
 * param: speed in [-1,1], where < 0 is reverse, and > 0 is forward
 */
int GPIO::setSpeed(float speed)
{
  float duty [2] = { 1.0, 1.0 };
  // Check bounds
  if(speed > 1)
    speed = 1;
  if(speed < -1)
    speed = -1;

  int index = 0;
  if(speed < 0)
  {
    index = 1;
    speed = -speed;
  }


  // Set duty[0] for forward, duty[1] for backward.
  duty[index] = (MOTOR_MAX-MOTOR_MIN)*speed + MOTOR_MIN;

  cout << "Level A: " << duty[0] << endl << "Level B:" << duty[1] << endl;

  // Set each motor with new duty cycles
  setMotor(MOTOR_A_PWM, duty[0], duty[1]);
  setMotor(MOTOR_B_PWM, duty[0], duty[1]);

  return 1;
}
/* Drive in a cirlce */
void GPIO::startCircle(void)
{
  this->enableHBridge();
  this->setTurn(+50);
  this->setSpeed(0.75);
}

void GPIO::stopCircle(void)
{
  this->disableHBridge();
  this->setTurn(0);
  this->setSpeed(0);
}

