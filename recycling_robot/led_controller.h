#ifndef LED_CONTROLLER_H
#define LED_CONTROLLER_H

#include <FastLED.h>
#include <Arduino.h>

class LEDController {
public:
    LEDController(int pin, int numLeds, int brightness);
    void init();
    void blinkSlow();
    void blinkFast();
    void blinkPattern();
    void setMode(int mode);

private:
    int ledPin;
    int numLeds;
    int bright;
    CRGB* leds;
};

#endif
