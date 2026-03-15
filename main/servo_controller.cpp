#include "servo_controller.h"

// ========== IMPLEMENTACIÓN DE ARTICULACIÓN ==========

Articulacion::Articulacion(uint8_t _canal) {
    canal = _canal;
    posicionActual = 90.0;
    posicionObjetivo = 90.0;
    velocidad = 50;  // Velocidad media por defecto
    ultimoTiempo = 0;
}

void Articulacion::setObjetivo(float nuevoAngulo, uint8_t nuevaVelocidad) {
    posicionObjetivo = constrain(nuevoAngulo, 0.0, 180.0);
    velocidad = constrain(nuevaVelocidad, 1, 100);
    
    #ifdef DEBUG_SERVO
    Serial.print("[CANAL "); 
    Serial.print(canal); 
    Serial.print("] OBJ-> "); 
    Serial.print(posicionObjetivo); 
    Serial.print("° vel=");
    Serial.println(velocidad);
    #endif
}

bool Articulacion::enMovimiento() {
    return fabs(posicionActual - posicionObjetivo) > 0.5;
}

void Articulacion::actualizar() {
    unsigned long ahora = millis();
    
    // Cadencia alta pero controlada (cada 12ms como en Articulaciones.ino)
    if (ahora - ultimoTiempo < 12) return;
    ultimoTiempo = ahora;

    if (!enMovimiento()) return;

    // Paso en grados por actualización
    // velocidad 1 -> 0.02° por tick, 100 -> 2° por tick
    float paso = velocidad * 0.02;

    if (posicionActual < posicionObjetivo) {
        posicionActual += paso;
        if (posicionActual > posicionObjetivo) 
            posicionActual = posicionObjetivo;
    } else {
        posicionActual -= paso;
        if (posicionActual < posicionObjetivo) 
            posicionActual = posicionObjetivo;
    }

    posicionActual = constrain(posicionActual, 0.0, 180.0);
}

// ========== IMPLEMENTACIÓN DE SERVOCONTROLLER ==========

ServoController::ServoController(int baseRotCh, int baseIncCh)
    : baseRotChannel(baseRotCh), baseIncChannel(baseIncCh) {
    
    baseRotacion = new Articulacion(baseRotCh);
    baseInclinacion = new Articulacion(baseIncCh);
    
    currentSequence = SEQ_IDLE;
    targetBin = BIN_HOME;
}

void ServoController::init() {
    pca.begin();
    pca.setPWMFreq(50);  // 50Hz para servos
    delay(300);

    // Inicializar ambas articulaciones en posición HOME (90°)
    int pulsoInicial = angleToPulse(90.0);
    
    pca.setPWM(baseRotChannel, 0, pulsoInicial);
    pca.setPWM(baseIncChannel, 0, pulsoInicial);
    
    baseRotacion->posicionActual = 90.0;
    baseRotacion->posicionObjetivo = 90.0;
    baseInclinacion->posicionActual = 90.0;
    baseInclinacion->posicionObjetivo = 90.0;
    
    delay(1000);
    
    Serial.println("✅ ServoController inicializado (2 articulaciones)");
}

int ServoController::angleToPulse(float ang) {
    return (int)map((long)ang, 0, 180, SERVOMIN, SERVOMAX);
}

void ServoController::setTargetBin(BinTarget bin) {
    targetBin = bin;
    startSequenceToBin(bin);
}

