#ifndef CONTROL_H
#define CONTROL_H

#include <queue>
#include "../GPIO/GPIO.h"
#include "../Hazard.h"
#include "../macros.h"

using namespace std;

class Control
{
  public:
    Control();
    ~Control();
    void update(Path path);
    float getTurn();
  private:
    GPIO *gpio;
    float turnPID(float angle);
    float speedPID(float turn);
    float turn;
    Path lpath;
    float P,I,D;
    float sum;
    queue<float> errors;
};

#endif /* CONTROL_H */
