#ifndef CONTROL_H
#define CONTROL_H

#include <Arduino.h>
#include <JC_Button.h>
#include <NeoPixelBrightnessBus.h>
#include "config.h"

enum commandType
{
  PC,
  CC, //normal CC
  EFFECT, //this is for the Booster,Mod,FX,Delay,Reverb because they have 4 States= off, green, red, yellow
  EFFECT_UP, // Switch between green, red, yellow for Effects
  BANK, //Switch between Bank A/B
  TAP,
};

struct Command
{
  commandType type;
  unsigned long address;
  unsigned long readAddress;
  byte startValue;
  byte endValue;
  byte valueSize;
};

class Control
{

protected:
  byte pin;
  Button *button;
  byte value;
  Command command;
  byte ledPosition;
  byte state;
  bool inverse;
  RgbColor color;

public:
  Control(byte pin, Command command, byte ledNumber, RgbColor color = DEFAULT_COLOR, bool inverted = false);
  byte getPin();
  byte getValue();
  byte getState();
  void setState(byte state);
  RgbColor getColor();
  bool isInverted();
  Command getCommand();
  byte getLedPosition();
  bool addressMatch(unsigned long address);
  bool readAddressMatch(unsigned long address);
  //void printStatus();
  virtual bool changed() = 0;
  virtual void updateValue(byte value) = 0;
};
#endif






