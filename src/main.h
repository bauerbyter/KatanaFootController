#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include <NeoPixelBrightnessBus.h>
#include "sysex.h"
#include "config.h"
#include "Ticker.h"

#define MS3_DEBUG_MODE
#include "./libs/MS3.h"
#include <JC_Button.h>

#include "./model/control.h"
#include "model/channelButton.h"
#include "model/effectButton.h"
#include "model/ccButton.h"
#include "model/tapButton.h"
#include "model/bankButton.h"
#include "model/effectBankButton.h"

void setupKatana();
void updateKatana();
void connectKatana();
void handleIncomingData(unsigned long parameter, byte data);
void getKatanaStatus();
void setLed(Led led);
void sendTap(int index);
void bankChange(int index);
bool effectButtonPressed(int index);
void blinkTapLed();

#endif
