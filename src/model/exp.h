#ifndef EXP_H
#define EXP_H

#include <Arduino.h>
#include <JC_Button.h>
#include "config.h"
#include "switch.h"
#include "control.h"

class Exp : public Control
{

public:
  Exp(byte pin, Command command, byte ledNumber);

  bool changed() override;
  void updateValue(byte value) override;
};
#endif