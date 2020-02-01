
#include "latch.h"

Latch::Latch(byte pin,
             Command command,
             byte ledPosition) : Switch(pin, command, ledPosition)
{
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