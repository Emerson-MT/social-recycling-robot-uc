#include "Actuators.h"

// Definición de las constantes estáticas de color (una sola vez)
const CRGB Actuators::VERDE_AMAR = CRGB(180, 255, 0);
const CRGB Actuators::ROJO       = CRGB(255, 0, 0);
const CRGB Actuators::BLANCO     = CRGB(255, 255, 255);

Actuators::Actuators(int lp, int nl, int b, int s1c, int s2c)
  : ledPin(lp), numLeds(nl), bright(b), servo1Channel(s1c), servo2Channel(s2c) {
    // Construcción de los leds
    leds = new CRGB[numLeds];
    // Inicialización de otras variables;
    currentAngle[servo1Channel] = targetAngle[servo1Channel] = 90;
    currentAngle[servo2Channel] = targetAngle[servo2Channel] = 90;
    speedPercent[servo1Channel] = speedPercent[servo2Channel] = 100;
  }

void Actuators::init() {

  // LEDs
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 1500);
  FastLED.addLeds<WS2812B, PIN_LED, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(bright);
  FastLED.clear(true);

  // Servos 
  pca.begin();
  pca.setPWMFreq(50); // 50 Hz para servos estándar

  pca.setPWM(servo1Channel, 0, angleToPulse(90));
  pca.setPWM(servo2Channel, 0, angleToPulse(90));


  delay(1000);
}

int Actuators::angleToPulse(int ang) {
  return map(ang, 0, 180, SERVOMIN, SERVOMAX);
}

void Actuators::setTargetAngles(int t) {
  switch (t) {
    case 0:
      targetAngle1 = 45; targetAngle2 = 45; // Tacho 1: Cartón
      break;
    case 1:
      targetAngle1 = 135; targetAngle2 = 45; // Tacho 2: Papel
      break;
    case 2:
      targetAngle1 = 45; targetAngle2 = 135; // Tacho 3: Plástico
      break;
    case 3:
      targetAngle1 = 135; targetAngle2 = 135; // Tacho 4: Residuo General
      break;
    case 4:
      targetAngle1 = 90; targetAngle2 = 90; // Posición por defecto
      break;
    default:
      Serial.println(F("⚠ Tacho inválido (0-4)"));
      return;
  }

  Serial.print(F("Tacho seleccionado: "));
  Serial.println(t);
}

void Actuators::moveServoSmooth(int canal) {
  static unsigned long lastMoveTime[16] = {0}; // hasta 16 canales PCA9685 posibles

  if (currentAngle[canal] == targetAngle[canal]) return;

  int delayTime = map(100 - speedPercent[canal], 0, 100, 0, 50);

  // Solo mover si ha pasado el tiempo correspondiente
  if (millis() - lastMoveTime[canal] >= (unsigned long)delayTime) {
    lastMoveTime[canal] = millis();

    if (currentAngle[canal] < targetAngle[canal]) currentAngle[canal]++;
    else if (currentAngle[canal] > targetAngle[canal]) currentAngle[canal]--;

    currentAngle[canal] = constrain(currentAngle[canal], 0, 180);
    pca.setPWM(canal, 0, angleToPulse(currentAngle[canal]));
  }
}

bool Actuators::isMotionComplete() {
  return (currentAngle[servo1Channel] == targetAngle[servo1Channel] &&
          currentAngle[servo2Channel] == targetAngle[servo2Channel]);
}

void Actuators::setLightsMode(String mode) {
  switch (mode) {
    case 0: digitalWrite(ledPin, LOW); break;        // Hibernación
    case 1: blinkSlow(); break;                      // Despertando
    case 2: digitalWrite(ledPin, HIGH); break;       // Clasificar
    case 3: blinkFast(); break;                      // Interacción
    case 4: blinkPattern(); break;                   // Agradecimiento
  }
}

void Actuators::blinkSlow() {
  digitalWrite(ledPin, (millis() / 1000) % 2);
}

void Actuators::blinkFast() {
  digitalWrite(ledPin, (millis() / 200) % 2);
}

void Actuators::blinkPattern() {
  static unsigned long last = 0;
  if (millis() - last > 300) {
    digitalWrite(ledPin, !digitalRead(ledPin));
    last = millis();
  }
}
