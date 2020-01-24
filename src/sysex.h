
//static const uint32_t PC =     0x60000140; //Todo
//static const unsigned long PC =     0x60000140; //Todo
//static const uint32_t FX_ON = 0x60000300;
// Booster SOLO
//Katana.write(0x60000015,1,1);

//Preamp Solo
//Katana.write(0x6000002b,0,1);

#ifndef SYSEX_H
#define SYSEX_H
#include <Arduino.h>


static const unsigned long PC =             0x00010000;
const unsigned long P_PATCH =               0x00010000;

static const unsigned long BOOSTER =        0x60000010;
static const unsigned long MOD =            0x60000100;
static const unsigned long FX =             0x60000300;

static const unsigned long BOOSTER_LED =    0x6000065D;
static const unsigned long MOD_LED =        0x6000065E;
static const unsigned long FX_LED =         0x6000065F;
static const unsigned long DELAY_LED =      0x6000065E;
static const unsigned long REVERB_LED =     0x6000065F;

#endif