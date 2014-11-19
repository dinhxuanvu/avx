#include "GPIO.h"
#include <stdio.h>
#include <stdlib.h>
#include <BBBio_lib/BBBiolib.h>

/* Main for testing? */
int main(int argc, char** argv)
{
  int test;
  printf("GPIO Module Testing\n\n");
  if(argc < 3)
  {
    printf("Usage: %s (1|2|3) value\n", argv[0]);
    printf("\t 1 - Battery level \n");
    printf("\t 2 - Turning servo angle \n");
    printf("\t 3 - Motor speed value\n");
    return -1;
  }
  
  test = atoi(argv[1]);

  printf("Initializing IO lib\n");
  initialize();

  if(test == 1)
  {
    printf("Battery level at %s\n",argv[2]);
    setBatteryLEDs(atoi(argv[2]));
  }else if(test == 2)
  {
    printf("Tuning servo 1 to %s\n",argv[2]);
    setTurn(atoi(argv[2]));
  }else if(test == 3)
  {
    enableHBridge();
    printf("Motor speed to %s \n",argv[2]);
    setSpeed(atoi(argv[2]));
  }else{
    printf("Invalid value\n");
  }

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

// Both servos need to be stored so we don't mess one up
int setTurn(signed int angle)
{
	float duty_wheels = (float)angle;
	float duty_camera = 50.0f;

	BBBIO_PWMSS_Setting(SERVO_PWM, SERVO_FREQ, duty_wheels, duty_camera);
  return 1;
}

// Both servos need to be stored so we don't mess one up
int setCamera(signed int angle)
{
	const float duty_wheels = 10.0f;
	const float duty_camera = angle/100;
  
	BBBIO_PWMSS_Setting(SERVO_PWM, SERVO_FREQ, duty_wheels, duty_camera);
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
