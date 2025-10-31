  #include "fsm.h"

  FSM::FSM(Recycling_robot& robot)
    : robot(robot), currentState(0), residuoId(-1) {}

  void FSM::run() {
    int newState = robot.comm.readIntValue("ESTADO"); // retorna -2 si no hay nueva indicación de estado

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
    if (robot.sensors.detectUser()){
      robot.comm.sendMessage("USUARIO:1");
      currentState = -1;
    } 
    if (robot.sensors.detectWaste()){
      robot.comm.sendMessage("RES_EN_POS:1");
      currentState = -1;
    }
  }

  void FSM::stateDespertando() {

    static unsigned long startTime = 0;

    if (firstEntry) {
      startTime = millis();
      firstEntry = false;
    }

    robot.actuators.setLightsMode('dimming');

    // Verifica si se detectó el residuo
    if (robot.sensors.detectWaste()) {
      robot.comm.sendMessage("RES_EN_POS:1");
      currentState = -1;
    }

    // Verifica si ya pasaron más de 3 segundos (3000 ms)
    if (millis() - startTime > 3000) {
      currentState = -1;
    }
  }

  void FSM::stateClasificar1() {

    if (firstEntry) {
      robot.actuators.setLightsMode('full');
      lightsOn = true;
      stateEntryTime = millis();  // guardamos el momento en que entró
      firstEntry = false;  // <-- importante

      residuoId = robot.comm.readIntValue("RESIDUO");
      if (residuoId != -1) {
        robot.actuators.setTargetAngles(residuoId);
      }
    }

    // mover servos de forma continua (no bloqueante)
    robot.actuators.moveServoSmooth(robot.actuators.servo1Channel);
    robot.actuators.moveServoSmooth(robot.actuators.servo2Channel);

    // Apagar luces después de 3 segundos sin bloquear
    if (lightsOn && millis() - stateEntryTime > 3000) {
      robot.actuators.setLightsMode('off');
      lightsOn = false;
    }

    // Pasar al siguiente estado cuando termine el movimiento
    if (robot.actuators.isMotionComplete()) {
      currentState = -1;
    }
  }


  void FSM::stateClasificar2() {

    if (firstEntry) {
      robot.actuators.setLightsMode('full');
      lightsOn = true;
      stateEntryTime = millis();  // guardamos el momento en que entró
      firstEntry = false;  // <-- importante
    }

    // Apagar luces después de 3 segundos sin bloquear
    if (lightsOn && millis() - stateEntryTime > 3000) {
      robot.actuators.setLightsMode('off');
      lightsOn = false;
    }

    residuoId = robot.comm.readIntValue("RESIDUO");

    if (residuoId != -1) {
      robot.actuators.sortWaste(residuoId);
      currentState = -1; // listo para volver a estado neutro o esperar otro comando
    }
  }

  void FSM::stateAgradecimiento() {
    if (firstEntry) {
      robot.actuators.setLightsMode('byebye');
      firstEntry = false;
    }
  }