void ServoController::startSequenceToBin(BinTarget bin) {
    currentSequence = SEQ_ROTATE_FIRST;
    
    float rotacionObjetivo = 90.0;
    float inclinacionObjetivo = 90.0;
    
    // Determinar ángulos según el contenedor
    switch(bin) {
        case BIN_HOME:
            rotacionObjetivo = 90.0;
            inclinacionObjetivo = 90.0;
            break;
            
        case BIN_1:  // Cartón: BR=45°, BI=45°
            rotacionObjetivo = 45.0;
            inclinacionObjetivo = 45.0;
            break;
            
        case BIN_2:  // Papel: BR=135°, BI=45°
            rotacionObjetivo = 135.0;
            inclinacionObjetivo = 45.0;
            break;
            
        case BIN_3:  // Plástico: BR=45°, BI=135°
            rotacionObjetivo = 45.0;
            inclinacionObjetivo = 135.0;
            break;
            
        case BIN_4:  // Residuo general: BR=135°, BI=135°
            rotacionObjetivo = 135.0;
            inclinacionObjetivo = 135.0;
            break;
            
        default:
            Serial.println("⚠ Contenedor inválido, usando HOME");
            rotacionObjetivo = 90.0;
            inclinacionObjetivo = 90.0;
            break;
    }
    
    // IMPORTANTE: Primero rotar la base, luego inclinar
    // Esto evita colisiones mecánicas
    baseRotacion->setObjetivo(rotacionObjetivo, 50);  // Velocidad media
    
    // La inclinación se establecerá cuando termine la rotación
    // (se maneja en processSequence)
    
    #ifdef DEBUG_SERVO
    Serial.print("Secuencia a BIN ");
    Serial.print((int)bin);
    Serial.print(": BR->");
    Serial.print(rotacionObjetivo);
    Serial.print("° BI->");
    Serial.print(inclinacionObjetivo);
    Serial.println("°");
    #endif
}

void ServoController::processSequence() {
    if (currentSequence == SEQ_IDLE) return;
    
    float rotacionObjetivo = 90.0;
    float inclinacionObjetivo = 90.0;
    
    // Determinar ángulos según el contenedor objetivo
    switch(targetBin) {
        case BIN_HOME:
            rotacionObjetivo = 90.0;
            inclinacionObjetivo = 90.0;
            break;
        case BIN_1:
            rotacionObjetivo = 45.0;
            inclinacionObjetivo = 45.0;
            break;
        case BIN_2:
            rotacionObjetivo = 135.0;
            inclinacionObjetivo = 45.0;
            break;
        case BIN_3:
            rotacionObjetivo = 45.0;
            inclinacionObjetivo = 135.0;
            break;
        case BIN_4:
            rotacionObjetivo = 135.0;
            inclinacionObjetivo = 135.0;
            break;
    }
    
    // Secuencia: primero rotar, luego inclinar
    if (currentSequence == SEQ_ROTATE_FIRST) {
        if (!baseRotacion->enMovimiento()) {
            // Rotación completa, ahora inclinar
            baseInclinacion->setObjetivo(inclinacionObjetivo, 50);
            currentSequence = SEQ_INCLINE_SECOND;
        }
    }
    else if (currentSequence == SEQ_INCLINE_SECOND) {
        if (!baseInclinacion->enMovimiento()) {
            // Secuencia completa
            currentSequence = SEQ_IDLE;
            
            #ifdef DEBUG_SERVO
            Serial.print("✓ Secuencia a BIN ");
            Serial.print((int)targetBin);
            Serial.println(" completada");
            #endif
        }
    }
}

void ServoController::moveAllServosSmooth() {
    // Actualizar posiciones de ambas articulaciones
    baseRotacion->actualizar();
    baseInclinacion->actualizar();
    
    // Enviar los pulsos PWM al PCA9685
    int pulsoRot = angleToPulse(baseRotacion->posicionActual);
    int pulsoInc = angleToPulse(baseInclinacion->posicionActual);
    
    pca.setPWM(baseRotChannel, 0, pulsoRot);
    pca.setPWM(baseIncChannel, 0, pulsoInc);
    
    // Procesar secuencias activas
    processSequence();
}

bool ServoController::isMotionComplete() {
    // Considera completo cuando:
    // 1. Ambas articulaciones llegaron a su objetivo
    // 2. No hay secuencia activa
    return (!baseRotacion->enMovimiento() && 
            !baseInclinacion->enMovimiento() && 
            currentSequence == SEQ_IDLE);
}

void ServoController::returnHome() {
    setTargetBin(BIN_HOME);
}
