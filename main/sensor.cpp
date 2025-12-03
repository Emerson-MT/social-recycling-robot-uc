#include "sensor.h"
#include <Arduino.h>

void initAllVL53L0XSensors(uint8_t* xshutPins, uint8_t numSensors) {
    Serial.println("🔧 Inicializando sistema VL53L0X...");
    
    // PASO 1: Apagar TODOS los sensores
    Serial.println("Apagando todos los sensores...");
    for (uint8_t i = 0; i < numSensors; i++) {
        pinMode(xshutPins[i], OUTPUT);
        digitalWrite(xshutPins[i], LOW);
    }
    delay(500);  // Esperar a que todos estén apagados
    
    Serial.println("  ✅ Todos los sensores apagados");
}

// ---------------- Sensor ----------------
Sensor::Sensor(uint8_t xshutPin, uint8_t address)
  : xshutPin(xshutPin), address(address) {}

void Sensor::init() {
    // PASO 2: Encender SOLO este sensor
    Serial.print("  🔌 Encendiendo sensor XSHUT ");
    Serial.print(xshutPin);
    Serial.print(" (dir 0x");
    Serial.print(address, HEX);
    Serial.println(")...");
    
    digitalWrite(xshutPin, HIGH);
    delay(100);  // Esperar a que el sensor esté estable
    
    // PASO 3: Inicializar con dirección I2C única
    if (!lox.begin(address)) {
        Serial.print("  ❌ ERROR: Sensor XSHUT ");
        Serial.print(xshutPin);
        Serial.print(" (dir 0x");
        Serial.print(address, HEX);
        Serial.println(") NO detectado");
        
        // NO bloquear el programa, solo avisar
        // while (true) delay(1000);  // ← COMENTADO para no bloquear
    } else {
        Serial.print("  ✅ Sensor XSHUT ");
        Serial.print(xshutPin);
        Serial.print(" (dir 0x");
        Serial.print(address, HEX);
        Serial.println(") inicializado");
    }
}

uint16_t Sensor::readDistance() {
    VL53L0X_RangingMeasurementData_t measure;
    lox.rangingTest(&measure, false);
    
    if (measure.RangeStatus != 4) {
        return measure.RangeMilliMeter;
    } else {
        return 8190; // Fuera de rango
    }
}

// ---------------- UserDetector ----------------
UserDetector::UserDetector(Sensor& s1, Sensor& s2)
  : sensorA(s1), sensorB(s2) {}

void UserDetector::init() {
    Serial.println("\n👤 Inicializando UserDetector (2 sensores)...");
    
    // Inicializar sensor A
    sensorA.init();
    
    // Inicializar sensor B
    sensorB.init();
    
    Serial.println("✅ UserDetector inicializado correctamente\n");
}

bool UserDetector::detectUser() {
    unsigned long currentMillis = millis();
    Sensor& activeSensor = useFirstSensor ? sensorA : sensorB;

    uint16_t dist = activeSensor.readDistance();
    float deltaTime = (currentMillis - lastSwitchTime) / 1000.0;
    lastSwitchTime = currentMillis;

    // Alternar sensor
    useFirstSensor = !useFirstSensor;

    // Actualizar acumulador
    if (dist < 1000) { // 1 metro en mm
        detectionAccum += deltaTime;
        lastDetectionTime = currentMillis;
    } else if ((currentMillis - lastDetectionTime) > 3000) {
        detectionAccum = 0; // Reiniciar si no hay detección en 3s
    }

    // Retornar true si acumulación supera 2 segundos
    if (detectionAccum >= 2.0) {
        detectionAccum = 0; // Reset
        return true;
    }
    return false;
}

// ---------------- WasteDetector ----------------
WasteDetector::WasteDetector(Sensor& s) : sensor(s) {}

void WasteDetector::init() {
    Serial.println("Inicializando WasteDetector (1 sensor)...");
    
    // Inicializar sensor de residuo
    sensor.init();
    
    Serial.println("✅ WasteDetector inicializado correctamente\n");
}

bool WasteDetector::detectWaste() {
    uint16_t dist = sensor.readDistance();
    return dist < threshold;
}
