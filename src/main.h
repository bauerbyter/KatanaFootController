#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include <NeoPixelBrightnessBus.h>

void setupKatana();
void updateKatana();
void reconnectKatana();
void handleIncomingData(unsigned long parameter, byte data);
void getKatanaStatus(bool notPC);
void setLED(int position, RgbColor color);

#endif
