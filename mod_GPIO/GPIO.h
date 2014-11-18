#ifndef GPIO_H_
#define GPIO_H_

#ifdef __cplusplus
extern "C" {
#endif

// Battery LEDs on P8
#define LED_BAT1    18
#define LED_BAT2    16
#define LED_BAT3    14
#define LED_BAT4    12

#define HBRIDGE_EN  11    // P8
#define BAT_SENSE   AIN1  // P9

#define SERVO_PWM       BBBIO_PWMSS2 // Channel A = turn, Channel B = Camera
#define SERVO_FREQ      50           // 50 Hz

#define MOTOR_A_PWM     BBBIO_PWMSS0 // Both channels A,B
#define MOTOR_B_PWM     BBBIO_PWMSS1 // Both channels A,B
#define MOTOR_FREQ      20000        // 20Khz

/* initialize - Starts up the configuration for GPIO
 * include pins and PWM
 */
int initialize(void);

/* Destroy IO lib, etc */
int deinitialize(void);

/* Enable the H-Bridge on the motor board */
int enableHBridge(void);

/* Disable the H-Bridge on the motor board */
int disableHBridge(void);

/* setBattery - sets battery level by 4 LEDs on motor-board
 * Param: percent (uint8 from 0 to 100)
 */
int setBatteryLEDs(uint8 percent);

// Both servos need to be stored so we don't mess one up
int setTurn(int16 angle);

// Both servos need to be stored so we don't mess one up
int setCamera(int16 angle);

//TODO: differential turning based on stored angle for turn servo
int setSpeed(uint8 speed);

#ifdef __cplusplus
  }
#endif

#endif /* GPIO_H_ */