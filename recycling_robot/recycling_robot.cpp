#include "recycling_robot.h"


RecyclingRobot::RecyclingRobot(Communication& c, Actuators& a, RobotSensors& s)
  : comm(c), actuators(a), sensors(s) {}

void RecyclingRobot::init() {

  sensors.userdetector.init();
  sensors.wastedetector.init();
  actuators.init();
  comm.init(); 
  Wire.begin(21, 22);
}
