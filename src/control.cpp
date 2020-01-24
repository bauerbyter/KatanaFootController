#include "control.h"
Control::Control(MS3 *katana,
                 NeoPixelBrightnessBus<NeoRgbFeature, Neo800KbpsMethod> *strip,
                 int pin,
                 unsigned long address,
                 byte value,
                 bool toggle,
                 int ledNumber)
{
  this->pin = pin;
  this->address = address;
  this->button = new Button(this->pin, 25, false, false);
  this->katana = katana;
  this->status = value;
  this->toggle = toggle; //maybe create type: Bank up/down, PC&tap, CC, LATCH
  this->strip = strip;
}

void Control::updateValue(byte data)
{
  if (toggle) //I dont like this toggle flag, think about something else or better way...
  {
    if (data >= 1)
    {
      this->status = 1;
    }
    else
    {
      this->status = 0;
    }
  }
}

//Todo Cleanup
void Control::checkButton()
{
  button->read();

  if (button->wasPressed())
  {
    if (toggle)
    {
      if (status == 1)
      {
        status = 0;
      }
      else
      {
        status = 1;
      }
      //create Function which sends value and sets Led
      //add colors to config.h Red, green, yellow, OFF
      katana->write(address, status, 1);
    }
    else
    {
      katana->write(address, status, 2);
    }
    printStatus();
  }
}

void Control::printStatus()
{
  Serial.print("Button on Pin ");
  Serial.print(this->pin);
  Serial.print(" Send: ");
  Serial.print(this->address, HEX);
  Serial.print(" with ");
  Serial.println(status, HEX);
}
