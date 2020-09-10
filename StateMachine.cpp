/*
 * StateMachine.cpp
 *
 *  Created on: May 19, 2020
 *      Author: tom-h
 */
#include "StateMachine.h"
#include <stdio.h>

StateMachine* _stub;
extern FRAMVar<unsigned long> totalUptime;
extern LittleFS fs;

uint8_t tempBufferd[5];

StateMachine::StateMachine(MB85RS &fram_in, BusMaster<PQ9Frame, PQ9Message> &busMaster, InternalCommandHandler<PQ9Frame, PQ9Message> &internalCmdHandler)
: PeriodicTask(1000, [](){_stub->StateMachineRun();})
{
    _stub = this;
    busHandler = &busMaster;
    intCmdHandler = &internalCmdHandler;
    fram = &fram_in;

    logFile.cfg = &logFilecfg;
    logTask.taskFile = &logFile;
    logTask.taskCompleted = true; //initialize with completed, as it is not running.
}

void StateMachine::init(){
    this->currentState.init(*fram, FRAM_OBC_STATE, true, true);
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
        rcvdMsg = busHandler->RequestReply(Address::COMMS, 1, &payload, ServiceNumber::Radio, MsgType::Request, 200);
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


        //LOGGING OPERATION

        //correction on uptime race-condition
        if((unsigned long) totalUptime == correctedUptime){
            //same time twice, increase with 1
            correctedUptime++;
        }else if((unsigned long) totalUptime - correctedUptime == 2){
            //skipped one second
            correctedUptime = (unsigned long) totalUptime - 1;
        }else{
            correctedUptime = (unsigned long) totalUptime;
        }


        if(!fs._mounted){
            Console::log("FS NOT MOUNTED!");
        }
        if((unsigned long) correctedUptime % LOG_INTERVAL == 0 && fs._mounted){
            if(logTask.taskCompleted){
                if(logTask.taskResult){
                    Console::log("##### EPSLogging Error?? : -%d", -logTask.taskResult);
                }


                char folderbuf[64];
                snprintf(folderbuf, sizeof(folderbuf), "LOG");
                fs.mkdir(folderbuf);
                snprintf(folderbuf, sizeof(folderbuf), "LOG/%d", (unsigned long)correctedUptime/100000);
                fs.mkdir(folderbuf);
                snprintf(folderbuf, sizeof(folderbuf), "LOG/%d/%d", (unsigned long)correctedUptime/100000,(unsigned long)correctedUptime/1000);
                fs.mkdir(folderbuf);

                snprintf(logTask.taskNameBuf, sizeof(logTask.taskNameBuf), "LOG/%d/%d/EPS_%d", (unsigned long)correctedUptime/100000,(unsigned long)correctedUptime/1000, (unsigned long)correctedUptime);
                logTask.taskOperation = FileSystemOperation::OWC;
                logTask.taskCompleted = false;
                logTask.taskSize = EPS_CONTAINER_SIZE;
                logTask.taskFlags = LFS_O_RDWR | LFS_O_CREAT | LFS_O_APPEND;
                logTask.taskArray = EPSContainer.getArray();

                int err = fs.queTask(logTask);
//                int err = fs.file_open_write_close_async(namebuf, LFS_O_RDWR | LFS_O_CREAT | LFS_O_APPEND, EPSContainer.getArray(), 111);
                if(err != 1){
                    Console::log("LFS QUE FULL");
                }
                Console::log("EPSUptime: %d", EPSContainer.getUptime());
                Console::log("Creating File: %s with Telemetry Size: %d", logTask.taskNameBuf, logTask.taskSize);
            }
            else
            {
                Console::log("LittleFS too busy! (err:-%d | op:%d)", -fs._err, fs.curOperation);
            }
        }

        //STATE MACHINE HERE
        switch((uint8_t)currentState)
        {
        case OBCState::Activation:
            Console::log("Total Uptime: %d", (unsigned long)totalUptime);
            if((unsigned long) totalUptime > ACTIVATION_TIME){
                Console::log("Activation Timer Expired! -> Deploy");
                currentState = OBCState::Deploy;
            }
            break;
        case OBCState::Deploy:
            Console::log("Get EPS Telemetry");
            getTelemetry(Address::EPS, EPSContainer);
            Console::log("Battery INA Status: %s | Battery GG Status: %s", EPSContainer.getBatteryINAStatus() ? "ACTIVE" : "ERROR", EPSContainer.getBatteryGGStatus() ? "ACTIVE" : "ERROR");

            short batteryVoltage;
            if(EPSContainer.getBatteryGGStatus()){
                batteryVoltage = EPSContainer.getBatteryGGVoltage();
            }else if(EPSContainer.getBatteryINAStatus()){
                batteryVoltage = EPSContainer.getBatteryINAVoltage();
            }else{
                //both sensors are dead...
                batteryVoltage = 3500;
            }

            if(batteryVoltage > DEPLOYMENT_VOLTAGE){
                Console::log("Voltage: %d mv -> DeployState: Turn on ADB", batteryVoltage);
                PowerBusControl(true, true, false, false);
//                todo;
//                PQ9Message* reply = busHandler->RequestReply(Address::ADB, 1, dataPayload, Service, Message_type, timeLimitMS)
                getTelemetry(Address::ADB, ADBContainer);
                short adbTemperature = ADBContainer.getTemperature()/10;
                Console::log("ADB Temperature: %s%d C", (adbTemperature<0)?"-":"", (adbTemperature<0)?-adbTemperature:adbTemperature);
                if(adbTemperature > 0 || (unsigned long) totalUptime > ACTIVATION_TIME + MAX_ADB_TEMPERATURE_WAIT){
                    Console::log("DeployState: DeployCommand");
                    uint8_t deployPayload = 1;
                    PQ9Message* reply = busHandler->RequestReply(Address::ADB, 1, &deployPayload, ServiceNumber::DeployService, MsgType::Request, 200);

                    if(reply){
                        if(reply->getDataPayload()[1] == 56){
                            Console::log("Command Succesful: ALREADY_BURNING");
                        }else if(reply->getDataPayload()[1] == 57){
                            Console::log("Command Succesful: FULLY_DEPLOYED");
                            currentState = OBCState::Normal;
                        }else if(reply->getDataPayload()[1] == 0){
                            Console::log("Command Succesful: STARTING_BURN");
                        }else{
                            Console::log("Command Succesful: %d %d %d", reply->getDataPayload()[0], reply->getDataPayload()[1], reply->getDataPayload()[2]);
                        }
                    }else{
                        Console::log("Command FAILED");
                    }
                }
            }
            break;
        case OBCState::Normal:
            getTelemetry(Address::EPS, EPSContainer);
            Console::log("NORMAL STATE tUt:%d | Battery INA Status: %s | Voltage %d mV", (unsigned long)correctedUptime, EPSContainer.getBatteryINAStatus() ? "ACTIVE" : "ERROR", EPSContainer.getBatteryINAVoltage());
            break;
        default:
            currentState = OBCState::Activation;
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
    rcvdMsg = busHandler->RequestReply(Address::COMMS, 1, &payload, ServiceNumber::Radio, MsgType::Request, 200);
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
                busHandler->RequestReply(Address::COMMS, rcvdMsg->getPayloadSize() + 7, radioReply, ServiceNumber::Radio, MsgType::Request, 200);
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
                busHandler->RequestReply(Address::COMMS, 8, radioReply, ServiceNumber::Radio, MsgType::Request, 200);
            }
        }
    }
    //pop Message from stack:
    payload = 5;
    busHandler->RequestReply(Address::COMMS, 1, &payload, ServiceNumber::Radio, MsgType::Request, 200);
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
    PQ9Message* Succes1 = busHandler->RequestReply(Address::EPS, 2, msgPayload, ServiceNumber::PowerBus, MsgType::Request, 500);
    msgPayload[0] = V2;
    msgPayload[1] = Line2 ? 0x01:0x00;
    PQ9Message* Succes2 = busHandler->RequestReply(Address::EPS, 2, msgPayload, ServiceNumber::PowerBus, MsgType::Request, 500);
    msgPayload[0] = V3;
    msgPayload[1] = Line3 ? 0x01:0x00;
    PQ9Message* Succes3 = busHandler->RequestReply(Address::EPS, 2, msgPayload, ServiceNumber::PowerBus, MsgType::Request, 500);
    msgPayload[0] = V4;
    msgPayload[1] = Line4 ? 0x01:0x00;
    PQ9Message* Succes4 = busHandler->RequestReply(Address::EPS, 2, msgPayload, ServiceNumber::PowerBus, MsgType::Request, 500);

    return Succes1 && Succes2 && Succes3 && Succes4;
}

bool StateMachine::getTelemetry(unsigned char destination, TelemetryContainer &targetContainer){
    PQ9Message* housekeepingMsg = busHandler->RequestReply(destination, 0, 0, ServiceNumber::Housekeeping, MsgType::Request, 500);
    if(housekeepingMsg){
        for(int i=0; i <housekeepingMsg->getPayloadSize(); i++){
            targetContainer.getArray()[i] = housekeepingMsg->getDataPayload()[i];
        }
//        Console::log("Tele %d %d %d %d %d", housekeepingMsg->getDataPayload()[0], housekeepingMsg->getDataPayload()[1], housekeepingMsg->getDataPayload()[2], housekeepingMsg->getDataPayload()[3], housekeepingMsg->getDataPayload()[4]);
        return true;
    }else{
        return false;
    }
}
