
#include "latch.h"

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