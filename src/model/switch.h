#ifndef SWITCH_H
#define SWITCH_H

#include <Arduino.h>
#include <JC_Button.h>
#include "config.h"
#include "control.h"

class Switch : public Control
{


public:
  Switch(Button *button, Command command, byte ledNumber);

  virtual bool changed() override;
  virtual void updateValue(byte value) override;
};
#endif