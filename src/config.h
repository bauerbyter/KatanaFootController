#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

//#define MIDI_CHANNEL 1

#define LED_PIN 6

#define LED_BRIGHTNESS 30
#define DEBOUNCE_MS 50
#define LONGPRESSDURATION 1500
#define EXP_THRESHOLD 2

#define LED_OFF RgbColor(0, 0, 0)
#define EFFECT_GREEN RgbColor(0, 255, 0)
#define EFFECT_RED RgbColor(255, 0, 0)
#define EFFECT_YELLOW RgbColor(250, 180, 10)
#define WARNING_LED RgbColor(255, 0, 0)
#define BOOST_LED RgbColor(255, 0, 0)
#define LOOP_LED RgbColor(0, 150, 0)

#define PC_A_ON RgbColor(0, 0, 255)
#define PC_B_ON RgbColor(200, 0, 200)
#define BANK_ON RgbColor(200, 0, 200)



#endif