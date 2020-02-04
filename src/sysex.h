#ifndef SYSEX_H
#define SYSEX_H
#include <Arduino.h>


static const unsigned long PARA_PC =                     0x00010000;
static const unsigned long PARA_PATCH_NAME =             0x00000000;

static const unsigned long PARA_BOOSTER =                0x60000010;
static const unsigned long PARA_MOD =                    0x60000100;
static const unsigned long PARA_FX =                     0x60000300;
static const unsigned long PARA_DELAY =                  0x60000500;
static const unsigned long PARA_REVERB =                 0x60000540;
static const unsigned long PARA_LOOP =                   0x60000562;
static const unsigned long PARA_INPUT_VOLUME =           0x60000561;
static const unsigned long PARA_TAP_TIME =               0x60000502;



static const unsigned long PARA_PREAMP_BOOST =           0x6000002B;
static const unsigned long PARA_FOOT_VOLUME =            0x60000561;

static const unsigned long PARA_BOOSTER_LED =            0x6000065D;
static const unsigned long PARA_MOD_LED =                0x6000065E;
static const unsigned long PARA_FX_LED =                 0x6000065F;
static const unsigned long PARA_DELAY_LED =              0x60000660;
static const unsigned long PARA_REVERB_LED =             0x60000661;



#endif