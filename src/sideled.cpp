#include "sideled.h"

CRGB leds_current[SIDELED_NUM_LEDS];
CRGB leds_color[SIDELED_NUM_LEDS];
uint8_t leds_state[SIDELED_NUM_LEDS];
uint8_t changed = false;
uint8_t block = false;

unsigned long next_blink = 0;
unsigned long next_effekt = 0;

void LEDtask(void *arg){
  while (1){
    if(changed) {
      for(int a = 0; a < SIDELED_NUM_LEDS; a++) {
        leds_current[a] = leds_color[a];
        if(leds_state[a] == SIDELED_STATE_OFF) {
          leds_current[a] = CRGB::Black;
        }
        if(leds_state[a] == SIDELED_STATE_FADE) {
          leds_state[a] = SIDELED_STATE_FADE_RG;
          leds_current[a].blue = 0x00;
          leds_current[a].red = 0xFF;
          leds_current[a].green = 0x00;
        }
      }
      FastLED.show();
      changed = false;
      block = false;
    }
    if(next_blink < millis() && !block) {
      for(int a = 0; a < SIDELED_NUM_LEDS; a++) {
        if(leds_state[a] == SIDELED_STATE_BLINK_INVERS) {
          leds_current[a] = CRGB::Black;
          leds_state[a] = SIDELED_STATE_BLINK;
        }
        else if(leds_state[a] == SIDELED_STATE_BLINK) {
          leds_current[a] = leds_color[a];
          leds_state[a] = SIDELED_STATE_BLINK_INVERS;
        }
      }
      FastLED.show();
      next_blink = millis() + 500;
    }
    if(next_effekt < millis() && !block) {
      for(int a = 0; a < SIDELED_NUM_LEDS; a++) {
        if(leds_state[a] == SIDELED_STATE_FADE_RG) {
          leds_current[a].red -= 10;
          leds_current[a].green += 10;
          if(leds_current[a].red <= 10) {
            leds_state[a] = SIDELED_STATE_FADE_GB;
          }
        }
        else if(leds_state[a] == SIDELED_STATE_FADE_GB) {
          leds_current[a].green -= 10;
          leds_current[a].blue += 10;
          if(leds_current[a].green <= 10) {
            leds_state[a] = SIDELED_STATE_FADE_BR;
          }
        }
        else if(leds_state[a] == SIDELED_STATE_FADE_BR) {
          leds_current[a].blue -= 10;
          leds_current[a].red += 10;
          if(leds_current[a].blue <= 10) {
            leds_state[a] = SIDELED_STATE_FADE_RG;
          }
        }
      }
      FastLED.show();
      next_effekt = millis() + 50;
    }
    delay(50);
  }
}

void init_sideled() {
  // Init default values for leds
    for(int a = 0; a < SIDELED_NUM_LEDS; a++) {
      leds_color[a] = CRGB::Black;
      leds_current[a] = leds_color[a];
      leds_state[a] = SIDELED_STATE_FADE;
      changed = true;
    }
    // Init FastLED
    FastLED.addLeds<NEOPIXEL, SIDELED_DATA_PIN>(leds_current, SIDELED_NUM_LEDS);
    // Start background task
    xTaskCreatePinnedToCore(LEDtask, "LEDTask", 4096, NULL, 2, NULL, 0);
}

void set_sideled_state(uint8_t led_start, uint8_t led_end, uint8_t state) {
  block = true;
  if(led_start >= SIDELED_NUM_LEDS || led_end > SIDELED_NUM_LEDS || led_start >= led_end) return;
  for(uint8_t a = led_start; a < led_end; a++) {
    leds_state[a] = state;
  }
  changed = true;
}

void set_sideled_color(uint8_t led_start, uint8_t led_end, CRGB color) {
  block = true;
  if(led_start >= SIDELED_NUM_LEDS || led_end > SIDELED_NUM_LEDS || led_start >= led_end) return;
  for(uint8_t a = led_start; a < led_end; a++) {
    leds_color[a] = color;
  }
  changed = true;
}