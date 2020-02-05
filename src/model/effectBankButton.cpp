#include "effectBankButton.h"
#include "../sysex.h"

EffectBankButton::EffectBankButton(byte pin,
                       byte ledPosition) : Control(pin,
                                                   ledPosition,
                                                   0XFF,
                                                   0XFF,
                                                   0,
                                                   1,
                                                   1,
                                                   EFFECT_BANK_BUTTON,
                                                   false)
{
}

bool EffectBankButton::changed()
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

void EffectBankButton::update(int value)
{
    if (value == 1)
    {
        currentValue = secondValue;
    }
    else
    {
        currentValue = firstValue;
    }
}

Led EffectBankButton::getLed(byte value)
{
    if (value == 1)
    {
        return Led{this->ledPosition, LED_EFFECT_UP};
    }
    else
    {
        return Led{this->ledPosition, LED_OFF};
    }
}
