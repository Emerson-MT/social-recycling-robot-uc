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

void RecyclingRobot::setTargetBin(int binId){
    // Convertir el ID numérico a BinTarget enum
    BinTarget target;
    switch(binId) {
        case 0: target = BIN_HOME; break;
        case 1: target = BIN_1; break;    // Cartón
        case 2: target = BIN_2; break;    // Papel
        case 3: target = BIN_3; break;    // Plástico
        case 4: target = BIN_4; break;    // Residuo general
        default: 
            Serial.println("⚠ BinID inválido, usando HOME");
            target = BIN_HOME; 
            break;
    }
    servos.setTargetBin(target);
}

// // Métodos de conveniencia para comunicación
void RecyclingRobot::sendMessage(const String& msg) {
    comm.sendMessage(msg);
}

int RecyclingRobot::readIntValue(const String& prefix) {
    return comm.readIntValue(prefix);
}
