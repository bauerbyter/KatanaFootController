#ifndef CONTROL_H
#define CONTROL_H

#include <Arduino.h>
#include <JC_Button.h>
#include "MS3.h"
#include "NeoPixelBrightnessBus.h"

// Todo: maybe inherit Togglecontrol from Control, think also about expresssionpedal.
class Control
{

private:
  byte pin;
  byte status;
  bool toggle;
  unsigned long address;
  Button *button;
  MS3 *katana;
  NeoPixelBrightnessBus<NeoRgbFeature, Neo800KbpsMethod> *strip;
  void printStatus();

public:
  Control(MS3 *katana,NeoPixelBrightnessBus<NeoRgbFeature, Neo800KbpsMethod> *strip, int pin, unsigned long address, byte value, bool toggle, int ledNumber);
  void checkButton();
  void updateValue(byte value);
  void addressMatch();
};
#endif