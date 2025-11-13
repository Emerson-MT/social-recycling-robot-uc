#include "communication.h"
#include "actuators.h"
#include "sensor.h"
#include "recycling_robot.h"
#include "fsm.h"

// Configuración i2c para sensores
#define SDA_PIN 21
#define SCL_PIN 22
#define XSHUT_PIN_USER_LEFT 24  // pin para habilitar o apagar el sensor
#define XSHUT_PIN_USER_RIGHT 25  // pin para habilitar o apagar el sensor
#define XSHUT_PIN_WASTE 26  // pin para habilitar o apagar el sensor

// Configuración serial
#define BAUD_RATE 9600

// Configuración de los actuadores
// Luces
#define PIN_LED    18
#define NUM_LEDS   80
#define BRIGHT     127
// Servos
// Definimos los canales del PCA9685 donde están los servos
#define SERVO_1 7
#define SERVO_2 8

// Pines XSHUT
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
RobotSensors robotSensors = { userDetector, wasteDetector};

// Crear instancia de comunicación
Communication comm(BAUD_RATE);

// Crear instancia de actuadores
Actuators actuators(PIN_LED, NUM_LEDS, BRIGHT, SERVO_1, SERVO_2);

// Instancia global del robot que contiene todo
RecyclingRobot robot(comm, actuators, robotSensors);

// Instancia global de la FSM
FSM fsm(robot);

void setup() {
  robot.init();
}

void loop() {
  fsm.run();
}