#ifndef CONTROL_H
#define CONTROL_H

#include <Arduino.h>
#include <JC_Button.h>
#include <NeoPixelBrightnessBus.h>
#include "config.h"

enum ClassType //.. no typeid in arduino -.-
{
  CONTROL,
  CHANNEL_BUTTON,
  EFFECT_BUTTON,
  CC_BUTTON,
  TAP_BUTTON,
  BANK_BUTTON,
  EFFECT_BANK_BUTTON,
};

struct Led
{
  byte ledPosition;
  RgbColor color;
};

class Control
{

protected:
  byte pin;
  byte ledPosition;
  Button *button;
  byte currentValue;
  unsigned long sendParameter;
  unsigned long readParameter;
  byte valueSize;
  byte firstValue;
  byte secondValue;
  bool needsRead;
  ClassType classType;

public:
  Control(byte pin,
          byte ledPosition,
          unsigned long sendParameter,
          unsigned long readParameter,
          byte firstValue,
          byte secondValue,
          byte valueSize,
          ClassType classType,
          bool needsRead = false);
  bool paramaterMatch(unsigned long parameter);
  byte getValueSize();
  byte getPin();
  unsigned long getSendParameter();
  unsigned long getReadParameter();
  byte getValue();
  ClassType getClassType();
  bool getNeedsRead();

  virtual Led getLed(byte value) = 0;
  virtual bool changed() = 0;
  virtual void update(byte value) = 0;
};

#endif
