/*
 * MAPA DE CONEXIONES ESP32
 * 
 * === COMUNICACIÓN I2C ===
 * GPIO 21 (SDA) -> PCA9685 SDA, VL53L0X (todos) SDA
 * GPIO 22 (SCL) -> PCA9685 SCL, VL53L0X (todos) SCL
 * 
 * === LEDs ===
 * GPIO 32 -> LED_DATA (WS2812B - 80 LEDs direccionables)
 * 
 * === SENSORES VL53L0X (XSHUT) ===
 * GPIO 13 -> XSHUT sensor usuario 1
 * GPIO 14 -> XSHUT sensor usuario 2  (DEBE ser 14 según requerimiento)
 * GPIO 25 -> XSHUT sensor residuo
 * 
 * === SERVOS (Conectados al PCA9685, NO directamente al ESP32) ===
 * Canal 0 (PCA9685) -> Servo Base Rotación (BR)
 * Canal 1 (PCA9685) -> Servo Base Inclinación (BI)
 * 
 * NOTA CRÍTICA: Los servos se conectan al PCA9685, que se comunica
 * con el ESP32 por I2C (GPIO 21/22). Los números 0 y 1 son CANALES
 * del PCA9685, NO pines GPIO del ESP32.
 */

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

// Servos - CANALES del PCA9685 (NO son pines GPIO del ESP32)
#define SERVO_1 0    // Canal 0 del PCA9685 (Base Rotación)
#define SERVO_2 1    // Canal 1 del PCA9685 (Base Inclinación)

// Pines XSHUT sensores VL53L0X (SÍ son pines GPIO del ESP32)
#define XSHUT_USER1 13
#define XSHUT_USER2 14  // DEBE ser 14 (requerimiento)
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
ServoController servos(SERVO_1, SERVO_2);  // Canales 0 y 1 del PCA9685

// Instancia global del robot
RecyclingRobot robot(comm, servos, leds, robotSensors);

// Instancia global de la FSM
FSM fsm(robot);

void setup() {
    Serial.begin(9600);
    Serial.println("Iniciando sistema...");
    robot.init();
    Serial.println("Sistema listo!");
}

void loop() {
    fsm.run();
}
