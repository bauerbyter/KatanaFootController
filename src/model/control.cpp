#include "control.h"
Control::Control(byte pin,
                 Command command,
                 byte ledPosition)
{
  this->pin = pin;
  this->button = new Button(pin, DEBOUNCE_MS);
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
byte Control::getPin()
{
  return this->pin;
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
