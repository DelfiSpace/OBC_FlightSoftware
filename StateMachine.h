/*
 * StateMachine.h
 *
 *  Created on: Jun 17, 2020
 *      Author: tom-h
 */


#ifndef STATEMACHINE_H_
#define STATEMACHINE_H_

#include "Task.h"
#include "OBCDataContainer.h"
#include "ActivationMode.h"
#include "DeployMode.h"

#define DEBUG 1

class StateMachine : public Task
{
private:
    Mode currentMode;
    unsigned long upTime;
    unsigned long totalUpTime;
    unsigned long OBCBootCount;
    OBCDataContainer * containerPointer;

protected:


public:
    StateMachine(OBCDataContainer *container) :
        Task(), containerPointer(container) {};
    bool notified( void );
    void setUp();
    void run();
};

#endif /* STATEMACHINE_H_ */
