#include <Arduino.h>
#include "main.h"
#include "sysex.h"
#include "config.h"
#include "./model/control.h"
//#define MS3_DEBUG_MODE
#include "./libs/MS3.h"
#include <JC_Button.h>
#include <NeoPixelBrightnessBus.h>

#define DEBUG_MODE
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

ChannelButton control1 = ChannelButton(29, 0, Command{W_PC, R_PATCH, 2, 1, 5, false});
ChannelButton control2 = ChannelButton(33, 0, Command{W_PC, R_PATCH, 2, 2, 6, false});
ChannelButton control3 = ChannelButton(26, 0, Command{W_PC, R_PATCH, 2, 3, 7, false});
ChannelButton control4 = ChannelButton(36, 0, Command{W_PC, R_PATCH, 2, 4, 8, false});
//Latch control5 = Latch(39, Command{CC, PREAMP_BOOST, PREAMP_BOOST, 0x00, 0x01, 1}, 4);
//Control control6 = Control(38, 1, Command{ BOOSTER, BOOSTER_LED, 1,1, false});
/*Switch control7 = Switch(37, Command{EFFECT, MOD, MOD_LED, 0x00, 0x01, 1}, 8);
Switch control8 = Switch(23, Command{EFFECT, FX, FX_LED, 0x00, 0x01, 1}, 7);
Switch control9 = Switch(22, Command{EFFECT, DELAY, DELAY_LED, 0x00, 0x01, 1}, 6);
Switch control10 = Switch(25, Command{EFFECT, REVERB, REVERB_LED, 0x00, 0x01, 1}, 5);
Switch control11 = Switch(32, Command{BANK, 0xFF, 0xFF, 0x00, 0x01, 1}, 10); //0xFF = not used
Switch control12 = Switch(28, Command{CC, 0xFF, 0xFF, 0x00, 0x01, 1}, 11);
Switch control13 = Switch(30, Command{CC, FOOT_VOLUME, FOOT_VOLUME, 100, 0, 1}, 12); //Todo: LED has to be inverted
Switch control14 = Switch(27, Command{CC, LOOP, LOOP, 0x00, 0x01, 1}, 13);
Switch control15 = Switch(31, Command{CC, 0xFF, 0xFF, 0x00, 0x01, 1}, 14);
*/
#define CONTROL_SIZE 4

NeoPixelBrightnessBus<NeoRgbFeature, Neo800KbpsMethod> strip(CONTROL_SIZE, LED_PIN);

Control *controller[CONTROL_SIZE] = {
    &control1,
    &control2,
    &control3,
    &control4,
    //&control5,
    //&control6,
    /* &control7,
    &control8,
    &control9,
    &control10,
    &control11,
    &control12,
    &control13,
    &control14,
    &control15*/

};

int bankNumber; //0 = Bank A, 1 = Bank B

void setup()
{
  Serial.begin(115200);

  strip.Begin();
  strip.SetBrightness(LED_BRIGHTNESS);
  strip.Show();
}

void loop()
{
  for (int i = 0; i < CONTROL_SIZE; i++)
  {
    if (controller[i]->changed())
    {
      /*
      if(it is a Bank UP){}
      updateChannelButtons



      */
      DEBUG("Button was pressed - Number: ");
      DEBUGLN(i);
      Command tempCommand = controller[i]->getCommand();
      katana.write(tempCommand.sendParameter, tempCommand.value, tempCommand.valueSize);
      DEBUG("Katana.write: ");
      DEBUG(tempCommand.sendParameter);
      DEBUG(" - ");
      DEBUGLN(tempCommand.value);
      if (tempCommand.needsRead)
      {
        katana.read(tempCommand.readParameter, tempCommand.valueSize);
        DEBUG("Katana.read: ");
        DEBUG(tempCommand.readParameter);
        DEBUG(" - ");
        DEBUGLN(tempCommand.value);
      }
    }
  }

  //updateKatana();
}
