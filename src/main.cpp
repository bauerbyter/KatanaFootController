#include <Arduino.h>
#include "main.h"
#include "sysex.h"
#include "config.h"
#include "./model/model.h"
//#define MS3_DEBUG_MODE
#include "./libs/MS3.h"
#include <JC_Button.h>
#include <NeoPixelBrightnessBus.h>
#include "Ticker.h"

MS3 katana;

Switch control1 = Switch(29, Command{PC, W_PC, R_PATCH, 0x01, 0x05, 2}, 0);
Switch control2 = Switch(33, Command{PC, W_PC, R_PATCH, 0x02, 0x06, 2}, 1);
Switch control3 = Switch(26, Command{PC, W_PC, R_PATCH, 0x03, 0x07, 2}, 2);
Switch control4 = Switch(36, Command{PC, W_PC, R_PATCH, 0x04, 0x08, 2}, 3);
Latch control5 = Latch(39, Command{CC, PREAMP_BOOST, PREAMP_BOOST, 0x00, 0x01, 1}, 4, BOOST_LED);
Switch control6 = Switch(38, Command{EFFECT, BOOSTER, BOOSTER_LED, 0x00, 0x01, 1}, 9);
Switch control7 = Switch(37, Command{EFFECT, MOD, MOD_LED, 0x00, 0x01, 1}, 8);
Switch control8 = Switch(23, Command{EFFECT, FX, FX_LED, 0x00, 0x01, 1}, 7);
Switch control9 = Switch(22, Command{EFFECT, DELAY, DELAY_LED, 0x00, 0x01, 1}, 6);
Switch control10 = Switch(25, Command{EFFECT, REVERB, REVERB_LED, 0x00, 0x01, 1}, 5);
Switch control11 = Switch(32, Command{BANK, 0xFF, 0xFF, 0x00, 0x01, 1}, 10); //0xFF = not used
Switch control12 = Switch(28, Command{EFFECT_UP, 0xFF, 0xFF, 0x00, 0x01, 1}, 11);
Switch control13 = Switch(30, Command{CC, FOOT_VOLUME, FOOT_VOLUME, 100, 0, 1}, 12, MUTE_LED, true);
Switch control14 = Switch(27, Command{CC, LOOP, LOOP, 0x00, 0x01, 1}, 13);
Switch control15 = Switch(31, Command{TAP, TAP_TIME, TAP_TIME, 0x00, 0x01, 1}, 14);

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
bool temp = false;
void test()
{
  if (temp)
  {
    //setLED(14, PC_A_ON);
    setLED(14, LED_OFF);
  }
  else
  {
    setLED(14, LED_OFF);
  }
  temp = !temp;
}

Ticker tapTimer(test, 1000);
int bankNumber; //0 = Bank A, 1 = Bank B
bool isEffectUp = false;

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

  tapTimer.start(); //start the ticker.
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

void effectUpPressed(int index)
{
  if (controller[index]->getValue() == 1)
  {
    setLED(controller[index]->getLedPosition(), EFFECT_UP_ON);
    isEffectUp = true;
  }
  else
  {
    setLED(controller[index]->getLedPosition(), LED_OFF);
    isEffectUp = false;
  }
}

