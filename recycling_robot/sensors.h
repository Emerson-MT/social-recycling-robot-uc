#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>

class Sensors {
private:
  int irUserPin = 2;
  int irWastePin = 3;

public:
  void init();
  bool detectUser();
  bool detectWaste();
};

#endif
