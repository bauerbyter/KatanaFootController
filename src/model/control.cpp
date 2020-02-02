#include "control.h"

Control::Control(byte pin,
                 byte ledPosition,
                 Command command)
{
  this->pin = pin;
  this->ledPosition = ledPosition;
  this->command = command;
  this->value = command.value;

  this->button = new Button(pin, DEBOUNCE_MS);
}

bool Control::changed()
{
  button->read();

  if (button->wasPressed())
  {
    return true;
  }
  return false;
}

Command Control::getCommand(){

}