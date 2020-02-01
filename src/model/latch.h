#ifndef LATCH_H
#define LATCH_H

#include <Arduino.h>
#include <JC_Button.h>
#include "config.h"
#include "switch.h"
#include "control.h"

class Latch : public Switch
{

protected:

public:
  Latch(Button *button, Command command, byte ledNumber);

  virtual bool changed() override;
  virtual void updateValue(byte value) override;
};
#endif