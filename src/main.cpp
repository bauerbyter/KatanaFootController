#include <Arduino.h>
#include "main.h"
#include "sysex.h"
#include "config.h"
#include "./model/model.h"
//#define MS3_DEBUG_MODE
#include "./libs/MS3.h"
#include <JC_Button.h>
#include <NeoPixelBrightnessBus.h>

MS3 katana;

//Todo: Maybe dont use different style of Buttons like Button, Latch... 
//but instead using effectButton, ProgramChangeButton, BankUpButton...
//so we can save the status and set LEDs inside the object.
Switch control1 = Switch(29, Command{PC, W_PC, R_PATCH, 0x01, 0x01, 2}, 0);
Switch control2 = Switch(33, Command{PC, W_PC, R_PATCH, 0x02, 0x02, 2}, 1);
Switch control3 = Switch(26, Command{PC, W_PC, R_PATCH, 0x03, 0x03, 2}, 2);
Switch control4 = Switch(36, Command{PC, W_PC, R_PATCH, 0x04, 0x04, 2}, 3);
Latch control5 = Latch(39, Command{CC, PREAMP_BOOST, PREAMP_BOOST, 0x00, 0x01, 1}, 4);
Switch control6 = Switch(38, Command{EFFECT, BOOSTER, BOOSTER_LED, 0x00, 0x01, 1}, 9);
Switch control7 = Switch(37, Command{EFFECT, MOD, MOD_LED, 0x00, 0x01, 1}, 8);
Switch control8 = Switch(23, Command{EFFECT, FX, FX_LED, 0x00, 0x01, 1}, 7);
Switch control9 = Switch(22, Command{EFFECT, DELAY, DELAY_LED, 0x00, 0x01, 1}, 6);
Switch control10 = Switch(25, Command{EFFECT, REVERB, REVERB_LED, 0x00, 0x01, 1}, 5);
Switch control11 = Switch(32, Command{BANK, 0xFF, 0xFF, 0x00, 0x01, 1}, 10); //0xFF = not used
Switch control12 = Switch(28, Command{CC, 0xFF, 0xFF, 0x00, 0x01, 1}, 11);
Switch control13 = Switch(30, Command{CC, FOOT_VOLUME, FOOT_VOLUME, 100, 0, 1}, 12); //Todo: LED has to be inverted
Switch control14 = Switch(27, Command{CC, LOOP, LOOP, 0x00, 0x01, 1}, 13);
Switch control15 = Switch(31, Command{CC, 0xFF, 0xFF, 0x00, 0x01, 1}, 14);

#define CONTROL_SIZE 15

NeoPixelBrightnessBus<NeoRgbFeature, Neo800KbpsMethod> strip(CONTROL_SIZE, LED_PIN);

Control *controller[CONTROL_SIZE] = {
    &control1,
    &control2,
    &control3,
    &control4,
    &control5,
    &control6,
    &control7,
    &control8,
    &control9,
    &control10,
    &control11,
    &control12,
    &control13,
    &control14,
    &control15

};

int bankNumber; //0 = Bank A, 1 = Bank B

void setup()
{
  Serial.begin(115200);

  strip.Begin();
  strip.SetBrightness(LED_BRIGHTNESS);
  strip.Show();


  for (int i = 0; i < CONTROL_SIZE; i++) //has to be done. Maybe issue in JC_Button?
  {
    pinMode(controller[i]->getPin(), INPUT_PULLUP);
  }
  
  setupKatana();

  getKatanaStatus(false);
}

//Todo add to main.h
void bankPressed(int index)
{
  bankNumber = controller[index]->getValue();
  if (bankNumber > 0)
  {
    setLED(controller[index]->getLedPosition(), BANK_ON);
  }
  else
  {
    setLED(controller[index]->getLedPosition(), LED_OFF);
  }
}

void pcPressed(int index)
{
  int value = controller[index]->getValue();
  if (bankNumber == 1)
  {
    value = value + 4; //Todo: this is okay but not nice
  }
  katana.write(controller[index]->getCommand().address, value, controller[index]->getCommand().valueSize);
}

