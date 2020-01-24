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

  updateKatana();
}

void reconnectKatana(void) {
  unsigned long test = 0;
  byte dataTest = 0;
  Serial.println();
  Serial.println(F("Waiting..."));
  Serial.println();
  katana.begin();
  delay(100);
  switch (katana.update(test, dataTest)) {
    case MS3_READY:
      Serial.println(F("############ Back again Baby!"));
      Serial.println();
      katana.setEditorMode();
      katana.read(PC, 0x02);
      break;
  }
}

void updateKatana(){
  unsigned long parameter = 0;
  uint8_t data = 0;
  bool changed = false;

  // Check for incoming data or send a queued item.
  switch (katana.update(parameter, data))
  {
  case MS3_NOT_READY:
    Serial.println(F("Katana OFF-LINE !"));
    Serial.println();
    reconnectKatana();
    break;

  // Fetch the current active patch on the MS-3.
  case MS3_READY:
    katana.setEditorMode();
    katana.read(P_PATCH, 0x02);
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
    if (changed) //Todo 
    {
      Serial.println("IDLE");
      changed = false;
    }
    break;
  }
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