#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include <NeoPixelBrightnessBus.h>
#include "sysex.h"
#include "config.h"
#include "./model/control.h"
//#define MS3_DEBUG_MODE
#include "./libs/MS3.h"
#include <JC_Button.h>

void setupKatana();
void updateKatana();
void connectKatana();
void handleIncomingData(unsigned long parameter, byte data);
void getKatanaStatus();
void setLed(Led led);
void sendTap(int index);
void bankChange(int index);

#endif

