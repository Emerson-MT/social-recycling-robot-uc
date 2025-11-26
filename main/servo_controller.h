#ifndef SERVO_CONTROLLER_H
#define SERVO_CONTROLLER_H

#include <Adafruit_PWMServoDriver.h>
#include <Arduino.h>

#define SERVOMIN 110
#define SERVOMAX 500

// BINS
enum BinTarget {
    BIN_HOME = 0,   // Posición inicial/neutral (90°, 90°)
    BIN_1 = 1,      // Cartón: BR=45°, BI=45°
    BIN_2 = 2,      // Papel: BR=135°, BI=45°
    BIN_3 = 3,      // Plástico: BR=45°, BI=135°
    BIN_4 = 4       // Residuo general: BR=135°, BI=135°
};

// --- Clase Articulación (Joint) ---
class Articulacion {
public:
    uint8_t canal;
    float posicionActual;
    float posicionObjetivo;
    uint8_t velocidad;           // 1..100
    unsigned long ultimoTiempo;

    Articulacion(uint8_t _canal = 0);
    
    void setObjetivo(float nuevoAngulo, uint8_t nuevaVelocidad);
    bool enMovimiento();
    void actualizar();
};


class ServoController {
public:
    ServoController(int baseRotChannel, int baseIncChannel);
    
    void init();
    void setTargetBin(BinTarget bin);
    void moveAllServosSmooth();
    bool isMotionComplete();
    void returnHome();
    
    // Métodos adicionales para compatibilidad
    int angleToPulse(float ang);

private:
    Adafruit_PWMServoDriver pca;
    
    // Dos articulaciones principales
    Articulacion* baseRotacion;      // BR - Base rotación
    Articulacion* baseInclinacion;   // BI - Base inclinación
    
    // Canales
    int baseRotChannel;
    int baseIncChannel;
    
    // Control de secuencias
    enum SequenceState {
        SEQ_IDLE,
        SEQ_ROTATE_FIRST,
        SEQ_INCLINE_SECOND
    };
    
    SequenceState currentSequence;
    BinTarget targetBin;
    
    // Métodos privados
    void startSequenceToBin(BinTarget bin);
    void processSequence();
};

#endif
