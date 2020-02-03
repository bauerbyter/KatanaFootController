#include "control.h"

Control::Control(byte pin,
                 byte ledPosition,
                 unsigned long sendParameter,
                 unsigned long readParameter,
                 byte firstValue,
                 byte secondValue,
                 byte valueSize,
                 bool needsRead = false)
{
  this->pin = pin;
  this->ledPosition = ledPosition;
  this->sendParameter = sendParameter;
  this->readParameter = readParameter;
  this->firstValue = firstValue;
  this->secondValue = secondValue;
  this->valueSize = valueSize;
  this->needsRead = needsRead;

  //this->currentValue = command.value;

  this->button = new Button(pin, DEBOUNCE_MS);
}

bool Control::changed()
{
  return false;
}

bool Control::paramaterMatch(unsigned long parameter)
{
  if (parameter == this->readParameter)
  {
    return true;
  }
  return false;
}

/*Led Control::getLed()
{
  return Led{0, LED}
}*/
//##############################//
//------------------------------//

ChannelButton::ChannelButton(byte pin,
                             byte ledPosition,
                             unsigned long sendParameter,
                             unsigned long readParameter,
                             byte firstValue,
                             byte secondValue,
                             byte valueSize,
                             bool needsRead = false,
                             bool bankAB = true) : Control(pin,
                                                           ledPosition,
                                                           sendParameter,
                                                           readParameter,
                                                           firstValue,
                                                           secondValue,
                                                           valueSize,
                                                           needsRead)
{
  this->bankAB = bankAB;
}

bool ChannelButton::changed()
{
  button->read();

  if (button->wasPressed())
  {
    return true;
  }
  return false;
}


void ChannelButton::BankSwitch(byte bank)
{
  if (bank == 1)
  {
    state = 1;
  }
  else
  {
    state = 0;
  }
}

void ChannelButton::update(byte value)
{
  if (value == this->firstValue)
  {
    state = 1;
  }
  else if (bankAB && value == this->secondValue)
  {
    state = 2;
  }
  else
  {
    state = 0;
  }
}

Led ChannelButton::getLed()
{
  if (state == 1)
  {
    return Led{this->ledPosition, PC_A_ON};
  }
  else if (state == 2)
  {
    return Led{this->ledPosition, PC_B_ON};
  }
  else
  {
    return Led{this->ledPosition, LED_OFF};
  }
}