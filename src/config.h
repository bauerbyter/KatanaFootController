#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

//#define MIDI_CHANNEL 1

#define LED_PIN 6


#define DEBOUNCE_MS 50
#define LONGPRESSDURATION 1500
#define EXP_THRESHOLD 2

#define LED_BRIGHTNESS 30
#define LED_OFF RgbColor(0, 0, 0)
#define LED_EFFECT_GREEN RgbColor(0, 255, 0)
#define LED_EFFECT_RED RgbColor(255, 0, 0)
#define LED_EFFECT_YELLOW RgbColor(250, 180, 10)
#define LED_WARNING RgbColor(60, 0, 0)
#define LED_BOOST RgbColor(255, 0, 0)
#define LED_LOOP RgbColor(0, 150, 150)
#define LED_MUTE RgbColor(255, 0, 0)
#define LED_TAP RgbColor(0, 40, 40)
#define TAP_LED 14

#define LED_DEFAULT RgbColor(0, 120, 120)

#define LED_PC_A_ON RgbColor(0, 0, 255)
#define LED_PC_B_ON RgbColor(200, 0, 200)
#define LED_BANK_ON RgbColor(200, 0, 200)
#define LED_EFFECT_UP RgbColor(200, 0, 200)



#endif