#include <Arduino.h>
#include <main.h>
#include <sysex.h>
#include <control.h>
//#define MS3_DEBUG_MODE
#include "MS3.h"
#include <MIDI.h>
#include <JC_Button.h>
#include <NeoPixelBrightnessBus.h>

MS3 katana;
NeoPixelBrightnessBus<NeoRgbFeature, Neo800KbpsMethod> strip(5, 6);

#define CONTROL_SIZE 6
//Todo: for configuration - read out of EEPROM?
Control controller[CONTROL_SIZE] = {
    Control(&katana, &strip, 2, BOOSTER, 0, true, 0),
    Control(&katana, &strip, 3, MOD, 0, true, 1),
    Control(&katana, &strip, 4, FX, 0, true, 2),
    Control(&katana, &strip, A0, PC, 1, false, 3),
    Control(&katana, &strip, A1, PC, 2, false, 4),
    Control(&katana, &strip, A2, PC, 3, false, 5),
};

void setup()
{
  Serial.begin(115200);

  setupKatana();

  strip.Begin();
  strip.Show();
  strip.SetBrightness(50);
}

void loop()
{
  for (unsigned int i = 0; i < CONTROL_SIZE; i++)
  {
    controller[i].checkButton();
  }

  updateKatana();
}

void reconnectKatana(void)
{
  unsigned long test = 0;
  byte dataTest = 0;
  Serial.println();
  Serial.println(F("Waiting..."));
  Serial.println();
  katana.begin();
  delay(100);
  switch (katana.update(test, dataTest))
  {
  case MS3_READY:
    Serial.println(F("############ Back again Baby!"));
    Serial.println();
    katana.setEditorMode();
    katana.read(PC, 0x02);
    break;
  }
}

void updateKatana()
{
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
    handleIncomingData(parameter, data);
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


void handleIncomingData(unsigned long parameter, byte data)
{
  Serial.print("Receive: ");
  Serial.print(parameter, HEX);
  Serial.print(" - ");
  Serial.println(data, HEX);
  //Just for testing right now -> will be removed and moved into control.cpp
  if (parameter == BOOSTER_LED)
  {
    if (data == 0)
    {
      strip.SetPixelColor(0, RgbColor(0, 0, 0));
    }
    else if (data == 1)
    {
      strip.SetPixelColor(0, RgbColor(0, 255, 0));
    }
    else if (data == 2)
    {
      strip.SetPixelColor(0, RgbColor(255, 0, 0));
    }
    else if (data == 3)
    {
      strip.SetPixelColor(0, RgbColor(200, 200, 0));
    }
  }
  else if (parameter == MOD_LED)
  {
    if (data == 0)
    {
      strip.SetPixelColor(1, RgbColor(0, 0, 0));
    }
    else if (data == 1)
    {
      strip.SetPixelColor(1, RgbColor(0, 255, 0));
    }
    else if (data == 2)
    {
      strip.SetPixelColor(1, RgbColor(255, 0, 0));
    }
    else if (data == 3)
    {
      strip.SetPixelColor(1, RgbColor(200, 200, 0));
    }
  }
  else if (parameter == FX_LED)
  {
    if (data == 0)
    {
      strip.SetPixelColor(2, RgbColor(0, 0, 0));
    }
    else if (data == 1)
    {
      strip.SetPixelColor(2, RgbColor(0, 255, 0));
    }
    else if (data == 2)
    {
      strip.SetPixelColor(2, RgbColor(255, 0, 0));
    }
    else if (data == 3)
    {
      strip.SetPixelColor(2, RgbColor(200, 200, 0));
    }
    controller[2].updateValue(data);
  }
  strip.Show();
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