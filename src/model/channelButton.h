#ifndef CHANNEL_BUTTON_H
#define CHANNEL_BUTTON_H

#include "control.h"

class ChannelButton : public Control
{

protected:

public:
    ChannelButton(byte pin,
                  byte ledPosition,
                  unsigned long sendParameter,
                  unsigned long readParameter,
                  byte firstValue,
                  byte secondValue);
    virtual Led getLed(byte value) override;
    virtual bool changed() override;
    virtual void update(int value) override;
    void bankUpdate(byte bank);
};

#endif