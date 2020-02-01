#include "exp.h"

Exp::Exp(byte pin,
         Command command,
         byte ledPosition) : Control(pin, command, ledPosition)
{
}

bool Exp::changed()
{
  int incomingValue = 100; //TODO !!!!!
  //int incomingValue = analogRead(this->pin);
  incomingValue = map(incomingValue, 0, 1023, 0, 100);
  incomingValue = constrain(incomingValue, 0, 100);

  if (incomingValue >= this->value - EXP_THRESHOLD && incomingValue <= this->value + EXP_THRESHOLD)
  {
    return false;
  }
  else
  {
    this->value = incomingValue;
    return true;
  }
}
void Exp::updateValue(byte value)
{
  if (value < 100)
  {
    this->value = value;
  }
}
