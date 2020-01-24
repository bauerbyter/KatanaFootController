#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include "MS3.h"

void setupKatana();
void updateKatana();
void reconnectKatana();
void handleIncomingData(unsigned long parameter, byte data);

#endif

/*
- Katana Maintainence
- Debug
- Togglebutton / normal button / Exp
- LEDs
- Also allow to send midi 
- Long Press

*/