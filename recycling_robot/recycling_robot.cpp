#include "recycling_robot.h"


RecyclingRobot::RecyclingRobot(Communication& c, Actuators& a, RobotSensors& s)
  : comm(c), actuators(a), sensors(s) {}

void RecyclingRobot::init() {

  sensors.user_left.init();
  sensors.user_right.init();
  sensors.waste.init();

  actuators.init();
  comm.init(); // solo si lo implementas
}
