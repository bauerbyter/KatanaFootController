#ifndef TAP_BUTTON_H
#define TAP_BUTTON_H

#include "control.h"

class TapButton : public Control
{

protected:
    unsigned long lastPressed;
    bool calculateValue();

public:
    TapButton(byte pin,
              byte ledPosition,
              unsigned long sendParameter,
              unsigned long readParameter);
    virtual Led getLed(byte value) override;
    virtual bool changed() override;
    virtual void update(byte value) override;
};

#endif