#include "Communication.h"
#include "Actuators.h"
#include "Sensors.h"

Communication comm;
Actuators actuators;
Sensors sensors;

int currentState = 0;  // 0: hibernación, 1: despertando, etc.
int residuoId = -1;

void setup() {
  Serial.begin(115200);
  actuators.init();
  sensors.init();
  comm.sendMessage("ARDUINO_READY");
}

void loop() {
  // 1️⃣ Revisar si hay comando entrante desde la RPi
  String msg = comm.readMessage();
  if (msg.length() > 0) handleMessage(msg);

  // 2️⃣ Lectura de sensores autónoma (si aplica)
  if (currentState == 0) {  // Hibernación
    if (sensors.detectUser()) comm.sendMessage("USER_DETECTED");
    if (sensors.detectWaste()) comm.sendMessage("WASTE_DETECTED");
  }

  delay(10);
}

void handleMessage(const String& msg) {
  if (msg.startsWith("ESTADO:")) {
    currentState = msg.substring(7).toInt();
    actuators.updateState(currentState);
    comm.sendMessage("OK_ESTADO");
  }

  else if (msg.startsWith("RESIDUO:")) {
    residuoId = msg.substring(8).toInt();
    actuators.segregateResidue(residuoId);
    comm.sendMessage("OK_RESIDUO");
  }

  else {
    comm.sendMessage("CMD_DESCONOCIDO");
  }
}
