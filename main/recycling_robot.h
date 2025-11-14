#ifndef RECYCLING_ROBOT_H
#define RECYCLING_ROBOT_H

#include <Arduino.h>
#include "communication.h"
#include "servo_controller.h"
#include "led_controller.h"
#include "sensor.h"
#include <Wire.h>

// Estructura que agrupa los sensores del robot
struct RobotSensors {
    UserDetector* userdetector;
    WasteDetector* wastedetector;
};

class RecyclingRobot {
public:
    RecyclingRobot(Communication& comm,
                   ServoController& servos,
                   LEDController& leds,
                   RobotSensors& sensors);

    void init();

    // Métodos de conveniencia para los sensores
    bool detectUser();
    bool detectWaste();

    // Métodos de conveniencia para las luces
    void setLightsMode(int mode);

    // Métodos de conveniencia para los servos
    void moveAllServosSmooth();
    bool isMotionComplete();
    void setTargetAngles(int t);

    // Métodos de conveniencia para comunicación
    void sendMessage(const String& msg);
    int readIntValue(const String& prefix);

private:
    Communication& comm;
    ServoController& servos;
    LEDController& leds;
    RobotSensors& sensors;
};

#endif
