/*
 * StateMachine.h
 *
 *  Created on: May 19, 2020
 *      Author: tom-h
 */

#ifndef STATEMACHINE_H_
#define STATEMACHINE_H_

#include "PeriodicTask.h"
#include "Console.h"
#include "BusMaster.h"
#include "PQ9Frame.h"
#include "PQ9Message.h"

class StateMachine : public PeriodicTask
{
public:
    StateMachine(BusMaster<PQ9Frame, PQ9Message> &busMaster);
    void StateMachineRun();
    virtual bool notified();

private:
    BusMaster<PQ9Frame, PQ9Message>* busHandler;
    PQ9Message* rcvdMsg;
    int MsgsInQue = 0;
    bool runPeriodic = false; //safety flag to make sure the periodic function runs.

};
void StateMachineInit();

#endif /* STATEMACHINE_H_ */
