#include "main.h"

//#define DEBUG_MODE
#ifdef DEBUG_MODE
#define DEBUG(x) Serial.print(x)
#define DEBUGLN(x) Serial.println(x)
#define DEBUG_AS(x, y) Serial.print(x, y)
#else
#define DEBUG(x) (void)(x)
#define DEBUGLN(x) (void)(x)
#define DEBUG_AS(x, y) (void)(x)
#endif

MS3 katana;

ChannelButton control1 = ChannelButton(29, 0, PARA_PC, PARA_PC, 1, 5);
ChannelButton control2 = ChannelButton(33, 1, PARA_PC, PARA_PC, 2, 6);
ChannelButton control3 = ChannelButton(26, 2, PARA_PC, PARA_PC, 3, 7);
ChannelButton control4 = ChannelButton(36, 3, PARA_PC, PARA_PC, 4, 8);
CCButton control5 = CCButton(39, 4, PARA_PREAMP_BOOST, PARA_PREAMP_BOOST, 0, 1, 1, LED_BOOST, true, true);
EffectButton control6 = EffectButton(38, 9, PARA_BOOSTER, PARA_BOOSTER_LED, PARA_BOOSTER_STATE);
EffectButton control7 = EffectButton(37, 8, PARA_MOD, PARA_MOD_LED, PARA_MOD_STATE);
EffectButton control8 = EffectButton(23, 7, PARA_FX, PARA_FX_LED, PARA_FX_STATE);
EffectButton control9 = EffectButton(22, 6, PARA_DELAY, PARA_DELAY_LED, PARA_DELAY_STATE);
EffectButton control10 = EffectButton(25, 5, PARA_REVERB, PARA_REVERB_LED, PARA_REVERB_STATE);
BankButton control11 = BankButton(32, 10);
EffectBankButton control12 = EffectBankButton(28, 11);
CCButton control13 = CCButton(30, 12, PARA_FOOT_VOLUME, PARA_FOOT_VOLUME, 100, 0, 1, LED_MUTE, false, true);
CCButton control14 = CCButton(27, 13, PARA_LOOP, PARA_LOOP, 0, 1, 1, LED_LOOP, false, true);
TapButton control15 = TapButton(31, 14, PARA_TAP_TIME, PARA_TAP_TIME);

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
    &control15};

Ticker tapTimer(blinkTapLed, 5000);
bool isEffectBankPressed = false;
bool isTapLedOn = false;

void setup()
{
  Serial.begin(115200);

  for (int i = 0; i < CONTROL_SIZE; i++) //has to be done. Maybe issue in JC_Button?
  {
    pinMode(controller[i]->getPin(), INPUT_PULLUP);
  }

  strip.Begin();
  strip.SetBrightness(LED_BRIGHTNESS);
  strip.Show();

  setupKatana();
}

void loop()
{
  for (int i = 0; i < CONTROL_SIZE; i++)
  {
    if (controller[i]->changed())
    {
      DEBUG("Button was pressed - Number: ");
      DEBUGLN(i);
      DEBUG("Katana.write: ");
      DEBUG_AS(controller[i]->getSendParameter(), HEX);
      DEBUG(" - ");
      DEBUG_AS(controller[i]->getValue(), HEX);
      DEBUGLN("");

      switch (controller[i]->getClassType())
      {
      case TAP_BUTTON:
        sendTap(i);
        break;
      case BANK_BUTTON:
        bankChange(i);
        break;
      case EFFECT_BANK_BUTTON:
        isEffectBankPressed = controller[i]->getValue();
        setLed(controller[i]->getLed(controller[i]->getValue()));
        break;
      case EFFECT_BUTTON:
        if (isEffectBankPressed && effectButtonPressed(i))
        {
          break;
        }
      default:
        katana.write(controller[i]->getSendParameter(), controller[i]->getValue(), controller[i]->getValueSize());
        break;
      }

      if (controller[i]->getNeedsRead())
      {
        katana.read(controller[i]->getReadParameter(), controller[i]->getValueSize());
        DEBUG("Katana.read: ");
        DEBUGLN(controller[i]->getReadParameter());
      }
    }
  }
  tapTimer.update();
  updateKatana();
}

