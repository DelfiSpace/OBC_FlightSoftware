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
extern HousekeepingService<OBCTelemetryContainer> hk;

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
                    Console::log("##### Logging Error?? : -%d", -logTask.taskResult);
                }

                char folderbuf[64];
                snprintf(folderbuf, sizeof(folderbuf), "LOG");
                fs.mkdir(folderbuf);
                snprintf(folderbuf, sizeof(folderbuf), "LOG/%d", (unsigned long)correctedUptime/100000);
                fs.mkdir(folderbuf);
                snprintf(folderbuf, sizeof(folderbuf), "LOG/%d/%d", (unsigned long)correctedUptime/100000,(unsigned long)correctedUptime/1000);
                fs.mkdir(folderbuf);

                //collect Telemetry
                //copy OBC telemetry:
                memcpy(totalTelemetryContainer, hk.getTelemetry()->getArray(), OBC_CONTAINER_SIZE);

                //query other telemetry:
                uint8_t tlmIndex = OBC_CONTAINER_SIZE;
                for(int j = 2; j <= 4; j++){
                    getTelemetry(j, &totalTelemetryContainer[tlmIndex]);
                    tlmIndex += telemetrySizes[j-1]; //OBC:0 EPS:1 ADB:2 COMMS:3
                }


                snprintf(logTask.taskNameBuf, sizeof(logTask.taskNameBuf), "LOG/%d/%d/TLM_%d", (unsigned long)correctedUptime/100000,(unsigned long)correctedUptime/1000, (unsigned long)correctedUptime);
                logTask.taskOperation = FileSystemOperation::OWC;
                logTask.taskCompleted = false;
                logTask.taskSize = totalTelemetrySize;
                logTask.taskFlags = LFS_O_RDWR | LFS_O_CREAT | LFS_O_APPEND;
                logTask.taskArray = totalTelemetryContainer;

                int err = fs.queTask(logTask);
//                int err = fs.file_open_write_close_async(namebuf, LFS_O_RDWR | LFS_O_CREAT | LFS_O_APPEND, EPSContainer.getArray(), 111);
                if(err != 1){
                    Console::log("LFS QUE FULL");
                }

                //Debug Test
                memcpy(EPSContainer.getArray(), &totalTelemetryContainer[OBC_CONTAINER_SIZE], 16);
                Console::log("DEBUG:: CONTAINER UPTIME: %d", EPSContainer.getUptime());
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
            Console::log("Activation Uptime: %d", (unsigned long)correctedUptime);
            if((unsigned long) correctedUptime > ACTIVATION_TIME){
                Console::log("Activation Timer Expired! -> Deploy State");
                lastDeployTime = (unsigned long) totalUptime  - ADB_DEPLOY_TIMEOUT;
                deployMode = 0;
                currentState = OBCState::Deploy;
            }
            break;
        case OBCState::Deploy:
            Console::log("Deploy Uptime: %d", (unsigned long)correctedUptime);
            getTelemetry(Address::EPS, EPSContainer.getArray());
//            Console::log("Battery INA Status: %s | Battery GG Status: %s", EPSContainer.getBatteryINAStatus() ? "ACTIVE" : "ERROR", EPSContainer.getBatteryGGStatus() ? "ACTIVE" : "ERROR");

            short batteryVoltage;
            if(EPSContainer.getBatteryGGStatus()){
                batteryVoltage = EPSContainer.getBatteryGGVoltage();
            }else if(EPSContainer.getBatteryINAStatus()){
                batteryVoltage = EPSContainer.getBatteryINAVoltage();
            }else{
                //both sensors are dead...
                batteryVoltage = DEPLOYMENT_VOLTAGE + 1;
            }

            if(batteryVoltage > DEPLOYMENT_VOLTAGE){
                //turn on ADB
                PowerBusControl(2, true);


                //check if already deployed
                uint8_t deployPayload = 0;
                PQ9Message* reply = busHandler->RequestReply(Address::ADB, 1, &deployPayload, ServiceNumber::DeployService, MsgType::Request, 50);
                if(reply){
                    if(reply->getDataPayload()[1] == 0x0F){
                        Console::log("+ALREADY DEPLOYED! ");
                        currentState = OBCState::Normal;
                        break;
                    }
                }else{
                    Console::log("ADB NOT RESPONDING!");
                    break;
                }

                switch(deployMode){
                case 0: //Wait for deploy timeout ( and (temperature or temperature wait timeout) )
                    getTelemetry(Address::ADB,ADBContainer.getArray());
                    if( (correctedUptime - lastDeployTime > ADB_DEPLOY_TIMEOUT && ADBContainer.getTemperature() > ADB_DEPLOY_MINTEMP) || correctedUptime - lastDeployTime > ADB_DEPLOY_TIMEOUT + ADB_DEPLOY_MAXTEMPWAIT){
                        deployMode = 1;
                    }else{
                        Console::log("WAIT FOR DEPLOY: %d s |  T:%s%d C", correctedUptime - lastDeployTime,ADBContainer.getTemperature()<0?"-":"", ADBContainer.getTemperature()<0?-ADBContainer.getTemperature():ADBContainer.getTemperature() );
                    }
                    break;
                case 1: //Deploy
                    Console::log("DEPLOY!");
                    deployPayload = 2;
                    reply = busHandler->RequestReply(Address::ADB, 1, &deployPayload, ServiceNumber::DeployService, MsgType::Request, 50);
                    if(reply){
                        if(reply->getDataPayload()[1] == 57){
                            Console::log("-ALREADY DEPLOYED! ");
                            currentState = OBCState::Normal;
                        }
                        deployMode = 2;
                    }
                    break;
                case 2: //Wait for finish
                    deployPayload = 1;
                    reply = busHandler->RequestReply(Address::ADB, 1, &deployPayload, ServiceNumber::DeployService, MsgType::Request, 50);
                    if(reply){
                        if(reply->getDataPayload()[2] == 1){
                            Console::log("BURNING!");
                        }else{
                            Console::log("DONE!");
                            deployMode = 3;
                        }
                    }
                    break;
                case 3: //Check if deployed
                    deployPayload = 0;
                    reply = busHandler->RequestReply(Address::ADB, 1, &deployPayload, ServiceNumber::DeployService, MsgType::Request, 50);
                    if(reply){
                        if(reply->getDataPayload()[1] == 0x0F){
                            Console::log("DEPLOYED! ");
                            currentState = OBCState::Normal;
                            break;
                        }else{
                            lastDeployTime = correctedUptime;
                            Console::log("DEPLOY FAILED : RETRYING : (new lastDeployTime: %d)", lastDeployTime);
                            deployMode = 0;
                        }
                    }
                    break;
                default:
                    deployMode = 0;
                    break;
                }


            }
            break;
