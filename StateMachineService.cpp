#include "StateMachineService.h"

StateMachineService::StateMachineService(StateMachine &stateMachine_in)
{
    stateMachine = &stateMachine_in;
};

bool StateMachineService::process(DataMessage &command, DataMessage &workingBuffer)
{
    if (command.getService() == STATEMACHINE_SERVICE)
    {
//        Console::log("RadioService: Service started");
        workingBuffer.setService(STATEMACHINE_SERVICE);
        workingBuffer.setMessageType(SERVICE_RESPONSE_REPLY);

        switch(command.getDataPayload()[0])
        {
        case STATEMACHINE_GET_STATE:
            Console::log("StateMachineService: GET_STATE");
            workingBuffer.getDataPayload()[0] = (uint8_t) stateMachine->currentState;
            workingBuffer.setPayloadSize(1);
            break;
        case STATEMACHINE_OVERRIDE_STATE:
            Console::log("StateMachineService: OVERRIDE_STATE");
            stateMachine->currentState = workingBuffer.getDataPayload()[1];
            workingBuffer.getDataPayload()[0] = 0;
            workingBuffer.setPayloadSize(1);
            break;
        case STATEMACHINE_1SEC_BUBBLE:
            Console::log("StateMachineService: 1SEC_BUBBLE");
            stateMachine->addOneSecWait();
            break;
        case STATEMACHINE_UPTIME_OVERRIDE:
            unsigned long ulong;
            ((unsigned char *)&ulong)[3] = command.getDataPayload()[1];
            ((unsigned char *)&ulong)[2] = command.getDataPayload()[2];
            ((unsigned char *)&ulong)[1] = command.getDataPayload()[3];
            ((unsigned char *)&ulong)[0] = command.getDataPayload()[4];
            Console::log("StateMachineService: OVERRIDE UPTIME : %d", ulong);
            stateMachine->overrideTotalUptime(ulong);
            break;
        default:
            Console::log("StateMachineService: Unknown command!");
            workingBuffer.setPayloadSize(1);
            workingBuffer.getDataPayload()[0] = 0;
            break;
        }

        return true;
    }
    else
    {
        // this command is related to another service,
        // report the command was not processed
        return false;
    }
}
