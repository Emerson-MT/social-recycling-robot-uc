#include "Communication.h"

void Communication::sendMessage(const String& msg) {
  Serial.println(msg);
}

String Communication::readMessage() {
  static String input = "";
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n') {
      String temp = input;
      input = "";
      return temp;
    } else if (c != '\r') {
      input += c;
    }
  }
  return "";
}
void Communication::processMessage(const String& message) {
  if (message.startsWith("ESTADO:")) {
    int estado = message.substring(7).toInt();
    handleEstado(estado);
  } 
  else if (message.startsWith("RESIDUO:")) {
    int residuo = message.substring(8).toInt();
    handleResiduo(residuo);
  }
  else {
    Serial.println("Comando no reconocido");
  }
}

