/*
 * StateMachine.cpp
 *
 *  Created on: May 19, 2020
 *      Author: tom-h
 */
#include "StateMachine.h"

StateMachine* _stub;
extern FRAMVar<unsigned long> totalUptime;

StateMachine::StateMachine(BusMaster<PQ9Frame, PQ9Message> &busMaster, InternalCommandHandler<PQ9Frame, PQ9Message> &internalCmdHandler)
: PeriodicTask(1000, [](){_stub->StateMachineRun();})
{
    _stub = this;
    busHandler = &busMaster;
    intCmdHandler = &internalCmdHandler;
    currentState = OBCState::Activation;
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
        processCOMMBuffer();

    }else{
        //get Messages from COMMS:
        unsigned char payload = 3; //command to ask the nr of messages
        rcvdMsg = busHandler->RequestReply(Address::COMMS, 1, &payload, RADIO_SERVICE, MsgType::Request, 200);
        if(rcvdMsg){
            if(rcvdMsg->getDataPayload()[0] == 0){
                MsgsInQue = rcvdMsg->getDataPayload()[1];
                if(MsgsInQue){
                    Console::log("Radio Commands In Que: %d", MsgsInQue);
                }
            }else{
//                Console::log("getRadioCommands: FAIL");
            }
        }


        //STATE MACHINE HERE
        switch(currentState)
        {
        case OBCState::Activation:
            Console::log("Total Uptime: %d", (unsigned long)totalUptime);
            if((unsigned long) totalUptime > ACTIVATION_TIME){
                Console::log("Activation Timer Expired! -> Deploy");
                currentState = OBCState::Deploy;
            }
            break;
        case OBCState::Deploy:
            Console::log("Deploy Mode -> Turn on ADB");
            PowerBusControl(true, true, false, false);
            currentState = OBCState::Normal;
            break;
        case OBCState::Normal:
            break;
        }





        runPeriodic = false; // if the periodic was executed due to this flag. lower it.
    }



    if(execute){
        runPeriodic = true; //if execute got raised during execution, make sure to carry it over.
    }
}



//Function that takes care of commands in COMMS Buffer
void StateMachine::processCOMMBuffer(){
    //get Message from comms:
    uint8_t payload = 4;
    rcvdMsg = busHandler->RequestReply(Address::COMMS, 1, &payload, RADIO_SERVICE, MsgType::Request, 200);
    if(rcvdMsg){
        if(rcvdMsg->getDataPayload()[0] == 0 && rcvdMsg->getPayloadSize() > 7){ //Command = OK  //
            uint8_t commandID = rcvdMsg->getDataPayload()[1];
            Console::log("ID: %d | DST:%d | SRC:%d | SRV: %d | TYPE:%d | PAY0:%d", commandID,
                         rcvdMsg->getDataPayload()[0+2], rcvdMsg->getDataPayload()[2+2], rcvdMsg->getDataPayload()[3+2],  rcvdMsg->getDataPayload()[4+2], rcvdMsg->getDataPayload()[5+2]);

            if(rcvdMsg->getDataPayload()[0+2] == Address::OBC){
                //Internal Command, deal with it yourself
//                    Console::log("InternalCommand!");
                PQ9Frame internalCommand;
                internalCommand.setDestination(Address::OBC);
                internalCommand.setSource(rcvdMsg->getDataPayload()[2+2]);
                internalCommand.setPayloadSize(rcvdMsg->getPayloadSize() - 7 + 2);
                for (int i = 0; i < internalCommand.getPayloadSize(); i++)
                {
                    internalCommand.getPayload()[i] = rcvdMsg->getDataPayload()[5+i];
                }
//                    Console::log("INTERNAL COMMAND! (Size: %d, ID: %d) CMD = DEST:%d SRC:%d SIZE:%d", internalCommand.getPayloadSize()-2,commandID,internalCommand.getDestination(),internalCommand.getSource(),internalCommand.getPayloadSize());
                intCmdHandler->received(internalCommand);
                intCmdHandler->run();
                rcvdMsg = intCmdHandler->getTxMsg();
            }else{
                //Put Command on Bus:
                rcvdMsg = busHandler->RequestReply(rcvdMsg->getDataPayload()[0+2], rcvdMsg->getPayloadSize() - 7, &rcvdMsg->getDataPayload()[5+2], rcvdMsg->getDataPayload()[3+2], rcvdMsg->getDataPayload()[4+2], 200);
            }

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
                busHandler->RequestReply(Address::COMMS, rcvdMsg->getPayloadSize() + 7, radioReply, RADIO_SERVICE, MsgType::Request, 200);
//                    Console::log("%d %d %d %d %d %d %d %d", radioReply[1], radioReply[2], radioReply[3], radioReply[4], radioReply[5], radioReply[6], radioReply[7], radioReply[8]);
            }else{
                Console::log("Command Unsuccesful!");
                //unsuccesful: create your own reply
                uint8_t radioReply[256];
                radioReply[0] = 9; //Send Command
                radioReply[1] = commandID; //return command ID number
                //Hack: walk backwards from the payload to receive the full frame.
                radioReply[2] = 0; //destination
                radioReply[3] = 2; //size
                radioReply[4] = 0; //Source
                radioReply[5] = 0; //service
                radioReply[6] = 2; //Reply
                radioReply[7] = 1; //NO_RESPONSE
                busHandler->RequestReply(Address::COMMS, 8, radioReply, RADIO_SERVICE, MsgType::Request, 200);
            }
        }
    }
    //pop Message from stack:
    payload = 5;
    busHandler->RequestReply(Address::COMMS, 1, &payload, RADIO_SERVICE, MsgType::Request, 200);
    MsgsInQue--;
}

bool StateMachine::PowerBusControl(bool Line1, bool Line2, bool Line3, bool Line4)
{
    // Define relevant hex values corresponding to EPS command over bus.
    char V1 = 0x01;
    char V2 = 0x02;
    char V3 = 0x03;
    char V4 = 0x04;

////    Send to EPS
    uint8_t msgPayload[2];
    msgPayload[0] = V1;
    msgPayload[1] = Line1 ? 0x01:0x00;
    PQ9Message* Succes1 = busHandler->RequestReply(Address::EPS, 2, msgPayload, BUS_SERVICE, MsgType::Request, 500);
    msgPayload[0] = V2;
    msgPayload[1] = Line2 ? 0x01:0x00;
    PQ9Message* Succes2 = busHandler->RequestReply(Address::EPS, 2, msgPayload, BUS_SERVICE, MsgType::Request, 500);
    msgPayload[0] = V3;
    msgPayload[1] = Line3 ? 0x01:0x00;
    PQ9Message* Succes3 = busHandler->RequestReply(Address::EPS, 2, msgPayload, BUS_SERVICE, MsgType::Request, 500);
    msgPayload[0] = V4;
    msgPayload[1] = Line4 ? 0x01:0x00;
    PQ9Message* Succes4 = busHandler->RequestReply(Address::EPS, 2, msgPayload, BUS_SERVICE, MsgType::Request, 500);

    return Succes1 && Succes2 && Succes3 && Succes4;
}
