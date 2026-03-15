#ifndef FSM_H
#define FSM_H

#include <Arduino.h>
#include "recycling_robot.h"

class FSM {
  public:
    FSM(RecyclingRobot& robot); //Cambie el Recycling_robot por RecyclingRobot (revisar recycling_robot.h)

    void run();  // Se llama en loop()

  private:

    RecyclingRobot& robot; //Cambie el Recycling_robot por RecyclingRobot (revisar recycling_robot.h)

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