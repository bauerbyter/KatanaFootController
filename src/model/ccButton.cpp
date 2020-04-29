#include "ccButton.h"

CCButton::CCButton(byte pin,
                   byte ledPosition,
                   unsigned long sendParameter,
                   unsigned long readParameter,
                   byte firstValue,
                   byte secondValue,
                   byte valueSize,
                   RgbColor color,
                   bool isLatch,
                   bool needsRead) : Control(pin,
                                             ledPosition,
                                             sendParameter,
                                             readParameter,
                                             firstValue,
                                             secondValue,
                                             valueSize,
                                             CC_BUTTON,
                                             needsRead)
{
    this->isLatch = isLatch;
    this->color = color;
}

bool CCButton::changed()
{
    button->read();
    if (isLatch)
    {
        if (button->wasPressed())
        {
            this->currentValue = this->secondValue;
            return true;
        }
        else if (button->wasReleased())
        {
            this->currentValue = this->firstValue;
            return true;
        }
    }
    else
    {
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
    }

    return false;
}

void CCButton::update(int value)
{
    if (value == secondValue)
    {
        currentValue = secondValue;
    }
    else
    {
        currentValue = firstValue;
    }
}

Led CCButton::getLed(byte value)
{

    if (value == secondValue)
    {
        return Led{this->ledPosition, this->color};
    }
    else
    {
        return Led{this->ledPosition, LED_OFF};
    }
}