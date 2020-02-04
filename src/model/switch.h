#ifndef SWITCH_H
#define SWITCH_H

#include <Arduino.h>
#include <JC_Button.h>
#include "config.h"
#include "control.h"

class Switch : public Control
{


public:
  Switch(byte pin, Command command, byte ledNumber, RgbColor color = DEFAULT_COLOR, bool inverted = false);

  virtual bool changed() override;
  virtual void updateValue(byte value) override;
};
#endif