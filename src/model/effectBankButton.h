#ifndef EFFECT_BANK_BUTTON_H
#define EFFECT_BANK_BUTTON_H

#include "control.h"

class EffectBankButton : public Control
{

protected:
public:
    EffectBankButton(byte pin,
               byte ledPosition);
    virtual Led getLed(byte value) override;
    virtual bool changed() override;
    virtual void update(int value) override;
};

#endif