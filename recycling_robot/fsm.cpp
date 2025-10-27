#include "FSM.h"

FSM::FSM(Communication& c, Actuators& a, Sensors& s)
  : comm(c), actuators(a), sensors(s), currentState(0), residuoId(-1) {}

void FSM::run() {
  switch (currentState) {
    case 0: stateHibernacion(); break;
    case 1: stateDespertando(); break;
    case 2: stateClasificar1(); break;
    case 3: stateClasificar2(); break;
    case 4: stateAgradecimiento(); break;
    default: break;
  }
}

void FSM::handleEstado(int nuevoEstado) {
  currentState = nuevoEstado;
  comm.sendMessage("OK_ESTADO");
}

void FSM::handleResiduo(int nuevoResiduo) {
  residuoId = nuevoResiduo;
  comm.sendMessage("OK_RESIDUO");
  actuators.segregateResidue(residuoId);
}

// ======= ESTADOS =======

void FSM::stateHibernacion() {
  if (sensors.detectUser()) comm.sendMessage("USER_DETECTED");
  if (sensors.detectWaste()) comm.sendMessage("WASTE_DETECTED");
}

void FSM::stateDespertando() {
  actuators.blinkSoftly();
}

void FSM::stateClasificar1() {
  // Ejemplo: encender luces y preparar segregación
  actuators.showReadyPattern();
}

void FSM::stateClasificar2() {
  actuators.playInteractionPattern();
}

void FSM::stateAgradecimiento() {
  actuators.playGratefulPattern();
}
