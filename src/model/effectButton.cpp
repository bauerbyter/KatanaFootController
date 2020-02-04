#include "EffectButton.h"

EffectButton::EffectButton(byte pin,
                           byte ledPosition,
                           unsigned long sendParameter,
                           unsigned long readParameter,
                           unsigned long typeParameter) : Control(pin,
                                                                  ledPosition,
                                                                  sendParameter,
                                                                  readParameter,
                                                                  0,
                                                                  1,
                                                                  1,
                                                                  EFFECT_BUTTON,
                                                                  false)
{
    this->typeParameter = typeParameter;
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
    state = value;

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

byte EffectButton::getType()
{

    if (state >= 3)
    {
        return 0;
    }
    else if (state == 0)
    {
        return 4; // 4 = nothing should happen ..
    }
    else
    {
        return this->state;
    }
}
byte EffectButton::getState()
{
    return this->state;
}

unsigned long EffectButton::getTypeParameter()
{
    return this->typeParameter;
}