#ifndef CONTROL_H
#define CONTROL_H

#include <Arduino.h>
#include <JC_Button.h>
#include <NeoPixelBrightnessBus.h>
#include "config.h"

/*
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

public:
  Control(byte pin, Command command, byte ledNumber);
  byte getPin();
  byte getValue();
  Command getCommand();
  byte getLedPosition();
  bool addressMatch(unsigned long address);
  bool readAddressMatch(unsigned long address);
  //void printStatus();
  virtual bool changed() = 0;
  virtual void updateValue(byte value) = 0;
};*/

struct Command
{
  unsigned long sendParameter;
  unsigned long readParameter;
  int valueSize;
  int value;
  bool needsRead;
};

struct Led{
  byte ledPosition;
  RgbColor color;
};

class Control
{

private:
  byte pin;
  byte ledPosition;
  Button *button;
  byte value;
  byte type;
  Command command;

public:
  Control(byte pin, byte ledPosition, Command command);
  bool changed();
  Command getCommand();
  Led getLed();
  void update(unsigned long paramater, byte value);
};
#endif
