#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <Arduino.h>

class Communication {
public:
  Communication(int br);
  void init();
  void sendMessage(const String& msg);
  int readIntValue(const String& expectedPrefix);

  int baud_rate;
};

#endif
