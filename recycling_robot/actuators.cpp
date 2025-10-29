#include "Actuators.h"

// Definición de las constantes estáticas de color (una sola vez)
const CRGB Actuators::VERDE_AMAR = CRGB(180, 255, 0);
const CRGB Actuators::ROJO       = CRGB(255, 0, 0);
const CRGB Actuators::BLANCO     = CRGB(255, 255, 255);

Actuators::Actuators(int lp, int nl, int b, int s1c, int s2c)
  : ledPin(lp), numLeds(nl), bright(b), servo1Channel(s1c), servo2Channel(s2c) {

    leds = new CRGB[numLeds];
  }

void Actuators::init() {

  // LEDs
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 1500);
  FastLED.addLeds<WS2812B, PIN_LED, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(bright);
  FastLED.clear(true);

  // Servos 
  pca.begin();
  pca.setPWMFreq(50); // 50 Hz para servos estándar

  pca.setPWM(servo1Channel, 0, STOP);
  pca.setPWM(servo2Channel, 0, STOP);


  delay(1000);
}

void Actuators::setLightsMode(String mode) {
  switch (mode) {
    case 0: digitalWrite(ledPin, LOW); break;        // Hibernación
    case 1: blinkSlow(); break;                      // Despertando
    case 2: digitalWrite(ledPin, HIGH); break;       // Clasificar
    case 3: blinkFast(); break;                      // Interacción
    case 4: blinkPattern(); break;                   // Agradecimiento
  }
}

void Actuators::sortWaste(int id) {
}

void Actuators::blinkSlow() {
  digitalWrite(ledPin, (millis() / 1000) % 2);
}

void Actuators::blinkFast() {
  digitalWrite(ledPin, (millis() / 200) % 2);
}

void Actuators::blinkPattern() {
  static unsigned long last = 0;
  if (millis() - last > 300) {
    digitalWrite(ledPin, !digitalRead(ledPin));
    last = millis();
  }
}
