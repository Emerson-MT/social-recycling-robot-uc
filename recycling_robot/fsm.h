#ifndef FSM_H
#define FSM_H

#include <Arduino.h>
#include "Communication.h"
#include "Actuators.h"
#include "Sensors.h"

class FSM {
  public:
    FSM(Communication& comm, Actuators& actuators, Sensors& sensors);

    void run();  // Se llama en loop()
    void handleEstado(int nuevoEstado);
    void handleResiduo(int nuevoResiduo);

  private:
    Communication& comm;
    Actuators& actuators;
    Sensors& sensors;

    int currentState;
    int residuoId;

    void stateHibernacion();
    void stateDespertando();
    void stateClasificar1();
    void stateClasificar2();
    void stateAgradecimiento();
};

#endif
