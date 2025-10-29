#include "sensor.h"
#include <Wire.h>

Sensor::Sensor(int sda_pin, int scl_pin, int shut_pin)
  : sda_pin(sda_pin), scl_pin(scl_pin), shut_pin(shut_pin) {}

void Sensor::init() {

  Wire.begin(sda_pin, scl_pin);

  pinMode(shut_pin, OUTPUT);
  digitalWrite(shut_pin, LOW);
  delay(10);                     // se apaga el sensor
  digitalWrite(shut_pin, HIGH); 
  delay(10);                     // se reinicia

  sensor_ok = lox.begin(0x29, &Wire);

  if (!sensor_ok) {
    Serial.println(F("❌ No se detecta VL53L0X"));
    while (true) delay(1000);
  }
  Serial.println(F("✅ VL53L0X detectado correctamente"));
}

bool Sensors::detectUser() {
  return digitalRead(irUserPin);
}

bool Sensors::detectWaste() {
  return digitalRead(irWastePin);
}
