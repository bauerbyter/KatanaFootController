#ifndef CONTROL_H
#define CONTROL_H

#include <Arduino.h>
#include <JC_Button.h>
#include "MS3.h"

// Todo: maybe inherit Togglecontrol from Control, think also about expresssionpedal.
class Control
{

private:
  byte pin;
  unsigned long address;
  ToggleButton *button;
  MS3 *katana;
  void printStatus(int value);

public:
  Control(MS3 *katana, int pin, unsigned long address);
  void update();
};
#endif