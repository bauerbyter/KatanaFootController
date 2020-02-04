#include "tapButton.h"

TapButton::TapButton(byte pin,
                     byte ledPosition,
                     unsigned long sendParameter,
                     unsigned long readParameter) : Control(pin,
                                                            ledPosition,
                                                            sendParameter,
                                                            readParameter,
                                                            0,
                                                            1,
                                                            1,
                                                            TAP_BUTTON,
                                                            false)
{
}

bool TapButton::changed()
{
    button->read();

    if (button->wasPressed())
    {
        return calculateValue();
    }
    return false;
}

bool TapButton::calculateValue()
{
    if ((millis() - lastPressed) > 2000)
    {
        lastPressed = millis();
        this->currentValue = 0;
        return false;
    }
    unsigned long tempoMillis = millis() - lastPressed;
    lastPressed = millis();
    unsigned long tempo = 60000 / tempoMillis;

    if (tempo > 400)
    {
        tempo = 400;
    }
    if (tempo < 30)
    {
        tempo = 30;
    }
    this->currentValue = tempo;

    return true;
}


void TapButton::update(byte value)
{
    /*if (value == 0)
    {
        currentValue = firstValue;
    }
    else
    {
        currentValue = secondValue;
    }*/
}

Led TapButton::getLed(byte value)
{

    /*if (value == 1)
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
    {*/
        return Led{this->ledPosition, LED_OFF};
    //}
}