#include "control.h"
Control::Control(int pin,
                 Command command,
                 byte ledPosition)
{
  this->pin = pin;
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

//---------------------------------------------------------------------
//#include "control.h"

Switch::Switch(int pin,
               Command command,
               byte ledPosition) : Control(pin, command, ledPosition)
{
  this->button = new Button(this->pin, DEBOUNCE_MS, INTERNAL_PULLUP, SWITCH_PULLUP);
}

bool Switch::changed()
{
  button->read();
  if (this->command.type == PC)
  {
    return button->wasPressed();
  }
  else
  {
    if (button->wasPressed())
    {
      if (value == this->command.endValue)
      {
        this->value = this->command.startValue;
      }
      else
      {
        this->value = this->command.endValue;
      }
      return true;
    }
  }
  return false;
}
void Switch::updateValue(byte value)
{
  if (this->command.type == PC)
  {
    return;
  }

  if (value == this->command.endValue)
  {
    this->value = this->command.endValue;
  }
  else if (value == this->command.startValue)
  {
    this->value = this->command.startValue;
  }
}

//---------------------------------------------------------------------
//#include "control.h"

Latch::Latch(int pin,
             Command command,
             byte ledPosition) : Switch(pin, command, ledPosition)
{
  this->button = new Button(this->pin, DEBOUNCE_MS, INTERNAL_PULLUP, SWITCH_PULLUP);
}

bool Latch::changed()
{
  button->read();

  if (button->wasPressed())
  {
    this->value = this->command.endValue;
    return true;
  }
  else if (button->wasReleased())
  {
    this->value = this->command.startValue;
    return true;
  }
  return false;
}

void Latch::updateValue(byte value)
{
  if (value == this->command.endValue)
  {
    this->value = this->command.endValue;
  }
  else if (value == this->command.startValue)
  {
    this->value = this->command.startValue;
  }
}

//---------------------------------------------------------------------
//#include "control.h"

Exp::Exp(int pin,
         Command command,
         byte ledPosition) : Control(pin, command, ledPosition)
{
}

bool Exp::changed()
{
  int incomingValue = analogRead(this->pin);
  incomingValue = map(incomingValue, 0, 1023, 0, 100);
  incomingValue = constrain(incomingValue, 0, 100);

  if (incomingValue >= this->value - EXP_THRESHOLD && incomingValue <= this->value + EXP_THRESHOLD)
  {
    return false;
  }
  else
  {
    this->value = incomingValue;
    return true;
  }
}
void Exp::updateValue(byte value)
{
  if (value < 100)
  {
    this->value = value;
  }
}
