#ifndef GPIO_DOUBLE_H
#define GPIO_DOUBLE_H

#include "I_Gpio.h"

class GpioDouble : public I_Gpio
{
public:
  GpioDouble() {};
  ~GpioDouble() {};

  virtual void Write(int channel) override;
};

#endif
