/*
 * StateMachine.cpp
 *
 *  Created on: May 19, 2020
 *      Author: tom-h
 */
#include "StateMachine.h"

StateMachine* _stub;

StateMachine::StateMachine(BusMaster<PQ9Frame, PQ9Message> &busMaster)
: PeriodicTask(2000, [](){_stub->StateMachineRun();})
{
    _stub = this;
    busHandler = &busMaster;
    Console::log("Works?");
}

bool StateMachine::notified(){
    return execute | MsgsInQue | runPeriodic;
}

void StateMachine::StateMachineRun()
{
    execute = false; //lower immidiately, to detect 're-raise'
    if(runPeriodic){
        Console::log("Periodic Detect!"); //this flag gets raised if you should make time for the Periodic Function
    }

    if(this->MsgsInQue && !runPeriodic){

        //get Message from comms:
        uint8_t payload = 4;
        rcvdMsg = busHandler->RequestReply(Address::COMMS, 1, &payload, 20, 1, 200);
        if(rcvdMsg){
            if(rcvdMsg->getDataPayload()[0] == 0 && rcvdMsg->getPayloadSize() > 7){ //Command = OK  //
                uint8_t commandID = rcvdMsg->getDataPayload()[1];
                Console::log("ID: %d | DST:%d | SRC:%d | SRV: %d | TYPE:%d | PAY0:%d", commandID,
                             rcvdMsg->getDataPayload()[0+2], rcvdMsg->getDataPayload()[2+2], rcvdMsg->getDataPayload()[3+2],  rcvdMsg->getDataPayload()[4+2], rcvdMsg->getDataPayload()[5+2]);
                volatile int po = 0;
//                Console::log("Payload Size: %d", rcvdMsg->getPayloadSize());
                //(CMD, ID) 0: dst 1: size 2: src 3: service 4: type 5: payload

                //run Command
                rcvdMsg = busHandler->RequestReply(rcvdMsg->getDataPayload()[0+2], rcvdMsg->getPayloadSize() - 7, &rcvdMsg->getDataPayload()[5+2], rcvdMsg->getDataPayload()[3+2], rcvdMsg->getDataPayload()[4+2], 200);
                if(rcvdMsg){
                    //Command Succesful, reply on the Radio
                    uint8_t radioReply[256];
                    radioReply[0] = 9; //Send Command
                    radioReply[1] = commandID; //return command ID number
                    //Hack: walk backwards from the payload to receive the full frame.
                    radioReply[2] = rcvdMsg->getBuffer()[-3]; //destination
                    radioReply[3] = rcvdMsg->getBuffer()[-2]; //size
                    radioReply[4] = rcvdMsg->getBuffer()[-1]; //Source
                    radioReply[5] = rcvdMsg->getService();
                    radioReply[6] = rcvdMsg->getMessageType();
                    for(int j = 0; j < rcvdMsg->getPayloadSize(); j++){
                        radioReply[7+j] = rcvdMsg->getDataPayload()[j];
                    }
                    busHandler->RequestReply(Address::COMMS, rcvdMsg->getPayloadSize() + 7, radioReply, 20, 1, 200);
                    Console::log("%d %d %d %d %d %d %d %d", radioReply[1], radioReply[2], radioReply[3], radioReply[4], radioReply[5], radioReply[6], radioReply[7], radioReply[8]);
                }
            }
        }

        //pop Message from stack:
        payload = 5;
        busHandler->RequestReply(Address::COMMS, 1, &payload, 20, 1, 200);
        MsgsInQue--;
    }else{
        Console::log("StateMachine :: Normal Mode");
        //get Messages from COMMS:
        unsigned char payload = 3; //command to ask the nr of messages
        rcvdMsg = busHandler->RequestReply(Address::COMMS, 1, &payload, 20, 1, 200);
        if(rcvdMsg){
            if(rcvdMsg->getDataPayload()[0] == 0){
                MsgsInQue = rcvdMsg->getDataPayload()[1];
                Console::log("getRadioCommands: %d", MsgsInQue);
            }else{
//                Console::log("getRadioCommands: FAIL");
            }
        }

        runPeriodic = false; // if the periodic was executed due to this flag. lower it.
    }



    if(execute){
        runPeriodic = true; //if execute got raised during execution, make sure to carry it over.
    }
}

void StateMachineRun()
{


}
