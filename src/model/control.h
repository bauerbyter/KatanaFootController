#ifndef CONTROL_H
#define CONTROL_H

#include <Arduino.h>
#include <JC_Button.h>
#include "config.h"

enum commandType
{
  PC,
  CC, //normal CC
  EFFECT, //this is for the Booster,Mod,FX,Delay,Reverb because they have 4 States= off, green, red, yellow
  EFFECT_UP, // Switch between green, red, yellow for Effects
  BANK, //Switch between Bank A/B
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
  Button *button;
  byte value;
  Command command;
  byte ledPosition;

public:
  Control(Button *button, Command command, byte ledNumber);

  byte getValue();
  Command getCommand();
  byte getLedPosition();
  bool addressMatch(unsigned long address);
  bool readAddressMatch(unsigned long address);
  //void printStatus();
  virtual bool changed() = 0;
  virtual void updateValue(byte value) = 0;
};
#endif






