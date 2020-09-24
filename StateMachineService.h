#include "Service.h"
#include "StateMachine.h"
#include "Console.h"

#ifndef STATEMACHINESERVICE_H_
#define STATEMACHINESERVICE_H_

#define STATEMACHINE_SERVICE               41
#define STATEMACHINE_GET_STATE             0
#define STATEMACHINE_OVERRIDE_STATE        1
#define STATEMACHINE_1SEC_BUBBLE           2
#define STATEMACHINE_UPTIME_OVERRIDE       3
#define STATEMACHINE_SET_BEACON            4



class StateMachineService: public Service
{
protected:
//    StateMachine* stateMachine;

public:
    StateMachineService();//StateMachine &stateMachine_in);

    virtual bool process( DataMessage &command, DataMessage &workingBbuffer );
};

#endif
