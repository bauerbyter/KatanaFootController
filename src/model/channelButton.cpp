#include "channelButton.h"

ChannelButton::ChannelButton(byte pin,
                             byte ledPosition,
                             unsigned long sendParameter,
                             unsigned long readParameter,
                             byte firstValue,
                             byte secondValue) : Control(pin,
                                                       ledPosition,
                                                       sendParameter,
                                                       readParameter,
                                                       firstValue,
                                                       secondValue,
                                                       2,
                                                       CHANNEL_BUTTON,
                                                       false)
{
}



bool ChannelButton::changed()
{
    button->read();

    if (button->wasPressed())
    {
        return true;
    }
    return false;
}


void ChannelButton::update(int value)
{
    if (value == this->firstValue)
    {
        currentValue = firstValue;
    }
    else if (value == this->secondValue)
    {
        currentValue = secondValue;
    }
}

Led ChannelButton::getLed(byte value)
{
    if (value == firstValue)
    {
        return Led{this->ledPosition, LED_PC_A_ON};
    }
    else if (value == secondValue)
    {
        return Led{this->ledPosition, LED_PC_B_ON};
    }
    else
    {
        return Led{this->ledPosition, LED_OFF};
    }
}

void ChannelButton::bankUpdate(byte bank){
    if(bank == 0){
        this->currentValue = this->firstValue;
    }
    else
    {
        this->currentValue = this->secondValue;
    }
    
}