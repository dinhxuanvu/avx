#ifndef CONTROL_H
#define CONTROL_H

#include "../GPIO/GPIO.h"
#include "../macros.h"

class Control
{
  public:
    Control();
    ~Control();
    void update(float angle);
  private:
    GPIO *gpio;
    float turnPID(float angle);
    float speedPID(float turn);
};

#endif /* CONTROL_H */
