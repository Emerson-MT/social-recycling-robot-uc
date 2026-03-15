#include "fsm.h"

FSM::FSM(RecyclingRobot& robot)
  : robot(robot), currentState(0), residuoId(-1) {}

void FSM::run() {

  int newState;  // declarar antes del bucle

  do {
    newState = robot.readIntValue("ESTADO");  // retorna -2 si no hay nueva indicación de estado
  } while (newState == -2);

  if (newState != -2) {  // -2 = sin mensaje nuevo
    if (newState != currentState) {
      previousState = currentState;
      currentState = newState;
      firstEntry = true;  // acabamos de entrar al nuevo estado
      lightsOn = false;   // reiniciar persistentes
    }
  } else {
    firstEntry = false; // no entramos a un estado nuevo
  }
  
  switch (currentState) {
    case 0: stateHibernacion(); break;
    case 1: stateDespertando(); break;
    case 2: stateClasificar1(); break;
    case 3: stateClasificar2(); break;
    case 4: stateAgradecimiento(); break;
    default: break;
  }
}

// ======= ESTADOS =======

void FSM::stateHibernacion(){
  while(currentState != -1){
    if (robot.detectUser()){
      robot.sendMessage("USUARIO:1");
      currentState = -1;
    } 
    if (robot.detectWaste()){
      robot.sendMessage("RES_EN_POS:1");
      currentState = -1;
    }
  }
}

void FSM::stateDespertando() {

  static unsigned long startTime = 0;

  if (firstEntry) {
    startTime = millis();
    firstEntry = false;
  }

  robot.setLightsMode(1);  // 1 = dimming (parpadeo lento)

  // Verifica si se detectó el residuo
  if (robot.detectWaste()) {
    robot.sendMessage("RES_EN_POS:1");
    currentState = -1;
  }

  // Verifica si ya pasaron más de 3 segundos (3000 ms)
  if (millis() - startTime > 3000) {
    currentState = -1;
  }
}

void FSM::stateClasificar1() {

  if (firstEntry) {
    robot.setLightsMode(2);  // 2 = full (encendido completo inicial)
    lightsOn = true;
    stateEntryTime = millis();
    firstEntry = false;

    residuoId = robot.readIntValue("RESIDUO");
    if (residuoId != -1) {
      robot.setTargetBin(residuoId);
      
      // Ejecutar animación LED según tipo de residuo
      switch(residuoId) {
        case 0: robot.setLightsMode(0); break;   // HOME → OFF
        case 1: robot.setLightsMode(10); break;  // Cartón → Marrón
        case 2: robot.setLightsMode(7); break;   // Papel → Azul
        case 3: robot.setLightsMode(6); break;   // Plástico → Blanco
        case 4: robot.setLightsMode(8); break;   // General → Gris
        default: robot.setLightsMode(2); break;  // Default → Full
      }
    }
  }

  // Apagar luces después de animación completa
  if (lightsOn && millis() - stateEntryTime > 5000) {  // 5s para dar tiempo a animación
    robot.setLightsMode(0);  // 0 = off (apagado)
    lightsOn = false;
  }

  // Pasar al siguiente estado cuando termine el movimiento
  while (!robot.isMotionComplete()) {
    // mover servos de forma continua (no bloqueante)
    robot.moveAllServosSmooth();
  }

  delay(2000);
  robot.setTargetBin(0);

  // Pasar al siguiente estado cuando termine el movimiento
  while (!robot.isMotionComplete()) {
    // mover servos de forma continua (no bloqueante)
    robot.moveAllServosSmooth();
  }

  currentState = -1;
}


void FSM::stateClasificar2() {

  if (firstEntry) {
    robot.setLightsMode(2);  // 2 = full (encendido completo inicial)
    lightsOn = true;
    stateEntryTime = millis();
    firstEntry = false;

    do{
      residuoId = robot.readIntValue("RESIDUO");
    }while(residuoId==-2);

    if (residuoId != -1) {
      robot.setTargetBin(residuoId);
      
      // Ejecutar animación LED según tipo de residuo
      switch(residuoId) {
        case 0: robot.setLightsMode(0); break;   // HOME → OFF
        case 1: robot.setLightsMode(10); break;  // Cartón → Marrón
        case 2: robot.setLightsMode(7); break;   // Papel → Azul
        case 3: robot.setLightsMode(6); break;   // Plástico → Blanco
        case 4: robot.setLightsMode(8); break;   // General → Gris
        default: robot.setLightsMode(2); break;  // Default → Full
      }
    }
  }
  
  // Apagar luces después de animación completa
  if (lightsOn && millis() - stateEntryTime > 5000) {  // 5s para dar tiempo a animación
    robot.setLightsMode(0);  // 0 = off (apagado)
    lightsOn = false;
  }
  
  // Pasar al siguiente estado cuando termine el movimiento
  while (!robot.isMotionComplete()) {
    // mover servos de forma continua (no bloqueante)
    robot.moveAllServosSmooth();
  }

  delay(2000);
  robot.setTargetBin(0);

  // Pasar al siguiente estado cuando termine el movimiento
  while (!robot.isMotionComplete()) {
    // mover servos de forma continua (no bloqueante)
    robot.moveAllServosSmooth();
  }

  currentState = -1;
}

void FSM::stateAgradecimiento() {
  if (firstEntry) {
    robot.setLightsMode(4);  // 4 = byebye (patrón de despedida)
    firstEntry = false;
  }
}