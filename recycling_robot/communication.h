#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <Arduino.h>

class Communication {
public:
  void sendMessage(const String& msg);
  String readMessage();

};

#endif