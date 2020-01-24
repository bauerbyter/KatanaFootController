#include <Arduino.h>
#include <main.h>
#include <sysex.h>
#include <control.h>
//#define MS3_DEBUG_MODE
#include "MS3.h"
#include <MIDI.h>
#include <JC_Button.h>

MS3 katana;

#define CONTROL_SIZE 3
//Todo: for configuration - read out of EEPROM?
Control controller[CONTROL_SIZE] = {
    Control(&katana, 2, BOOSTER),
    Control(&katana, 3, MOD),
    Control(&katana, 4, FX),
};

void setup()
{
  Serial.begin(115200);

  setupKatana();
}

void loop()
{
  for (unsigned int i = 0; i < CONTROL_SIZE; i++)
  {
    controller[i].update();
  }

  // Todo Build a function for reconnection and stuff :-)
  unsigned long parameter = 0;
  byte data = 0;
  katana.update(parameter, data);
}

void setupKatana()
{
  if (!katana.begin())
  {
    Serial.println(F("*** USB / Katana init error! ***"));
    while (true)
      ;
  }

  unsigned long parameter = 0;
  byte data = 0;
  byte editMode = 0;
  while (editMode == 0)
  {
    switch (katana.update(parameter, data))
    {
    case MS3_NOT_READY:
      Serial.println(F("Katana OFFLINE !"));
      Serial.println();
      delay(100);
      break;
    case MS3_READY:
      katana.setEditorMode();
      katana.read(P_PATCH, 0x02);
      editMode = 1;
      Serial.println(F("Ready!"));
      break;
    }
  }
}