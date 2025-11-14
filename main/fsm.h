#ifndef FSM_H
#define FSM_H

#include <Arduino.h>
#include "recycling_robot.h"

class FSM {
  public:
    FSM(Recycling_robot& robot);

    void run();  // Se llama en loop()

  private:

    Recycling_robot& robot;

    int currentState;
    int previousState = -1;
    int residuoId;

    bool lightsOn = false;
    bool firstEntry = true;
    unsigned long stateEntryTime = 0;

    void stateHibernacion();
    void stateDespertando();
    void stateClasificar1();
    void stateClasificar2();
    void stateAgradecimiento();
};

#endif
