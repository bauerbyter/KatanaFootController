#include "control.h"
Control::Control(Button *button,
                 Command command,
                 byte ledPosition)
{
  this->button = button;
  this->command = command;
  this->ledPosition = ledPosition;
  this->value = command.startValue;
}

byte Control::getValue()
{
  return this->value;
}
Command Control::getCommand()
{
  return this->command;
}

byte Control::getLedPosition()
{
  return this->ledPosition;
}

bool Control::addressMatch(unsigned long address)
{
  return this->command.address == address;
}

bool Control::readAddressMatch(unsigned long address)
{
  return this->command.readAddress == address;
}

bool Control::changed()
{
  return false;
}
void Control::updateValue(byte value)
{
}
