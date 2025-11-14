#include "communication.h"
#include "servo_controller.h"
#include "led_controller.h"
#include "sensor.h"
#include "recycling_robot.h"
#include "fsm.h"

// Configuración serial
#define BAUD_RATE 9600

// Configuración LEDs
#define PIN_LED    32
#define NUM_LEDS   80
#define BRIGHT     127

// Servos PCA9685
#define SERVO_1 14
#define SERVO_2 15

// Pines XSHUT sensores
#define XSHUT_USER1 13
#define XSHUT_USER2 14
#define XSHUT_WASTE 25

// Crear instancias de sensores
Sensor sensorUser1(XSHUT_USER1, 0x30);
Sensor sensorUser2(XSHUT_USER2, 0x31);
Sensor sensorWaste(XSHUT_WASTE, 0x32);

// Detectores
UserDetector userDetector(sensorUser1, sensorUser2);
WasteDetector wasteDetector(sensorWaste);

// Instancia global del conjunto de sensores
RobotSensors robotSensors = {&userDetector, &wasteDetector};

// Crear instancia de comunicación
Communication comm(BAUD_RATE);

// Crear instancias de actuadores
LEDController leds(PIN_LED, NUM_LEDS, BRIGHT);
ServoController servos(SERVO_1, SERVO_2);

// Instancia global del robot
RecyclingRobot robot(comm, servos, leds, robotSensors);

// Instancia global de la FSM
FSM fsm(robot);

void setup() {
    robot.init();
}

void loop() {
    fsm.run();
}