void handleIncomingData(unsigned long parameter, byte data)
{
  DEBUG("Receive: ");
  DEBUG_AS(parameter, HEX);
  DEBUG(" - ");
  DEBUG_AS(data, HEX);
  DEBUGLN("");
  bool needUpdate = false;

  for (unsigned int i = 0; i < CONTROL_SIZE; i++)
  {
    if (controller[i]->paramaterMatch(parameter))
    {
      if (parameter == PARA_TAP_TIME)
      {
        int calc = (katana.dataOut2 * 128) + data;

        controller[i]->update(calc);
        tapTimer.interval(calc / 2);
        tapTimer.start();
      }
      else
      {
        controller[i]->update(data);
        setLed(controller[i]->getLed(data));
      }

      if (controller[i]->getClassType() == CHANNEL_BUTTON)
      {
        needUpdate = true;
      }
    }
  }
  if (needUpdate)
  {
    getKatanaStatus();
  }
}

void setLed(Led led)
{
  if (led.ledPosition >= 0 && led.ledPosition < CONTROL_SIZE)
  {
    strip.SetPixelColor(led.ledPosition, led.color);
    strip.Show();
  }
}

void clearAllLeds()
{
  for (byte i = 0; i < CONTROL_SIZE; i++)
  {
    setLed(Led{i, LED_OFF});
  }
}

void sendTap(int index)
{
  uint8_t dd_time_msb = ((60000 / controller[index]->getValue()) / 128);
  uint8_t dd_time_lsb = ((60000 / controller[index]->getValue()) - (dd_time_msb * 128));
  uint8_t data[2];
  data[0] = {dd_time_msb};
  data[1] = {dd_time_lsb};
  katana.send(controller[index]->getSendParameter(), data, 2, MS3_WRITE); //Bypass the queue..
}

void bankChange(int index)
{
  for (unsigned int i = 0; i < CONTROL_SIZE; i++)
  {
    if (controller[i]->getClassType() == CHANNEL_BUTTON)
    {
      ChannelButton *temp = (ChannelButton *)controller[i];
      temp->bankUpdate(controller[index]->getValue());
    }
  }
  setLed(controller[index]->getLed(controller[index]->getValue() + 4)); //Todo not nice
}

bool effectButtonPressed(int index)
{

  EffectButton *temp = (EffectButton *)controller[index];
  if (temp->getState() != 0)
  {
    katana.write(temp->getTypeParameter(), temp->getType(), temp->getValueSize());

    /* Only if you want the Button to reset after an Effect was pressed
    isEffectBankPressed = false;
    for (unsigned int i = 0; i < CONTROL_SIZE; i++)
    {
      if (controller[i]->getClassType() == EFFECT_BANK_BUTTON)
      {
        controller[i]->update(0);
        setLed(controller[i]->getLed(0));
      }
    }*/
    return true;
  }
  return false;
}

void getKatanaStatus()
{
  for (unsigned int i = 0; i < CONTROL_SIZE; i++)
  {
    if (controller[i]->getClassType() != CHANNEL_BUTTON && controller[i]->getClassType() != BANK_BUTTON)
    {
      katana.read(controller[i]->getReadParameter(), controller[i]->getValueSize());
    }
  }
}

void updateKatana()
{
  unsigned long parameter = 0;
  uint8_t data = 0;

  // Check for incoming data or send a queued item.
  switch (katana.update(parameter, data))
  {
  case MS3_NOT_READY:
    Serial.println(F("Katana OFF-LINE !"));
    Serial.println();
    connectKatana();
    break;

  // Fetch the current active patch on the Katana.
  case MS3_READY:
    katana.setEditorMode();
    getKatanaStatus();
    katana.read(PARA_PC, 0x02);
    break;

  // Parse the incoming data.
  case MS3_DATA_RECEIVED:
    handleIncomingData(parameter, data);
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
  connectKatana();
}

void connectKatana()
{
  unsigned long parameter = 0;
  byte data = 0;
  byte editMode = 0;
  while (editMode == 0)
  {
    switch (katana.update(parameter, data))
    {
    case MS3_NOT_READY:
      clearAllLeds();
      setLed(Led{10, LED_WARNING});
      Serial.println(F("Katana OFFLINE !"));
      Serial.println();
      delay(100);
      break;
    case MS3_READY:
      clearAllLeds();
      katana.setEditorMode();
      getKatanaStatus();
      katana.read(PARA_PC, 0x02);
      editMode = 1;
      Serial.println(F("Ready!"));

      break;
    }
    delay(100);
  }
}

void blinkTapLed()
{
  if (isTapLedOn)
  {
    setLed(Led{TAP_LED, LED_OFF});
  }
  else
  {
    setLed(Led{TAP_LED, LED_TAP});
  }
  isTapLedOn = !isTapLedOn;
}
