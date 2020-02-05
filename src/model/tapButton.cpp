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
                                                            true)
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

void TapButton::update(int value)
{
    this->currentValue = 60000 / value;
}

Led TapButton::getLed(byte value)
{
    return Led{this->ledPosition, LED_OFF};
}