#ifndef CONTROL_H
#define CONTROL_H

#include "../GPIO/GPIO.h"
#include "../Hazard.h"
#include "../macros.h"

class Control
{
  public:
    Control();
	~Control();
	void update(float angle);
  private:
  	GPIO *gpio;
};

#endif /* CONTROL_H */
