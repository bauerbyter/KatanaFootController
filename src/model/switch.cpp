//---------------------------------------------------------------------
#include "switch.h"

Switch::Switch(byte pin,
               Command command,
               byte ledPosition,
               RgbColor color,
               bool inverted) : Control(pin, command, ledPosition, color, inverted)
{

}

bool Switch::changed()
{
  button->read();

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