void ccPressed(int index)
{
  katana.write(controller[index]->getCommand().address, controller[index]->getValue(), controller[index]->getCommand().valueSize);
  katana.read(controller[index]->getCommand().address, 1);
}

void effectPressed(int index)
{
  //Todo add FX UP Code .. also think about saving the FX status
  katana.write(controller[index]->getCommand().address, controller[index]->getValue(), controller[index]->getCommand().valueSize);
}

void loop() //Todo set LEDs when writing and check when incoming
{
  for (unsigned int i = 0; i < CONTROL_SIZE; i++)
  {
    if (controller[i]->changed())
    {
      switch (controller[i]->getCommand().type)
      {
      case BANK:
        bankPressed(i);
        break;
      case PC:
        pcPressed(i);
        break;
      case CC:
        ccPressed(i);
        break;
      case EFFECT:
        effectPressed(i);
        break;
      case EFFECT_UP:
        //Todo
        break;
      }
    }
  }

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
  }
}

void incomingPC(int index, unsigned long parameter, byte data)
{
  if (data > 4)
  {
    bankNumber = 1;
  }
  else
  {
    bankNumber = 0;
  }

  if (controller[index]->getValue() == data)
  {
    setLED(controller[index]->getLedPosition(), PC_A_ON);
  }
  else if (controller[index]->getValue() + (bankNumber * 4) == data && bankNumber == 1) //Todo: NOT NICE
  {
    setLED(controller[index]->getLedPosition(), PC_B_ON);
  }
  else
  {
    setLED(controller[index]->getLedPosition(), LED_OFF);
  }
  getKatanaStatus(true);
}

void incomingCC(int index, unsigned long parameter, byte data)
{
  if (data == 0)
  {
    controller[index]->updateValue(0);
    setLED(controller[index]->getLedPosition(), LED_OFF);
  }
  else if (data >= 1)
  {
    controller[index]->updateValue(1);
    setLED(controller[index]->getLedPosition(), TEMPORARY);
  }
}

void incomingEffect(int index, unsigned long parameter, byte data)
{
  if (data == 0)
  {
    controller[index]->updateValue(0);
    setLED(controller[index]->getLedPosition(), LED_OFF);
  }
  else if (data == 1)
  {
    controller[index]->updateValue(1);
    setLED(controller[index]->getLedPosition(), EFFECT_GREEN);
  }
  else if (data == 2)
  {
    controller[index]->updateValue(1);
    setLED(controller[index]->getLedPosition(), EFFECT_RED);
  }
  else if (data == 3)
  {
    controller[index]->updateValue(1);
    setLED(controller[index]->getLedPosition(), EFFECT_YELLOW);
  }
}

void incomingBank(int index, unsigned long parameter, byte data)
{
  if (bankNumber > 0)
  {
    setLED(controller[index]->getLedPosition(), BANK_ON);
  }
  else
  {
    setLED(controller[index]->getLedPosition(), LED_OFF);
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
      switch (controller[i]->getCommand().type)
      {
      case PC:
        incomingPC(i, parameter, data);
        break;
      case CC:
        incomingCC(i, parameter, data);
        break;
      case EFFECT:
        incomingEffect(i, parameter, data);
        break;
      case BANK:
        incomingBank(i, parameter, data);
      case EFFECT_UP:
        //Todo
        break;
      }
    }
  }
}

void setLED(int position, RgbColor color)
{
  if (position >= 0)
  {
    strip.SetPixelColor(position, color);
    strip.Show();
  }
}

void setAllLeds(RgbColor color)
{
  for (unsigned int i = 0; i < strip.PixelCount(); i++)
  {
    setLED(i, color);
  }
}

//--------------
//----KATANA----
//--------------
void disconnected(){
  setAllLeds(LED_OFF);
  setLED(10,WARNING_LED);
}

void connected(){
  setAllLeds(LED_OFF);
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
      disconnected();
      Serial.println(F("Katana OFFLINE !"));
      Serial.println();
      delay(100);
      break;
    case MS3_READY:
      connected();
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

  disconnected();
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
    connected();
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