//                Console::log("ADB ON!");

//                //check if already deployed:
//                uint8_t deployPayload = 0;
//                PQ9Message* reply = busHandler->RequestReply(Address::ADB, 1, &deployPayload, ServiceNumber::DeployService, MsgType::Request, 50);
//                if(reply->getDataPayload()[1] == 0x0F){
//                    Console::log(" ALREADY DEPLOYED! ");
//                    currentState = OBCState::Normal;
//                    break;
//                }
//
//                //get ADB temperature
//                getTelemetry(Address::ADB, ADBContainer.getArray());
//                signed short adbTemperature = ADBContainer.getTemperature();
//                Console::log("ADB Temperature: %s%d C", (adbTemperature<0)?"-":"", (adbTemperature<0)?-adbTemperature:adbTemperature);
//
//                //check if DeployTimeout has expired
//                if(adbTemperature > 0 || (unsigned long) totalUptime - lastBurnTime > ADB_DEPLOY_TIMEOUT + ADB_TEMPERATURE_WAIT){
//
//                    //Check if deploying!
//                    deployPayload = 1;
//                    PQ9Message* reply = busHandler->RequestReply(Address::ADB, 1, &deployPayload, ServiceNumber::DeployService, MsgType::Request, 50);
//                    if(reply){
//                        if(reply->getDataPayload()[2] == 1){
//                            Console::log("Command Succesful: ALREADY_BURNING");
//                        }else if(reply->getDataPayload()[2] == 0){
//
//                            //Deploy!
//                            lastBurnTime = (unsigned long) totalUptime;
//                            deployPayload = 2;
//                            PQ9Message* reply = busHandler->RequestReply(Address::ADB, 1, &deployPayload, ServiceNumber::DeployService, MsgType::Request, 50);
//                            if(reply->getDataPayload()[1] == 57){
//                                Console::log("Command Succesful: Already Deployed");
//                                currentState = OBCState::Normal;
//                            }
//
//                        }
//                    }else{
//                        Console::log("DEPLOY Command FAILED");
//                        //reboot?
//                    }

        case OBCState::Normal:
            Console::log("Nominal mode (totalTime: %d s)", correctedUptime);
//            getTelemetry(Address::EPS, EPSContainer);
//            Console::log("NORMAL STATE tUt:%d | Battery INA Status: %s | Voltage %d mV", (unsigned long)correctedUptime, EPSContainer.getBatteryINAStatus() ? "ACTIVE" : "ERROR", EPSContainer.getBatteryINAVoltage());
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

bool StateMachine::PowerBusControl(uint8_t bus, bool status)
{

////    Send to EPS
    uint8_t msgPayload[2];
    msgPayload[0] = bus;
    msgPayload[1] = status ? 0x01:0x00;
    PQ9Message* Succes = busHandler->RequestReply(Address::EPS, 2, msgPayload, ServiceNumber::PowerBus, MsgType::Request, 50);

    __delay_cycles(48000000/(1000L/50)); // @suppress("Function cannot be resolved")
    return Succes;
}

bool StateMachine::getTelemetry(uint8_t destination, uint8_t* targetContainer){
    PQ9Message* housekeepingMsg = busHandler->RequestReply(destination, 0, 0, ServiceNumber::Housekeeping, MsgType::Request, 100);
    if(housekeepingMsg){
        for(int i=0; i <housekeepingMsg->getPayloadSize(); i++){
            targetContainer[i] = housekeepingMsg->getDataPayload()[i];
        }
        Console::log("TELEMETRY %d RESPONDING", destination);

//        Console::log("Tele %d %d %d %d %d", housekeepingMsg->getDataPayload()[0], housekeepingMsg->getDataPayload()[1], housekeepingMsg->getDataPayload()[2], housekeepingMsg->getDataPayload()[3], housekeepingMsg->getDataPayload()[4]);
        return true;
    }else{
        Console::log("TELEMETRY %d NOT RESPONDING", destination);
        memset(targetContainer,0,telemetrySizes[destination-1]);
        return false;
    }
}
