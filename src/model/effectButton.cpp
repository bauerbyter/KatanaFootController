#include "EffectButton.h"

EffectButton::EffectButton(byte pin,
                           byte ledPosition,
                           unsigned long sendParameter,
                           unsigned long readParameter) : Control(pin,
                                                                  ledPosition,
                                                                  sendParameter,
                                                                  readParameter,
                                                                  0,
                                                                  1,
                                                                  1,
                                                                  EFFECT_BUTTON,
                                                                  false)
{
}

bool EffectButton::changed()
{
    button->read();

    if (button->wasPressed())
    {
        if (currentValue == this->firstValue)
        {
            this->currentValue = this->secondValue;
        }
        else if (currentValue == this->secondValue)
        {
            this->currentValue = this->firstValue;
        }
        return true;
    }
    return false;
}

void EffectButton::update(byte value)
{
    if (value == 0)
    {
        currentValue = firstValue;
    }
    else
    {
        currentValue = secondValue;
    }
}

Led EffectButton::getLed(byte value)
{
  
    if (value == 1)
    {
        return Led{this->ledPosition, LED_EFFECT_GREEN};
    }
    else if (value == 2)
    {
        return Led{this->ledPosition, LED_EFFECT_RED};
    }
    else if (value == 3)
    {
        return Led{this->ledPosition, LED_EFFECT_YELLOW};
    }
    else
    {
        return Led{this->ledPosition, LED_OFF};
    }
}