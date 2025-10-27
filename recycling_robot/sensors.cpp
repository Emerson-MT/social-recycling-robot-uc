#include "Sensors.h"

void Sensors::init() {
  pinMode(irUserPin, INPUT);
  pinMode(irWastePin, INPUT);
}

bool Sensors::detectUser() {
  return digitalRead(irUserPin);
}

bool Sensors::detectWaste() {
  return digitalRead(irWastePin);
}
