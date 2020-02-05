#ifndef CC_BUTTON_H
#define CC_BUTTON_H

#include "control.h"

class CCButton : public Control
{

protected:
    bool isLatch;
    RgbColor color;

public:
    CCButton(byte pin,
             byte ledPosition,
             unsigned long sendParameter,
             unsigned long readParameter,
             byte firstValue,
             byte secondValue,
             byte valueSize,
             RgbColor color,
             bool isLatch = false,
             bool needsRead = false);
    virtual Led getLed(byte value) override;
    virtual bool changed() override;
    virtual void update(int value) override;
};

#endif