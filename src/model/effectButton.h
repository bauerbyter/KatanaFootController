#ifndef EFFECT_BUTTON_H
#define EFFECT_BUTTON_H

#include "control.h"

class EffectButton : public Control
{

protected:
public:
    EffectButton(byte pin,
                  byte ledPosition,
                  unsigned long sendParameter,
                  unsigned long readParameter);
    virtual Led getLed(byte value) override;
    virtual bool changed() override;
    virtual void update(byte value) override;
};

#endif