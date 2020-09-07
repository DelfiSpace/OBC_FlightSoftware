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
#include "InternalCommandHandler.h"

#define RADIO_SERVICE 20
#define BUS_SERVICE 1
#define ACTIVATION_TIME 1*60

enum OBCState {Activation = 0x00, Deploy = 0x01, Normal = 0x02 };

class StateMachine : public PeriodicTask
{
public:
    StateMachine(BusMaster<PQ9Frame, PQ9Message> &busMaster, InternalCommandHandler<PQ9Frame, PQ9Message> &internalCmdHandler);
    void StateMachineRun();
    virtual bool notified();

private:
    void processCOMMBuffer();
    bool PowerBusControl(bool Line1, bool Line2, bool Line3, bool Line4);

    BusMaster<PQ9Frame, PQ9Message>* busHandler;
    InternalCommandHandler<PQ9Frame, PQ9Message>* intCmdHandler;
    PQ9Message* rcvdMsg;
    int MsgsInQue = 0;
    bool runPeriodic = false; //safety flag to make sure the periodic function runs.

    uint8_t currentState;

};

#endif /* STATEMACHINE_H_ */
