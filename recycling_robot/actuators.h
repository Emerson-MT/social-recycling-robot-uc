#ifndef ACTUATORS_H
#define ACTUATORS_H

#include <Arduino.h>
#include <Servo.h>
#include <Adafruit_PWMServoDriver.h>
#include <FastLED.h>

#define SERVOMIN  120  // Pulso mínimo (~0.6 ms)
#define SERVOMAX  600  // Pulso máximo (~2.4 ms)
#define STOP      370  // Pulso para detener el servo (ajústalo según tu servo)

class Actuators {

private:
  int ledPin; // Número de pin para los leds
  int numLeds; // Cantidad de leds en el arreglo
  int bright; // Nivel de brillo
  int servo1Channel; // Número de canal de servo 1
  int servo2Channel; // Número de canal de servo 2
  Adafruit_PWMServoDriver pca = Adafruit_PWMServoDriver(); // Instancia de driver PC9685
  CRGB* leds;  // Puntero al arreglo dinámico de LEDs

public:

  Actuators(int lp, int nl, int b, int s1c, int s2c);
  void init();
  void setLightsMode(String mode);
  void sortWaste(int id);

  void blinkSlow();
  void blinkFast();
  void blinkPattern();

  static const CRGB VERDE_AMAR;
  static const CRGB ROJO;
  static const CRGB BLANCO;
};

#endif
