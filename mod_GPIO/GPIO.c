#include "GPIO.h"
#include <stdio.h>
#include <stdlib.h>
#include <BBBio_lib/BBBiolib.h>

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

	BBBIO_ehrPWM_Enable(SERVO_PWM);   // Servo channel
	BBBIO_ehrPWM_Enable(MOTOR_A_PWM); // Motor A channel
	BBBIO_ehrPWM_Enable(MOTOR_B_PWM); // Motor B channel

  return 1;
}

/* Destroy IO lib, etc */
int deinitialize(void)
{
  // H-Bridge Enable/Disable
  pin_low(8, HBRIDGE_EN);

  // Disable PWM channels
	BBBIO_ehrPWM_Disable(SERVO_PWM);   // Servo channel
	BBBIO_ehrPWM_Disable(MOTOR_A_PWM); // Motor A channel
	BBBIO_ehrPWM_Disable(MOTOR_B_PWM); // Motor B channel

  iolib_free();
  return 0;
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
int setBatteryLEDs(uint8 percent)
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

// Both servos need to be stored so we don't mess one up
int setTurn(int16 angle)
{
	const float duty_wheels = 20.0f;
	const float duty_camera = 50.0f;

	BBBIO_PWMSS_Setting(SERVO_PWM, SERVO_FREQ, duty_wheels, duty_camera);
  return 1;
}

// Both servos need to be stored so we don't mess one up
int setCamera(int16 angle)
{
	const float duty_wheels = 20.0f;
	const float duty_camera = 50.0f;
  
	BBBIO_PWMSS_Setting(SERVO_PWM, SERVO_FREQ, duty_wheels, duty_camera);
  return 1;
}

//TODO: differential turning based on stored angle for turn servo
int setSpeed(uint8 speed)
{
	float duty = speed/100;
  
	BBBIO_PWMSS_Setting(MOTOR_A_PWM, MOTOR_FREQ, duty, 0.0);
	BBBIO_PWMSS_Setting(MOTOR_B_PWM, MOTOR_FREQ, duty, 0.0);
  return 1;
}
