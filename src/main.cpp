#include <Arduino.h>
#include <main.h>
#include <sysex.h>
#include "config.h"
#include <./model/control.h>
//#define MS3_DEBUG_MODE
#include "./libs/MS3.h"
#include <JC_Button.h>
#include <NeoPixelBrightnessBus.h>

MS3 katana;
NeoPixelBrightnessBus<NeoRgbFeature, Neo800KbpsMethod> strip(6, 6);

Switch control1 = Switch(2, Command{PC, W_PC, R_PATCH, 0x01, 0x01, 2}, 0);
Switch control2 = Switch(3, Command{PC, W_PC, R_PATCH, 0x02, 0x02, 2}, 1);
Switch control3 = Switch(4, Command{PC, W_PC, R_PATCH, 0x03, 0x03, 2}, 2);
Switch control4 = Switch(A0, Command{CC, BOOSTER, BOOSTER_LED, 0x00, 0x01, 1}, 3);
Switch control5 = Switch(A1, Command{CC, MOD, MOD_LED, 0x00, 0x01, 1}, 4);
//Switch control6 = Switch(A2, Command{CC, FX, FX_LED, 0x00, 0x01, 1}, 5);
Switch control6 = Switch(A2, Command{BANK, 0, 0, 0x00, 0x01, 1}, 5);
//Latch control6 = Latch(A2, Command{CC, PREAMP_BOOST, FX_LED, 0x00, 0x01, 1}, 5);
Exp control7 = Exp(A3, Command{CC, FOOT_VOLUME, FX_LED, 100, 0x00, 1}, 6);

#define CONTROL_SIZE 7

Control *controller[CONTROL_SIZE] = {
    &control1,
    &control2,
    &control3,
    &control4,
    &control5,
    &control6,
    &control7,

};

int bankNumber;

void setup()
{
  Serial.begin(115200);

  strip.Begin();
  strip.SetBrightness(LED_BRIGHTNESS);
  strip.Show();

  setupKatana();

  getKatanaStatus(false);
}

void loop()
{
  for (unsigned int i = 0; i < CONTROL_SIZE; i++)
  {
    if (controller[i]->changed())
    {
      Command tempCommand = controller[i]->getCommand();
      if (tempCommand.type == BANK)
      {
        bankNumber = controller[i]->getValue();
        if (bankNumber > 0)
        {
          setLED(controller[i]->getLedPosition(), BANK_ON);
        }
        else
        {
          setLED(controller[i]->getLedPosition(), LED_OFF);
        }
      }
      else if (tempCommand.type == PC)
      {
        katana.write(tempCommand.address, controller[i]->getValue() + (bankNumber * 4), tempCommand.valueSize); //PFUSCH
      }
      else
      {
        katana.write(tempCommand.address, controller[i]->getValue(), tempCommand.valueSize);
      }
    }
  }
  //ask for values from Katana ..
  updateKatana();
}

void getKatanaStatus(bool notPC)
{
  for (unsigned int i = 0; i < CONTROL_SIZE; i++)
  {
    Command tempCommand = controller[i]->getCommand();
    if (!(notPC && tempCommand.type == PC))
    {
      katana.read(tempCommand.readAddress, tempCommand.valueSize);
    }
    //katana.read(R_PATCH_NAME, 1);
  }
}

void handleIncomingData(unsigned long parameter, byte data)
{
  /*Serial.print("Receive: ");
  Serial.print(parameter, HEX);
  Serial.print(" - ");
  Serial.println(data, HEX);*/

  for (unsigned int i = 0; i < CONTROL_SIZE; i++)
  {
    if (controller[i]->readAddressMatch(parameter))
    {
      // ---- PC ----
      if (controller[i]->getCommand().type == PC)
      {
        if (data > 4)
        {
          bankNumber = 1;
        }
        else
        {
          bankNumber = 0;
        }

        if (controller[i]->getValue() == data)
        {
          setLED(controller[i]->getLedPosition(), PC_A_ON);
        }
        else if (controller[i]->getValue() + (bankNumber * 4) == data && bankNumber == 1) //PFUSCH
        {
          setLED(controller[i]->getLedPosition(), PC_B_ON);
        }
        else
        {
          setLED(controller[i]->getLedPosition(), LED_OFF);
        }
        getKatanaStatus(true);
      }
      // ---- CC ----
      else if (controller[i]->getCommand().type == CC)
      {
        if (data == 0)
        {
          controller[i]->updateValue(0);
          setLED(controller[i]->getLedPosition(), LED_OFF);
        }
        else if (data == 1)
        {
          controller[i]->updateValue(1);
          setLED(controller[i]->getLedPosition(), EFFECT_GREEN);
        }
        else if (data == 2)
        {
          controller[i]->updateValue(1);
          setLED(controller[i]->getLedPosition(), EFFECT_RED);
        }
        else if (data == 3)
        {
          controller[i]->updateValue(1);
          setLED(controller[i]->getLedPosition(), EFFECT_YELLOW);
        }
      }
    }
    // ---- BANK ----
    if (controller[i]->getCommand().type == BANK)
    {
      if (bankNumber > 0)
      {
        setLED(controller[i]->getLedPosition(), BANK_ON);
      }
      else
      {
        setLED(controller[i]->getLedPosition(), LED_OFF);
      }
    }
  }
}

void setLED(int position, RgbColor color)
{
  strip.SetPixelColor(position, color);
  strip.Show();
}

void setAllLeds(RgbColor color)
{
  for (unsigned int i = 0; i < strip.PixelCount(); i++)
  {
    setLED(i, color);
  }
}

void notConnected()
{
}

//--------------
//----KATANA----
//--------------
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
      setAllLeds(WARNING_LED);
      Serial.println(F("Katana OFFLINE !"));
      Serial.println();
      delay(100);
      break;
    case MS3_READY:
      setAllLeds(LED_OFF);
      katana.setEditorMode();
      katana.read(R_PATCH, 0x02);
      editMode = 1;
      Serial.println(F("Ready!"));
      break;
    }
  }
}
void reconnectKatana(void)
{

  setAllLeds(WARNING_LED);
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
    setAllLeds(LED_OFF);
    Serial.println(F("############ Back again Baby!"));
    Serial.println();
    katana.setEditorMode();
    getKatanaStatus(false);
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

  // Fetch the current active patch on the Katana.
  case MS3_READY:
    katana.setEditorMode();
    katana.read(R_PATCH, 0x02);
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