#ifndef ACTUATORS_H
#define ACTUATORS_H

#include <Arduino.h>
#include <Servo.h>
#include <Adafruit_PWMServoDriver.h>
#include <FastLED.h>

#define SERVOMIN  150  // Pulso mínimo (~0.6 ms)
#define SERVOMAX  600  // Pulso máximo (~2.4 ms)
#define STOP      370  // Pulso para detener el servo (ajústalo según tu servo)

class Actuators {

private:
  // Leds
  int ledPin; // Número de pin para los leds
  int numLeds; // Cantidad de leds en el arreglo
  int bright; // Nivel de brillo
  CRGB* leds;  // Puntero al arreglo dinámico de LEDs
  // Servos
  int servo1Channel; // Número de canal de servo 1
  int servo2Channel; 
  int currentAngle[16];
  int targetAngle[16];
  int speedPercent[16];
  // Driver
  Adafruit_PWMServoDriver pca = Adafruit_PWMServoDriver(); // Instancia de driver PC9685
  

public:
  // Constructor
  Actuators(int lp, int nl, int b, int s1c, int s2c);
  // Inicializador
  void init();
  // Leds
  void blinkSlow();
  void blinkFast();
  void blinkPattern();
  void setLightsMode(String mode);
  // Constantes para leds
  static const CRGB VERDE_AMAR;
  static const CRGB ROJO;
  static const CRGB BLANCO;
  // Servo motores
  int angleToPulse(int ang);
  void setTargetAngles(int t);
  void moveServoSmooth(int canal);
  bool isMotionComplete();
};

#endif