void pcPressed(int index)
{
  int value;
  if (bankNumber == 1)
  {
    value = controller[index]->getCommand().endValue;
  }
  else
  {
    value = controller[index]->getCommand().startValue;
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
  if (isEffectUp)
  {
    int state = controller[index]->getState();
    Serial.print("prestate: ");
    Serial.println(state);
    if (state == 0)
    {
      return;
    }
    else if (state >= 3)
    {
      state = 0;
    }
    unsigned long address;
    switch (controller[index]->getCommand().address)
    {
    case BOOSTER:
      address = BOOSTER_STATE;
      break;
    case MOD:
      address = MOD_STATE;
      break;
    case FX:
      address = FX_STATE;
      break;
    case DELAY:
      address = DELAY_STATE;
      break;
    case REVERB:
      address = REVERB_STATE;
      break;
    }
    katana.write(address, state, controller[index]->getCommand().valueSize);
    //SET EFFECT_UP Back
    for (int i = 0; i < CONTROL_SIZE; i++)
    {
      if (controller[i]->getCommand().type == EFFECT_UP)
      {
        controller[i]->updateValue(0);
        effectUpPressed(i);
        break;
      }
    }
  }
  else
  {
    katana.write(controller[index]->getCommand().address, controller[index]->getValue(), controller[index]->getCommand().valueSize);
  }
  isEffectUp = false;
}

//Todo: move
unsigned long tapLastPressed;
void tapPressed(int index)
{
  if ((millis() - tapLastPressed) > 2000)
  {
    tapLastPressed = millis();
    return;
  }
  unsigned long tempoMillis = millis() - tapLastPressed;
  tapLastPressed = millis();
  unsigned long tempo = 60000 / tempoMillis;

  if (tempo > 400)
  {
    tempo = 400;
  }
  if (tempo < 30)
  {
    tempo = 30;
  }
  uint8_t dd_time_msb = ((60000 / tempo) / 128);
  uint8_t dd_time_lsb = (60000 / tempo) - (dd_time_msb * 128);
  uint8_t data[2];
  data[0] = {dd_time_msb};
  data[1] = {dd_time_lsb};
  katana.send(controller[index]->getCommand().address, data, 2, MS3_WRITE); // bypassing the MS3 Queue, but only way to send 2 byte data
}

void loop()
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
        effectUpPressed(i);
        break;
      case TAP:
        tapPressed(i);
        break;
      }
    }
  }
  tapTimer.update();
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

  //SET BANK Back
  byte bankIndex = 0;
  for (int i = 0; i < CONTROL_SIZE; i++)
  {
    if (controller[i]->getCommand().type == BANK)
    {
      bankIndex = i;
      break;
    }
  }
  controller[bankIndex]->updateValue(bankNumber);
  bankPressed(bankIndex);

  if (controller[index]->getCommand().startValue == data)
  {
    setLED(controller[index]->getLedPosition(), PC_A_ON);
  }
  else if (controller[index]->getCommand().endValue == data)
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
  if (controller[index]->isInverted())
  {
    if (data == 0)
    {
      controller[index]->updateValue(0);
      setLED(controller[index]->getLedPosition(), controller[index]->getColor());
    }
    else if (data >= 1)
    {
      controller[index]->updateValue(1);
      setLED(controller[index]->getLedPosition(), LED_OFF);
    }
  }
  else
  {
    if (data == 0)
    {
      controller[index]->updateValue(0);
      setLED(controller[index]->getLedPosition(), LED_OFF);
    }
    else if (data >= 1)
    {
      controller[index]->updateValue(1);
      setLED(controller[index]->getLedPosition(), controller[index]->getColor());
    }
  }
}

void incomingEffect(int index, unsigned long parameter, byte data)
{
  controller[index]->setState(data);
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
//Todo useless - so remove
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

void incomingTap(int index, unsigned long parameter, byte data)
{
  /*unsigned long tempoMillis = millis() - tapLastPressed;
  tapLastPressed = millis();
  unsigned long tempo = 60000 / tempoMillis;

  if (tempo > 400)
  {
    tempo = 400;
  }
  if (tempo < 30)
  {
    tempo = 30;
  }
  uint8_t dd_time_msb = ((60000 / tempo) / 128);
  uint8_t dd_time_lsb = (60000 / tempo) - (dd_time_msb * 128);
  uint8_t data[2];
  data[0] = {dd_time_msb};
  data[1] = {dd_time_lsb};
  katana.send(controller[index]->getCommand().address, data, 2, MS3_WRITE)*/
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
      case TAP:
        incomingTap(i, parameter, data);
        break;
      default:
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
void disconnected()
{
  setAllLeds(LED_OFF);
  setLED(10, WARNING_LED);
}

void connected()
{
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