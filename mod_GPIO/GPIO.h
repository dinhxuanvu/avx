#ifndef GPIO_H_
#define GPIO_H_

// Battery LEDs on P8
#define LED_BAT1    18
#define LED_BAT2    16
#define LED_BAT3    14
#define LED_BAT4    12

#define HBRIDGE_EN  11    // P8
#define BAT_SENSE   AIN1  // P9

#define SERVO_PWM       BBBIO_PWMSS2  // Channel A = turn, Channel B = Camera
#define SERVO_FREQ      50.0f           // 50 Hz

#define TURN_LEFT       2.7f
#define TURN_CENTER     4.425f
#define TURN_RIGHT      6.5f

#define CAM_LEFT        2.7f
#define CAM_CENTER      4.425f
#define CAM_RIGHT       6.5f

#define MOTOR_A_PWM     BBBIO_PWMSS0  // Both channels A,B
#define MOTOR_B_PWM     BBBIO_PWMSS1  // Both channels A,B
#define MOTOR_FREQ      15000.0f        // 20Khz

#define MOTOR_MIN		0.0f
#define MOTOR_MAX		100.0f

  class GPIO
  {
    private:
      float duty_wheels, duty_camera;

      static GPIO *s_instance;

      GPIO(void);

      int setMotor(int, float, float);
    public:
      static GPIO *instance();

      int deinitialize(void);
    
      /* Enable the H-Bridge on the motor board */
      int enableHBridge(void);

      /* Disable the H-Bridge on the motor board */
      int disableHBridge(void);

      /* setBattery - sets battery level by 4 LEDs on motor-board
       * Param: percent (uint8 from 0 to 100)
       */
      int setBatteryLEDs(float);

      /* setTurn - sets the turning servo angle
       * param: float angle - [-1,1] range */
      int setTurn(float);

      /* setCamera - sets the camera angle
       * param: angle - [-90,90] degrees from center */
      int setCamera(float);

      /* setSpeed - sets motor speed based on value.
       * param: speed in [-1,1], where < 0 is reverse, and > 0 is forward
       */
      int setSpeed(float);
  };

#endif /* GPIO_H_ */
