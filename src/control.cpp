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
      Serial.println("Button on");
      katana->write(address, 1,1);
    }
    else
    {
      Serial.println("Button off");
      katana->write(address, 0,1);
    }
  }
}

