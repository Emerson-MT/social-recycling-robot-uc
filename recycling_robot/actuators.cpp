#include "Actuators.h"

void Actuators::init() {
  pinMode(ledPin, OUTPUT);
  servo.attach(servoPin);
}

void Actuators::updateState(int state) {
  switch (state) {
    case 0: digitalWrite(ledPin, LOW); break;        // Hibernación
    case 1: blinkSlow(); break;                      // Despertando
    case 2: digitalWrite(ledPin, HIGH); break;       // Clasificar
    case 3: blinkFast(); break;                      // Interacción
    case 4: blinkPattern(); break;                   // Agradecimiento
  }
}

void Actuators::segregateResidue(int id) {
  if (id == 0) servo.write(30);       // plástico
  else if (id == 1) servo.write(90);  // papel
  else if (id == 2) servo.write(150); // metal

  delay(1000);
  servo.write(90); // volver a posición neutral
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
