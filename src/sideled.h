#ifndef SIDELED_H_   /* Include guard */
#define SIDELED_H_

#define SIDELED_NUM_LEDS 10
#define SIDELED_DATA_PIN 25

#define SIDELED_STATE_OFF 0
#define SIDELED_STATE_ON 1
#define SIDELED_STATE_BLINK 2
#define SIDELED_STATE_BLINK_INVERS 3
#define SIDELED_STATE_FADE 4
#define SIDELED_STATE_FADE_RG 5
#define SIDELED_STATE_FADE_GB 6
#define SIDELED_STATE_FADE_BR 7


#include <Arduino.h>
#include <FastLED.h>


void init_sideled();

void set_sideled_state(uint8_t led_start, uint8_t led_end, uint8_t state);

void set_sideled_color(uint8_t led_start, uint8_t led_end, CRGB color);

#endif /* SIDELED_H_ */