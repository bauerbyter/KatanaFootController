#ifndef EFFECT_BUTTON_H
#define EFFECT_BUTTON_H

#include "control.h"

class EffectButton : public Control
{

protected:
byte state;
unsigned long typeParameter;

public:
    EffectButton(byte pin,
                  byte ledPosition,
                  unsigned long sendParameter,
                  unsigned long readParameter,
                  unsigned long typeParameter);
    virtual Led getLed(byte value) override;
    virtual bool changed() override;
    virtual void update(int value) override;
    byte getState();
    byte getType();
    unsigned long getTypeParameter();
};

#endif