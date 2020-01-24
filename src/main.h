#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include "MS3.h"

void setupKatana();
void updateKatana();
void reconnectKatana();

#endif

/*
- Katana Maintainence
- Debug
- Togglebutton / normal button / Exp
- LEDs
- Also allow to send midi 
- Long Press




unsigned long parameter = 0;
  uint8_t data = 0;
  bool changed = false;

  // Check for incoming data or send a queued item.
  switch (Katana.update(parameter, data))
  {
  case MS3_NOT_READY:
    Serial.println(F("Katana OFF-LINE !"));
    Serial.println();
    setEdit();
    break;

  // Fetch the current active patch on the MS-3.
  case MS3_READY:
    Katana.setEditorMode();
    Katana.read(P_PATCH, 0x02);
    break;

  // Parse the incoming data.
  case MS3_DATA_RECEIVED:
    changed = true;
    Serial.print("Receive: ");
    Serial.print(parameter);
    Serial.print(" - ");
    Serial.println(data);

    break;

  // If all communication is done, print the result.
  case MS3_IDLE:
    if (changed)
    {
      Serial.println("IDLE");
      changed = false;
    }
    break;
  }
*/