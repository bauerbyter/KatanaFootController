#ifndef SYSEX_H
#define SYSEX_H
#include <Arduino.h>


static const unsigned long W_PC =               0x00010000;
static const unsigned long R_PATCH =            0x00010000;
static const unsigned long R_PATCH_NAME =       0x00000000;

static const unsigned long BOOSTER =            0x60000010;
static const unsigned long MOD =                0x60000100;
static const unsigned long FX =                 0x60000300;


static const unsigned long PREAMP_BOOST =       0x6000002b;
static const unsigned long FOOT_VOLUME =        0x60000561;

static const unsigned long BOOSTER_LED =        0x6000065D;
static const unsigned long MOD_LED =            0x6000065E;
static const unsigned long FX_LED =             0x6000065F;
static const unsigned long DELAY_LED =          0x6000065E;
static const unsigned long REVERB_LED =         0x6000065F;


#endif