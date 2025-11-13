#include "recycling_robot.h"

RecyclingRobot::RecyclingRobot(Communication& c,
                               ServoController& s,
                               LEDController& l,
                               RobotSensors& rs)
    : comm(c), servos(s), leds(l), sensors(rs) {}

void RecyclingRobot::init() {
    Wire.begin(21, 22); // SDA, SCL

    // Inicializar sensores
    if (sensors.userdetector) sensors.userdetector->init();
    if (sensors.wastedetector) sensors.wastedetector->init();

    // Inicializar actuadores y comunicación
    servos.init();
    leds.init();
    comm.init();
}

// Métodos de conveniencia para sensores
bool RecyclingRobot::detectUser() {
    return sensors.userdetector ? sensors.userdetector->detectUser() : false;
}

bool RecyclingRobot::detectWaste() {
    return sensors.wastedetector ? sensors.wastedetector->detectWaste() : false;
}

// Métodos de conveniencia para leds
void RecyclingRobot::setLightsMode(int mode) {
    leds.setMode(mode);
}

// Métodos de conveniencia para servos
void RecyclingRobot::moveAllServosSmooth() {
    servos.moveAllServosSmooth();
}

bool RecyclingRobot::isMotionComplete() {
    return servos.isMotionComplete();
}

void Recycling_robot::setTargetAngles(int t){
    servos.setTargetAngles(t);
}

// // Métodos de conveniencia para comunicación
void RecyclingRobot::sendMessage(const String& msg) {
    comm.sendMessage(msg);
}

int RecyclingRobot::readIntValue(const String& prefix) {
    return comm.readIntValue(prefix);
}