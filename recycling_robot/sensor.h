#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>
#include <Adafruit_VL53L0X.h>

class Sensor {
private:
  int sda_pin;
  int scl_pin;
  int shut_pin;
  int irUserPin = 2;
  int irWastePin = 3;

public:

  Sensor(int sda_pin, int scl_pin, int shut_pin);
  void init();
  bool detectUser();
  bool detectWaste();
};

#endif
