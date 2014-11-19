#include "GPIO.h"
#include <stdio.h>
#include <stdlib.h>
#include <BBBio_lib/BBBiolib.h>

/* Main for testing? */
int main(int argc, char** argv)
{
  int test;
  initialize();
  printf("Initialize\n");
  
  setCamera(atof(argv[1]));


  printf("Cleanup\n");
  deinitialize();
  return 0;
}

/* initialize - Starts up the configuration for GPIO
 * include pins and PWM
 */
int initialize(void)
{
  // Initialize library
  iolib_init();
  // Set Batteries as output pins
	iolib_setdir(8, LED_BAT1, BBBIO_DIR_OUT);
  iolib_setdir(8, LED_BAT2, BBBIO_DIR_OUT);
	iolib_setdir(8, LED_BAT3, BBBIO_DIR_OUT);
	iolib_setdir(8, LED_BAT4, BBBIO_DIR_OUT);

  // H-Bridge Enable/Disable
  pin_low(8, HBRIDGE_EN);
	iolib_setdir(8, HBRIDGE_EN, BBBIO_DIR_OUT);

  // Configure PWM channels
	BBBIO_PWMSS_Setting(SERVO_PWM,    SERVO_FREQ,   0.25, 0.25);  // Clear settings
	BBBIO_PWMSS_Setting(MOTOR_A_PWM,  MOTOR_FREQ,   0.0,  0.0);   // Clear settings
	BBBIO_PWMSS_Setting(MOTOR_B_PWM,  MOTOR_FREQ,   0.0,  0.0);   // Clear settings

	//BBBIO_ehrPWM_Enable(SERVO_PWM);   // Servo channel
	//BBBIO_ehrPWM_Enable(MOTOR_A_PWM); // Motor A channel
	//BBBIO_ehrPWM_Enable(MOTOR_B_PWM); // Motor B channel

  return 1;
}

/* Destroy IO lib, etc */
int deinitialize(void)
{
  // H-Bridge Enable/Disable
  pin_low(8, HBRIDGE_EN);

  iolib_free();
  return 0;
}

int disablePWM(void)
{
  // Disable PWM channels
	BBBIO_ehrPWM_Disable(SERVO_PWM);   // Servo channel
	BBBIO_ehrPWM_Disable(MOTOR_A_PWM); // Motor A channel
	BBBIO_ehrPWM_Disable(MOTOR_B_PWM); // Motor B channel
  return 1;
}

/* Enable the H-Bridge on the motor board */
int enableHBridge(void)
{
  pin_high(8, HBRIDGE_EN);
  return 1;
}

/* Disable the H-Bridge on the motor board */
int disableHBridge(void)
{
  pin_low(8, HBRIDGE_EN);
  return 1;
}

/* setBattery - sets battery level by 4 LEDs on motor-board
 * Param: percent (uint8 from 0 to 100)
 */
int setBatteryLEDs(unsigned int percent)
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

/* setTurn - sets the turning servo angle
 * param: float angle - [-1,1] range */
int setTurn(float angle)
{
	float duty_wheels;

  if(angle > 1)
    angle = 1;
  if(angle < -1)
    angle = -1;

  if(angle > 0)
  {
    duty_wheels = (TURN_RIGHT-TURN_CENTER)*angle + TURN_CENTER;
  } else
  {
    duty_wheels = (TURN_CENTER-TURN_LEFT)*angle + TURN_CENTER;
  }

	float duty_camera = CAM_CENTER;

	BBBIO_PWMSS_Setting(SERVO_PWM, SERVO_FREQ, duty_wheels, duty_camera);
  BBBIO_ehrPWM_Enable(SERVO_PWM);
  return 1;
}

/* setCamera - sets the camera angle
 * param: angle - [-90,90] degrees from center */
int setCamera(float angle)
{
	float duty_camera;

  angle = angle/90;

  if(angle > 1)
    angle = 1;
  if(angle < -1)
    angle = -1;

  if(angle > 0)
    duty_camera = (CAM_RIGHT-CAM_CENTER)*angle + CAM_CENTER;
  else
    duty_camera = (CAM_CENTER-CAM_LEFT)*angle + CAM_CENTER;

	float duty_wheels = TURN_CENTER;

	BBBIO_PWMSS_Setting(SERVO_PWM, SERVO_FREQ, duty_wheels, duty_camera);
  BBBIO_ehrPWM_Enable(SERVO_PWM);
  return 1;

}

//TODO: differential turning based on stored angle for turn servo
int setSpeed(unsigned int speed)
{
	float duty = (float)speed;
  
	BBBIO_PWMSS_Setting(MOTOR_A_PWM, MOTOR_FREQ, duty, duty);
	BBBIO_PWMSS_Setting(MOTOR_B_PWM, MOTOR_FREQ, duty, duty);

  BBBIO_ehrPWM_Enable(MOTOR_A_PWM);
  BBBIO_ehrPWM_Enable(MOTOR_B_PWM);
  printf("%0.2f percent of motor\n",duty);
  return 1;
}
