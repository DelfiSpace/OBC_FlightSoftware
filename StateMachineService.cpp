#include "StateMachineService.h"

extern StateMachine stateMachine;
extern MB85RS fram;
extern TelemetryRequestService tlmReqServ;

StateMachineService::StateMachineService()//StateMachine &stateMachine_in)
{
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
            if(command.getPayloadSize() == 1){
                Console::log("StateMachineService: GET_STATE");
                workingBuffer.getDataPayload()[0] = STATEMACHINE_GET_STATE;
                workingBuffer.getDataPayload()[1] = (uint8_t) stateMachine.currentState;
                workingBuffer.setPayloadSize(2);
            }else{
                goto UNKNOWNCOMMAND;
            }
            break;
        case STATEMACHINE_OVERRIDE_STATE:
            if(command.getPayloadSize() == 2){
                Console::log("StateMachineService: OVERRIDE_STATE : %d",  command.getDataPayload()[1]);
                stateMachine.currentState.write(command.getDataPayload()[1]);
                workingBuffer.getDataPayload()[0] = STATEMACHINE_OVERRIDE_STATE;
                workingBuffer.getDataPayload()[1] = 0; //NO_ERROR
                workingBuffer.setPayloadSize(2);
            }else{
                goto UNKNOWNCOMMAND;
            }
            break;
        case STATEMACHINE_1SEC_BUBBLE:
            Console::log("StateMachineService: 1SEC_BUBBLE");
            stateMachine.addOneSecWait();
            break;
        case STATEMACHINE_UPTIME_OVERRIDE:
            if(command.getPayloadSize() == 5){
                unsigned long ulong;
                ((unsigned char *)&ulong)[3] = command.getDataPayload()[1];
                ((unsigned char *)&ulong)[2] = command.getDataPayload()[2];
                ((unsigned char *)&ulong)[1] = command.getDataPayload()[3];
                ((unsigned char *)&ulong)[0] = command.getDataPayload()[4];
                Console::log("StateMachineService: OVERRIDE UPTIME : %d", ulong);
                stateMachine.overrideTotalUptime(ulong);
                workingBuffer.getDataPayload()[0] = STATEMACHINE_UPTIME_OVERRIDE;
                workingBuffer.getDataPayload()[1] = 0; //NO_ERROR
                workingBuffer.setPayloadSize(2);
            }else{
                goto UNKNOWNCOMMAND;
            }
            break;
        case STATEMACHINE_SET_BEACON:
            if(command.getPayloadSize() == 2){
                Console::log("StateMachineService: BEACON %s", (command.getDataPayload()[1] != 0) ? "ON" : "OFF");
                stateMachine.beaconEnabled = (command.getDataPayload()[1] != 0) ? 1 : 0;
                workingBuffer.getDataPayload()[0] = STATEMACHINE_SET_BEACON;
                workingBuffer.getDataPayload()[1] = 0; //NO_ERROR
                workingBuffer.setPayloadSize(2);
            }else{
                goto UNKNOWNCOMMAND;
            }
            break;
        case STATEMACHINE_COMPLETE_RESET:
            //1 FORMAT SD CARD
            tlmReqServ.formatFileSystem();
            //2 FORMAT FRAM
            fram.erase();
            //(3 ENABLE BEACON?)

            //4 RESET
            this->setPostFunc([](){MAP_SysCtl_A_rebootDevice();});
            workingBuffer.getDataPayload()[0] = STATEMACHINE_COMPLETE_RESET;
            workingBuffer.getDataPayload()[1] = 0; //NO_ERROR
            workingBuffer.setPayloadSize(2);
            break;
UNKNOWNCOMMAND:
        default:
            Console::log("StateMachineService: Unknown command!");
            workingBuffer.setPayloadSize(1);
            workingBuffer.getDataPayload()[0] = 1; //UNKNOWN CMD ERROR
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
