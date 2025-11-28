#include "sensor.h"
#include <Arduino.h>

// ---------------- Sensor ----------------
Sensor::Sensor(uint8_t xshutPin, uint8_t address)
  : xshutPin(xshutPin), address(address) {}

void Sensor::init() {
  pinMode(xshutPin, OUTPUT);
  digitalWrite(xshutPin, LOW);
  delay(10);
  digitalWrite(xshutPin, HIGH);
  delay(10);

  if (!lox.begin(address)) {
    Serial.print(F("❌ Sensor en XSHUT "));
    Serial.print(xshutPin);
    Serial.println(F(" no detectado"));
    while (true) delay(1000);
  }
  Serial.print(F("✅ Sensor en XSHUT "));
  Serial.print(xshutPin);
  Serial.println(F(" inicializado correctamente"));
}

uint16_t Sensor::readDistance() {
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false);
  if (measure.RangeStatus != 4) {
    return measure.RangeMilliMeter;
  } else {
    return 8190; // valor de error o "fuera de rango"
  }
}

// ---------------- UserDetector ----------------
UserDetector::UserDetector(Sensor& s1, Sensor& s2)
  : sensorA(s1), sensorB(s2) {}

void UserDetector::init() {
  // Inicializar ambos sensores
  sensorA.init();
  sensorB.init();
  Serial.println("✅ UserDetector inicializado");
}

bool UserDetector::detectUser() {
  unsigned long currentMillis = millis();
  Sensor& activeSensor = useFirstSensor ? sensorA : sensorB;

  uint16_t dist = activeSensor.readDistance();
  float deltaTime = (currentMillis - lastSwitchTime) / 1000.0; // segundos
  lastSwitchTime = currentMillis;

  // Alternar sensor
  useFirstSensor = !useFirstSensor;

  // Actualizar acumulador
  if (dist < 1000) { // 1 metro en mm
    detectionAccum += deltaTime;
    lastDetectionTime = currentMillis;
  } else if ((currentMillis - lastDetectionTime) > 3000) {
    detectionAccum = 0; // reiniciar si no hay detección en 3 s
  }

  // Retornar true si la acumulación supera 2 segundos
  if (detectionAccum >= 2.0) {
    detectionAccum = 0; // reset para siguiente detección
    return true;
  }
  return false;
}

// ---------------- WasteDetector ----------------
WasteDetector::WasteDetector(Sensor& s) : sensor(s) {}

void WasteDetector::init() {
  // Inicializar el sensor de residuo
  sensor.init();
  Serial.println("✅ WasteDetector inicializado");
}

bool WasteDetector::detectWaste() {
  uint16_t dist = sensor.readDistance();
  return dist < threshold;
}
