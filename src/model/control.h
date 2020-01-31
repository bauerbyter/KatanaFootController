#ifndef CONTROL_H
#define CONTROL_H

#include <Arduino.h>
#include <JC_Button.h>
#include "config.h"

enum commandType
{
  PC,
  CC,
  BANK,
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

#ifndef SWITCH_H
#define SWITCH_H

#include <Arduino.h>
#include <JC_Button.h>

class Switch : public Control
{


public:
  Switch(Button *button, Command command, byte ledNumber);

  virtual bool changed() override;
  virtual void updateValue(byte value) override;
};
#endif

#ifndef LATCH_H
#define LATCH_H

#include <Arduino.h>
#include <JC_Button.h>

class Latch : public Switch
{

protected:

public:
  Latch(Button *button, Command command, byte ledNumber);

  virtual bool changed() override;
  virtual void updateValue(byte value) override;
};
#endif


#ifndef EXP_H
#define EXP_H

#include <Arduino.h>
#include <JC_Button.h>

class Exp : public Control
{

public:
  Exp(Button *button, Command command, byte ledNumber);

  bool changed() override;
  void updateValue(byte value) override;
};
#endif