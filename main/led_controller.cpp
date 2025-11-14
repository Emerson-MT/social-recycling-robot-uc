#include "led_controller.h"

LEDController::LEDController(int pin, int numLeds, int brightness)
  : ledPin(pin), numLeds(numLeds), bright(brightness) {
    leds = new CRGB[numLeds];
}

void LEDController::init() {
    FastLED.setMaxPowerInVoltsAndMilliamps(5, 1500);
    FastLED.addLeds<WS2812B, ledPin, GRB>(leds, numLeds);
    FastLED.setBrightness(bright);
    FastLED.clear(true);
}

void LEDController::blinkSlow() {
    digitalWrite(ledPin, (millis() / 1000) % 2);
}

void LEDController::blinkFast() {
    digitalWrite(ledPin, (millis() / 200) % 2);
}

void LEDController::blinkPattern() {
    static unsigned long last = 0;
    if (millis() - last > 300) {
        digitalWrite(ledPin, !digitalRead(ledPin));
        last = millis();
    }
}

void LEDController::setMode(int mode) {
    switch(mode){
        case 0: digitalWrite(ledPin, LOW); break;
        case 1: blinkSlow(); break;
        case 2: digitalWrite(ledPin, HIGH); break;
        case 3: blinkFast(); break;
        case 4: blinkPattern(); break;
    }
}
