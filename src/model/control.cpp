#include "control.h"

Control::Control(byte pin,
                 byte ledPosition,
                 unsigned long sendParameter,
                 unsigned long readParameter,
                 byte firstValue,
                 byte secondValue,
                 byte valueSize,
                 ClassType classType,
                 bool needsRead)
{
  this->pin = pin;
  this->ledPosition = ledPosition;
  this->sendParameter = sendParameter;
  this->readParameter = readParameter;
  this->firstValue = firstValue;
  this->secondValue = secondValue;
  this->valueSize = valueSize;
  this->classType = classType;
  this->needsRead = needsRead;

  this->currentValue = firstValue;
  this->button = new Button(pin, DEBOUNCE_MS);
}

bool Control::paramaterMatch(unsigned long parameter)
{
  if (parameter == this->readParameter)
  {
    return true;
  }
  return false;
}

byte Control::getValue()
{
  return this->currentValue;
}

byte Control::getValueSize()
{
  return this->valueSize;
}
unsigned long Control::getSendParameter()
{
  return this->sendParameter;
}
unsigned long Control::getReadParameter()
{
  return this->readParameter;
}
byte Control::getPin()
{
  return this->pin;
}

ClassType Control::getClassType()
{
  return this->classType;
}

bool Control::getNeedsRead()
{
  return this->needsRead;
}
