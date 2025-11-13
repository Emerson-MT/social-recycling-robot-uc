#include "fsm.h"

FSM::FSM(Recycling_robot& robot)
  : robot(robot), currentState(0), residuoId(-1) {}

void FSM::run() {
  int newState = robot.readIntValue("ESTADO"); // retorna -2 si no hay nueva indicación de estado

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
  if (robot.detectUser()){
    robot.sendMessage("USUARIO:1");
    currentState = -1;
  } 
  if (robot.detectWaste()){
    robot.sendMessage("RES_EN_POS:1");
    currentState = -1;
  }
}

void FSM::stateDespertando() {

  static unsigned long startTime = 0;

  if (firstEntry) {
    startTime = millis();
    firstEntry = false;
  }

  robot.setLightsMode('dimming');

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
    robot.setLightsMode('full');
    lightsOn = true;
    stateEntryTime = millis();  // guardamos el momento en que entró
    firstEntry = false;  // <-- importante

    residuoId = robot.readIntValue("RESIDUO");
    if (residuoId != -1) {
      robot.setTargetAngles(residuoId);
    }
  }

  // mover servos de forma continua (no bloqueante)
  robot.moveAllServosSmooth();

  // Apagar luces después de 3 segundos sin bloquear
  if (lightsOn && millis() - stateEntryTime > 3000) {
    robot.setLightsMode('off');
    lightsOn = false;
  }

  // Pasar al siguiente estado cuando termine el movimiento
  if (robot.isMotionComplete()) {
    currentState = -1;
  }
}


void FSM::stateClasificar2() {

  if (firstEntry) {
    robot.setLightsMode('full');
    lightsOn = true;
    stateEntryTime = millis();  // guardamos el momento en que entró
    firstEntry = false;  // <-- importante

    residuoId = robot.readIntValue("RESIDUO");
    if (residuoId != -1) {
      robot.setTargetAngles(residuoId);
    }
  }

  // mover servos de forma continua (no bloqueante)
  robot.moveAllServosSmooth();

  // Apagar luces después de 3 segundos sin bloquear
  if (lightsOn && millis() - stateEntryTime > 3000) {
    robot.setLightsMode('off');
    lightsOn = false;
  }

  // Pasar al siguiente estado cuando termine el movimiento
  if (robot.isMotionComplete()) {
    currentState = -1;
  }
}

void FSM::stateAgradecimiento() {
  if (firstEntry) {
    robot.setLightsMode('byebye');
    firstEntry = false;
  }
}
