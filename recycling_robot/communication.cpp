#include "Communication.h"

Communication::Communication(int br)
  : baud_rate(br){}

void Communication::init(){
  Serial.begin(baud_rate);
  delay(200);
}

void Communication::sendMessage(const String& msg) {
  Serial.println(msg);
}

int Communication::readIntValue(const String& expectedPrefix) {
  static String buffer = "";

  // Leer todos los caracteres disponibles
  while (Serial.available()) {
    char c = Serial.read();

    // Fin de línea = fin de mensaje
    if (c == '\n') {
      String message = buffer;
      buffer = "";  // limpiar para siguiente mensaje
      message.trim();

      // Validar formato PREFIX:valor
      int separatorIndex = message.indexOf(':');
      if (separatorIndex != -1) {
        String prefix = message.substring(0, separatorIndex);
        int value = message.substring(separatorIndex + 1).toInt();

        // Si coincide con lo que esperamos, devolver valor
        if (prefix == expectedPrefix) {
          return value;
        }
      }

      // Si el formato no coincide o el prefijo no es el esperado
      return -1;
    } 
    else if (c != '\r') {
      buffer += c;
    }
  }

  // Si no había nada que leer
  return -2;
}


