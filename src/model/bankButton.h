#ifndef BANK_BUTTON_H
#define BANK_BUTTON_H

#include "control.h"

class BankButton : public Control
{

protected:
public:
    BankButton(byte pin,
               byte ledPosition);
    virtual Led getLed(byte value) override;
    virtual bool changed() override;
    virtual void update(byte value) override;
};

#endif