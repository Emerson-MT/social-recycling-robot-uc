#ifndef RECYCLING_ROBOT_H
#define RECYCLING_ROBOT_H

#include <Arduino.h>
#include "communication.h"
#include "actuators.h"
#include "sensor.h"
#include <Wire.h>

// Estructura que agrupa los sensores del robot
struct RobotSensors {
  UserDetector& userdetector;
  WasteDetector& wastedetector;
};

class RecyclingRobot {
  public:
    RecyclingRobot(Communication& comm, Actuators& actuators, RobotSensors& sensors);
    void init();

  private:
    Communication& comm;
    Actuators& actuators;
    RobotSensors& sensors
};

#endif