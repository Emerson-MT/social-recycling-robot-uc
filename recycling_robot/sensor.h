#ifndef SENSOR_H
#define SENSOR_H

#include <Wire.h>
#include <Arduino.h>
#include "Adafruit_VL53L0X.h"

class Sensor {
  public:
    Sensor(uint8_t xshutPin, uint8_t address);
    void init();
    uint16_t readDistance();      // devuelve distancia en mm
  private:
    uint8_t xshutPin;
    uint8_t address;
    Adafruit_VL53L0X lox;
};

class UserDetector {
  public:
    UserDetector(Sensor& s1, Sensor& s2);
    bool detectUser();
  private:
    Sensor& sensorA;
    Sensor& sensorB;
    unsigned long lastDetectionTime = 0;
    unsigned long lastSwitchTime = 0;
    bool useFirstSensor = true;
    float detectionAccum = 0;   // tiempo acumulado en segundos
};

class WasteDetector {
  public:
    WasteDetector(Sensor& s);
    bool detectWaste();
  private:
    Sensor& sensor;
    const uint16_t threshold = 200; // 20 cm en mm
};

#endif
