#ifndef I_GPIO_H
#define I_GPIO_H

class I_Gpio
{
public:
  I_Gpio() {};
  ~I_Gpio() {};

  virtual void Write(int channel) =0;
};

#endif
