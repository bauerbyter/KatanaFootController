#include "control.h"
Control::Control(MS3 *katana, int pin, unsigned long address)
{
  this->pin = pin;
  this->address = address;
  this->button = new ToggleButton(this->pin, false, 25, false, false);
  this->katana = katana;
}

void Control::update()
{
  button->read();

  if (button->changed())
  {
    if (button->toggleState())
    {
      katana->write(address, 1, 1);
      this->printStatus(1);
    }
    else
    {
      katana->write(address, 0, 1);
      this->printStatus(0);
    }
  }
}

void Control::printStatus(int value)
{
  Serial.print("Button on Pin ");
  Serial.print(this->pin);
  Serial.print(" Send: ");
  Serial.print(this->address, HEX);
  Serial.print(" with ");
  Serial.println(value, HEX);
}
