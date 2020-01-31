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

//---------------------------------------------------------------------
//#include "control.h"

Switch::Switch(Button *button,
               Command command,
               byte ledPosition) : Control(button, command, ledPosition)
{
  //this->button = new Button(this->pin, DEBOUNCE_MS, INTERNAL_PULLUP, SWITCH_PULLUP);
}

bool Switch::changed()
{
  button->read();
  if (this->command.type == PC)
  {
    return button->wasPressed();
  }
  else if (this->command.type == CC || this->command.type == BANK)
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

Latch::Latch(Button *button,
             Command command,
             byte ledPosition) : Switch(button, command, ledPosition)
{
}

bool Latch::changed()
{
  button->read();

  if (button->wasPressed())
  {
    Serial.println("Latch was pressed");
    this->value = this->command.endValue;
    return true;
  }
  else if (button->wasReleased())
  {
    Serial.println("Latch was released");
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

Exp::Exp(Button *button,
         Command command,
         byte ledPosition) : Control(button, command, ledPosition)
{
}

bool Exp::changed()
{
  int incomingValue = 100; //TODO !!!!!
  //int incomingValue = analogRead(this->pin);
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
