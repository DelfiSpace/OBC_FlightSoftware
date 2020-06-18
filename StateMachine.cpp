/*
 * StateMachine.cpp
 *
 *  Created on: Jun 17, 2020
 *      Author: tom-h
 */

#ifndef STATEMACHINE_CPP_
#define STATEMACHINE_CPP_

#include "StateMachine.h"
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

    #ifdef DEBUG
    Console::log("switch entered");
    #endif
    switch(currentMode) {
        case ACTIVATION:
            #ifdef DEBUG
            Console::log("Activation mode case");
            #endif
            ActivationMode(containerPointer);
            break;
        case DEPLOYMENT:
            #ifdef DEBUG
            Console::log("deployment mode case");
            #endif
            DeployMode(containerPointer);
            //run deploy mode code
            break;
        case SAFE:
            //run safe code
            #ifdef DEBUG
            Console::log("safe mode case");
            #endif
            break;
        case ADCS:
            #ifdef DEBUG
            Console::log("adcs mode case");
            #endif
            //run ADCS code
            break;
        case NOMINAL:
            #ifndef DEBUG
            Console::log("nominal mode case");
            #endif
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
