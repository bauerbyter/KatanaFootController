#include "bankButton.h"
#include "../sysex.h"

BankButton::BankButton(byte pin,
                       byte ledPosition) : Control(pin,
                                                   ledPosition,
                                                   PARA_PC,
                                                   PARA_PC,
                                                   0,
                                                   1,
                                                   1,
                                                   BANK_BUTTON,
                                                   false)
{
}

bool BankButton::changed()
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

void BankButton::update(int value)
{
    if (value > 4)
    {
        currentValue = secondValue;
    }
    else
    {
        currentValue = firstValue;
    }
}

Led BankButton::getLed(byte value)
{
    if (value > 4)
    {
        return Led{this->ledPosition, LED_PC_B_ON};
    }
    else
    {
        return Led{this->ledPosition, LED_OFF};
    }
}
