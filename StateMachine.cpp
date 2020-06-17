/*
 * StateMachine.cpp
 *
 *  Created on: Jun 17, 2020
 *      Author: tom-h
 */

#ifndef STATEMACHINE_CPP_
#define STATEMACHINE_CPP_

#include "StateMachine.h"
//#include "ActivationMode.h"
#include "Console.h"

/**
 *
 *   Define which state it should be and call relative functions.
 *   Override run() in Task.h
 *
 *   Parameters:
 *
 *   Returns:
 *
 */
void StateMachine::run() {

    //put TDEM code here

    switch(currentMode) {
        case ACTIVATION:
            //ActivationMode();
            break;
        case DEPLOYMENT:
            //run safe mode code
            break;
        case SAFE:
            //run deployment code
            DeployMode();
            break;
        case ADCS:
            //run ADCS code
            break;
        case NOMINAL:
            //run nominal mode code
            break;
     }
}

/**
 *
 *   Override notified() in Task.h so StateMachine is notified in every loop!
 *
 *   Parameters:
 *
 *   Returns:
 *      true (1)
 */
bool StateMachine::notified() {
    return true;
}

/**
 *
 *   Initial setting. Override setUp() in Task.h
 *
 *   Parameters:
 *
 *   Returns:
 *
 */
void StateMachine::setUp() {

    Console::log("Statemachine setup called");

    currentMode = ACTIVATION; // initial mode
    upTime = 0;
    totalUpTime = 0; // TODO
    OBCBootCount = 1; // TODO

    // TODO: load totalUpTime & OBCBootCount from FRAM
}



#endif /* STATEMACHINE_CPP_ */
