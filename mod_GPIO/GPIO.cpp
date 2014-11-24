#include "GPIO.h"
#include <stdlib.h>
#include <iostream>
#include <BBBio_lib/BBBiolib.h>

using namespace std;

// Private instance for singleton
GPIO * GPIO::s_instance = NULL;

// Private constructor
GPIO::GPIO(void)
{
  // Initialize library
  iolib_init();
  // Set Batteries as output pins
  iolib_setdir(8, LED_BAT1, BBBIO_DIR_OUT);
  iolib_setdir(8, LED_BAT2, BBBIO_DIR_OUT);
  iolib_setdir(8, LED_BAT3, BBBIO_DIR_OUT);
  iolib_setdir(8, LED_BAT4, BBBIO_DIR_OUT);

  // H-Bridge Disable
  disableHBridge();
  iolib_setdir(8, HBRIDGE_EN, BBBIO_DIR_OUT);

  duty_wheels = TURN_CENTER;
  duty_camera = CAM_CENTER;

  // Configure PWM channels
  BBBIO_PWMSS_Setting(SERVO_PWM,    SERVO_FREQ,   0.25, 0.25);  // Clear settings
  BBBIO_PWMSS_Setting(MOTOR_A_PWM,  MOTOR_FREQ,   0.0,  0.0);   // Clear settings
  BBBIO_PWMSS_Setting(MOTOR_B_PWM,  MOTOR_FREQ,   0.0,  0.0);   // Clear settings

  // Setup ADC
  BBBIO_ADCTSC_module_ctrl(BBBIO_ADC_WORK_MODE_TIMER_INT, 160);
  // Setup the analog input sampling
  BBBIO_ADCTSC_channel_ctrl(BAT_SENSE, BBBIO_ADC_STEP_MODE_SW_CONTINUOUS, 0, 1, BBBIO_ADC_STEP_AVG_2, batBuff, 1);
}

// Disable the hbridge and free the IO library
int GPIO::deinitialize(void)
{
  // H-Bridge Disable
  disableHBridge();

  iolib_free();
  return 1;
}

// Set a single motor PWM value, used by setSpeed.
int GPIO::setMotor(int motor, float duty1, float duty2)
{
  // Update value
  BBBIO_PWMSS_Setting(motor, MOTOR_FREQ, duty1, duty2);
  // Re-enable after update
  BBBIO_ehrPWM_Enable(motor);
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
  pin_high(8, HBRIDGE_EN);
  return 1;
}

/* Disable the H-Bridge on the motor board */
int GPIO::disableHBridge(void)
{
  pin_low(8, HBRIDGE_EN);
  return 1;
}

/* setBattery - sets battery level by 4 LEDs on motor-board
 * Param: percent (uint8 from 0 to 100)
 */
int GPIO::setBatteryLEDs(float percent)
{
  // Reset to clear
  pin_low(8,LED_BAT1);
  pin_low(8,LED_BAT2);
  pin_low(8,LED_BAT3);
  pin_low(8,LED_BAT4);

  // Based on level light up LEDs
  if(percent > 5)
    pin_high(8,LED_BAT1);
  if(percent > 25)
    pin_high(8,LED_BAT2);
  if(percent > 50)
    pin_high(8,LED_BAT3);
  if(percent > 75)
    pin_high(8,LED_BAT4);

  return 1;
}

/* getBatteryLevel - gets the battery level as a percentage */
float GPIO::getBatteryLevel(void)
{
  // Enable the anaglog input
  BBBIO_ADCTSC_channel_enable(BAT_SENSE);
  // Make one reading from the ADC
  BBBIO_ADCTSC_work(1);
  float level = ((float)batBuff[0] / 27.3f);
  // Disable the channel input until next rea
  BBBIO_ADCTSC_channel_disable(BAT_SENSE);
  return level;
}

/* setTurn - sets the turning servo angle
 * param: float angle - [-1,1] range */
int GPIO::setTurn(float angle)
{
  float new_duty;

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

  // Update PWM hardware
  BBBIO_PWMSS_Setting(SERVO_PWM, SERVO_FREQ, duty_wheels, duty_camera);
  BBBIO_ehrPWM_Enable(SERVO_PWM);
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

  // Update PWM hardware
  BBBIO_PWMSS_Setting(SERVO_PWM, SERVO_FREQ, duty_wheels, duty_camera);
  BBBIO_ehrPWM_Enable(SERVO_PWM);
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
  //setMotor(MOTOR_A_PWM, duty[0], duty[1]);
  setMotor(MOTOR_B_PWM, duty[0], duty[1]);

  return 1;
}


/* Main for testing? */
int main(int argc, char** argv)
{
  int test;

  cout << "Initialize GPIO" << endl;
  GPIO *gpio = GPIO::instance();

  gpio->disableHBridge();
  cout << "Disabled";

  cin.ignore();

  gpio->enableHBridge();
  cout << "Enabled";

  cin.ignore();

  cout << "Cleanup" << endl;
  gpio->deinitialize();
  return 0;
}
