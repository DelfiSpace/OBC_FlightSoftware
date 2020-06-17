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
#include "DeployMode.h"

class StateMachine : public Task
{
private:
    Mode currentMode;
    unsigned long upTime;
    unsigned long totalUpTime;
    unsigned long OBCBootCount;
    OBCDataContainer * containerPointer;

protected:
    void run();

public:
    StateMachine(OBCDataContainer *container) :
        Task(), containerPointer(container) {};
    bool notified( void );
    void setUp();
};

#endif /* STATEMACHINE_H_ */
