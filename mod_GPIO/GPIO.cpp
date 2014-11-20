#include "GPIO.h"
#include <stdlib.h>
#include <iostream>
#include <BBBio_lib/BBBiolib.h>

using namespace std;


      GPIO * GPIO::s_instance = NULL;
      // Constructor not public
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
      }
      
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
        float duty [2] = { 0.0, 0.0 };
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
      
        // Set each motor with new duty cycles
        setMotor(MOTOR_A_PWM, duty[0], duty[1]);
        setMotor(MOTOR_B_PWM, duty[0], duty[1]);

        return 1;
      }


/* Main for testing? */
int main(int argc, char** argv)
{
  int test;

  cout << "Initialize GPIO" << endl;
  GPIO *gpio = GPIO::instance();

  gpio->setCamera(atof(argv[1]));

  cout << "Cleanup" << endl;
  gpio->deinitialize();
  return 0;
}
