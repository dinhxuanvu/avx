#ifndef CONTROL_H
#define CONTROL_H

#include <queue>
#include "../GPIO/GPIO.h"
#include "../macros.h"

using namespace std;

class Control
{
  public:
    Control();
    ~Control();
    void update(float angle);
    float getTurn();
  private:
    GPIO *gpio;
    float turnPID(float angle);
    float speedPID(float turn);
    float turn;
    float P,I,D;
    float sum;
    queue<float> errors;
};

#endif /* CONTROL_H */
