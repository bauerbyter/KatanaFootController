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
  byte state;
  unsigned long sendParameter;
  unsigned long readParameter;
  bool valueSize;
  bool firstValue;
  bool secondValue;
  bool needsRead;

public:
  Control(byte pin,
          byte ledPosition,
          unsigned long sendParameter,
          unsigned long readParameter,
          byte firstValue,
          byte secondValue,
          byte valueSize,
          bool needsRead = false);
  bool paramaterMatch(unsigned long parameter);
  virtual bool changed();
  virtual void update(byte value);
  virtual Led getLed() = 0;
};

class ChannelButton : public Control
{
private:
  bool bankAB;

public:
  ChannelButton(byte pin,
                byte ledPosition,
                unsigned long sendParameter,
                unsigned long readParameter,
                byte firstValue,
                byte secondValue,
                byte valueSize,
                bool needsRead = false,
                bool bankAB = true);
  virtual bool changed() override;
  virtual void update(byte value) override;
  virtual Led getLed() override;
};

#endif
