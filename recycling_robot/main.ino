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

// Instancias globales de los componentes
Communication comm(BAUD_RATE);
Actuators actuators(PIN_LED, NUM_LEDS, BRIGHT, SERVO_1, SERVO_2);
Sensor user_left_sensor(SDA_PIN, SCL_PIN, XSHUT_PIN_USER_LEFT);
Sensor user_right_sensor(SDA_PIN, SCL_PIN, XSHUT_PIN_USER_RIGHT);
Sensor waste_sensor(SDA_PIN, SCL_PIN, XSHUT_PIN_WASTE);
// Instancia global del conjunto de sensores
RobotSensors robotSensors = { user_left_sensor, user_right_sensor, waste_sensor };
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