#ifndef ACTUATORS_H
#define ACTUATORS_H

#include <Arduino.h>
#include <Servo.h>

class Actuators {
private:
  Servo servo;
  int ledPin = 13;
  int servoPin = 9;

public:
  void init();
  void updateState(int state);
  void segregateResidue(int id);

private:
  void blinkSlow();
  void blinkFast();
  void blinkPattern();
};

#endif
