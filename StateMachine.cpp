/*
 * StateMachine.cpp
 *
 *  Created on: May 19, 2020
 *      Author: tom-h
 */
#include "StateMachine.h"
#include <stdio.h>

StateMachine* _stub;
extern FRAMBackedVar<unsigned long> totalUptime;
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
    this->currentDeployTime.init(*fram, FRAM_CURRENT_DEPLOY_TIME, true, true);
    this->beaconEnabled.init(*fram, FRAM_BEACON_ENABLED, true, true);
    this->lastMsgReceived.init(*fram, FRAM_LAST_MSG_TIME ,true,true);
}

bool StateMachine::notified(){
    return execute | MsgsInQue | runPeriodic;
}

void StateMachine::StateMachineRun()
{
    if(execute){
        runPeriodic = true;  //execute flag is raised, periodic function should be run.
    }
    execute = false; //lower immidiately, to detect 're-raise'
//    if(runPeriodic){
//        Console::log("Periodic Detect!"); //this flag gets raised if you should make time for the Periodic Function
//    }

    if(this->MsgsInQue && !runPeriodic && !waitTime){
        processCOMMBuffer();

    }else if(runPeriodic){
        //get Messages from COMMS:
        unsigned char payload = 3; //command to ask the nr of messages
        rcvdMsg = busHandler->RequestReply(Address::COMMS, 1, &payload, ServiceNumber::Radio, MsgType::Request, 50);
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

        //UPDATE WAIT TIME
        if(waitTime>0){
            Console::log("REDUCING WAIT TIME: %d", waitTime);
            waitTime--;
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

        //RESET BATTERY IF NO LAST MSG RECEIVED
        if(correctedUptime - (unsigned long)lastMsgReceived > MIN_MSG_INTERVAL && (unsigned long) lastMsgReceived != 0){
            Console::log("Havent received any ground messages, reset battery");
            lastMsgReceived = (unsigned long) correctedUptime;
            uint8_t resetPayload = 5;
            rcvdMsg = busHandler->RequestReply(Address::EPS, 1, &resetPayload, ServiceNumber::PowerBus, MsgType::Request, 50);
        }

        if((unsigned long) correctedUptime % LOG_INTERVAL == 0){
            //collect Telemetry
            //copy OBC telemetry:
            memcpy(totalTelemetryContainer, hk.getTelemetry()->getArray(), OBC_CONTAINER_SIZE);

            //query other telemetry:
            uint8_t tlmIndex = OBC_CONTAINER_SIZE;
            for(int j = 2; j <= 4; j++){
                getTelemetry(j, &totalTelemetryContainer[tlmIndex]);
                tlmIndex += telemetrySizes[j-1]; //OBC:0 EPS:1 ADB:2 COMMS:3
            }

            if(logTask.taskCompleted  && fs._mounted){
                if(logTask.taskResult){
                    Console::log("##### Logging Error?? : -%d", -logTask.taskResult);
                }

                char folderbuf[64];
                snprintf(folderbuf, sizeof(folderbuf), "LOG");
                fs.mkdir(folderbuf);
                snprintf(folderbuf, sizeof(folderbuf), "LOG/%d", (unsigned long)correctedUptime/10000000);
                fs.mkdir(folderbuf);
                snprintf(folderbuf, sizeof(folderbuf), "LOG/%d/%d", (unsigned long)correctedUptime/10000000, (unsigned long)correctedUptime/100000);
                fs.mkdir(folderbuf);
                snprintf(folderbuf, sizeof(folderbuf), "LOG/%d/%d/%d", (unsigned long)correctedUptime/10000000, (unsigned long)correctedUptime/100000,(unsigned long)correctedUptime/1000);
                fs.mkdir(folderbuf);


                snprintf(logTask.taskNameBuf, sizeof(logTask.taskNameBuf), "LOG/%d/%d/%d/TLM_%d", (unsigned long)correctedUptime/10000000, (unsigned long)correctedUptime/100000,(unsigned long)correctedUptime/1000, (unsigned long)correctedUptime);
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
//                memcpy(EPSContainer.getArray(), &totalTelemetryContainer[OBC_CONTAINER_SIZE], 16);
//                Console::log("DEBUG:: CONTAINER UPTIME: %d", EPSContainer.getUptime());
                Console::log("Creating File: %s with Telemetry Size: %d", logTask.taskNameBuf, logTask.taskSize);
            }
            else
            {
                if(fs.curOperation == 1){
                    Console::log("LittleFS Still Mounting..");
                }else{
                    Console::log("LittleFS too busy! (err:-%d | op:%d)", -fs._err, fs.curOperation);
                }
            }
        }

        //STATE MACHINE HERE
        switch((uint8_t)currentState)
        {
        case OBCState::Activation:
            Console::log("ACTIVATION: Uptime: %d s (ACTIVATION_TIME: %d s)", (unsigned long)correctedUptime, ACTIVATION_TIME);
            if((unsigned long) correctedUptime >= ACTIVATION_TIME){
//                Console::log("Activation Timer Expired! -> Deploy State");
                deployMode = 0;
                currentState = OBCState::Deploy;
            }
            break;
        case OBCState::Deploy:
//            Console::log("Deploy Uptime: %d", (unsigned long)correctedUptime);
            getTelemetry(Address::EPS, EPSContainer.getArray());
//            Console::log("Battery INA Status: %s | Battery GG Status: %s", EPSContainer.getBatteryINAStatus() ? "ACTIVE" : "ERROR", EPSContainer.getBatteryGGStatus() ? "ACTIVE" : "ERROR");

            short batteryVoltage = 0;
            short batteryVoltageGG = 0;
            short batteryVoltageINA = 0;
            short batteryVoltageUNREG = 0;
            if(EPSContainer.getBatteryGGStatus()){
                batteryVoltageGG = EPSContainer.getBatteryGGVoltage();
            }
            if(EPSContainer.getBatteryINAStatus()){
                batteryVoltageINA = EPSContainer.getBatteryINAVoltage();
            }
            if(EPSContainer.getUnregulatedINAStatus()){
                batteryVoltageUNREG = EPSContainer.getUnregulatedINAVoltage();
            }


            if(!(EPSContainer.getBatteryGGStatus() ||  EPSContainer.getBatteryINAStatus() || EPSContainer.getUnregulatedINAStatus())){
                //all sensors are dead...
                batteryVoltage = DEPLOYMENT_VOLTAGE + 1;
            }else{

            }

            if(batteryVoltage > DEPLOYMENT_VOLTAGE || batteryVoltageINA > DEPLOYMENT_VOLTAGE || batteryVoltageGG > DEPLOYMENT_VOLTAGE || batteryVoltageUNREG > DEPLOYMENT_VOLTAGE){

                //current persistent DeployTime
                currentDeployTime += 1;

                //turn on ADB
                PowerBusControl(2, true);

                //check if already deployed
                uint8_t deployPayload = 0;
                PQ9Message* reply = busHandler->RequestReply(Address::ADB, 1, &deployPayload, ServiceNumber::DeployService, MsgType::Request, 50);
                if(reply){
                    if(reply->getDataPayload()[1] == 0x0F){
                        Console::log("DEPLOY: +Deployed! ");
                        currentState = OBCState::Normal;
                        break;
                    }
                }else{
                    Console::log("DEPLOY: ADB NOT RESPONDING!");
                    break;
                }

                switch(deployMode){
                case 0: //Temperature or temperature wait timeout
                    getTelemetry(Address::ADB,ADBContainer.getArray());
                    if( (ADBContainer.getTemperature() > ADB_DEPLOY_MINTEMP) || currentDeployTime > ADB_DEPLOY_MAXTEMPWAIT){
                        deployMode = 1;
                    }else{
                        Console::log("DEPLOY: Wait for suitable temperature: %d s (MAX: %d s) |  T:%s %d C (MIN: %d C)", currentDeployTime, ADB_DEPLOY_MAXTEMPWAIT, ADBContainer.getTemperature()<0?"-":"", ADBContainer.getTemperature()<0?-ADBContainer.getTemperature():ADBContainer.getTemperature() ,ADB_DEPLOY_MINTEMP );
                    }
                    break;
                case 1: //Deploy
                    Console::log("DEPLOY: Initiating Deployment!");
                    deployPayload = 2;
                    reply = busHandler->RequestReply(Address::ADB, 1, &deployPayload, ServiceNumber::DeployService, MsgType::Request, 50);
                    if(reply){
                        if(reply->getDataPayload()[1] == 57){
                            Console::log("Deploy: -Already Deployed! ");
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
                            Console::log("DEPLOY: Burning!");
                        }else{
                            Console::log("DEPLOY: Done Burning!");
                            deployMode = 3;
                        }
                    }
                    break;
                case 3: //Check if deployed
                    deployPayload = 0;
                    reply = busHandler->RequestReply(Address::ADB, 1, &deployPayload, ServiceNumber::DeployService, MsgType::Request, 50);
                    if(reply){
                        if(reply->getDataPayload()[1] == 0x0F){
                            Console::log("DEPLOY: Successful deployment! ");
                            currentState = OBCState::Normal;
                            break;
                        }else{
                            stoppedDeployTime = currentDeployTime;
                            Console::log("DEPLOY: Failed deployment, retrying : (new stoppedDeployTime: %d)", stoppedDeployTime);
                            deployMode = 4;
                        }
                    }
                    break;
                case 4: //Wait for TIMEOUT expire
                    if(currentDeployTime - stoppedDeployTime > ADB_DEPLOY_TIMEOUT){
                        currentDeployTime = 0;
                        deployMode = 0;
                    }else{
                        Console::log("DEPLOY: Wait for Retry %d s (TIMEOUT: %d s)", currentDeployTime - stoppedDeployTime, ADB_DEPLOY_TIMEOUT);
                    }
                    break;
                default:
                    deployMode = 0;
                    break;
                }
            }
            break;

        case OBCState::Normal:
            switch(this->operationalState){
            case 0: //SAFEMODE
                bool EPSAlive = getTelemetry(Address::EPS, EPSContainer.getArray());
                short batteryVoltage = 0;
                short batteryVoltageGG = 0;
                short batteryVoltageINA = 0;
                short batteryVoltageUNREG = 0;
                if(EPSAlive){

                    if(EPSContainer.getBatteryGGStatus()){
                        batteryVoltageGG = EPSContainer.getBatteryGGVoltage();
                    }
                    if(EPSContainer.getBatteryINAStatus()){
                        batteryVoltageINA = EPSContainer.getBatteryINAVoltage();
                    }
                    if(EPSContainer.getUnregulatedINAStatus()){
                        batteryVoltageUNREG = EPSContainer.getUnregulatedINAVoltage();
                    }


                    if(!(EPSContainer.getBatteryGGStatus() ||  EPSContainer.getBatteryINAStatus() || EPSContainer.getUnregulatedINAStatus())){
                        //all sensors are dead...
                        batteryVoltage = DEPLOYMENT_VOLTAGE + 1;
                    }

                }else{
                    //EPS is dead...
                    batteryVoltage = SAFE_VOLTAGE + 1;
                    Console::log("OPERATIONAL: - SAFE - current totalTime: %d s | - EPS DEAD -", correctedUptime, batteryVoltage);

                }
                Console::log("OPERATIONAL: - SAFE - current totalTime: %d s | GG: %d | INA: %d | UNREG %d | BatVolt: %d", correctedUptime, batteryVoltageGG, batteryVoltageINA, batteryVoltageUNREG, batteryVoltage);
                if(batteryVoltage > SAFE_VOLTAGE || batteryVoltageINA > SAFE_VOLTAGE || batteryVoltageGG > SAFE_VOLTAGE || batteryVoltageUNREG > SAFE_VOLTAGE){
                    operationalState = 1;
                }
                break;
            case 1:
                Console::log("OPERATIONAL: - NOMINAL - current totalTime: %d s - BEACON ENABLED: %d", correctedUptime, (uint8_t )beaconEnabled);
                if(correctedUptime % BEACON_INTERVAL == 0 && (uint8_t ) beaconEnabled == 1){
                   Console::log("OPERATIONAL: BEACON TRANSMIT!", correctedUptime);
                   //get Pointer to target system

                   int telemetryIndex = 0;
                   uint8_t beaconCmdPayload[200];
                   beaconCmdPayload[0] = 9; //SEND PACKET COMMS
                   beaconCmdPayload[1] = 0; //id Number
                   beaconCmdPayload[2] = 8; //Destination Ground
                   beaconCmdPayload[3] = 0; //size (different per payload)
                   beaconCmdPayload[4] = Address::OBC; //source (OBC)
                   beaconCmdPayload[5] = ServiceNumber::TelemetryRequest;
                   beaconCmdPayload[6] = MsgType::Reply;
                   beaconCmdPayload[7] = 0; //OK No error

                   for(int j = 1; j < 5; j++){
                       beaconCmdPayload[8] = j;
                       beaconCmdPayload[3] = telemetrySizes[j-1] + 3;
                       memcpy(&beaconCmdPayload[9], &totalTelemetryContainer[telemetryIndex], telemetrySizes[j-1]);
                       busHandler->RequestReply(Address::COMMS, beaconCmdPayload[3] + 4, beaconCmdPayload, ServiceNumber::Radio, MsgType::Request, 50);
                       telemetryIndex += telemetrySizes[j-1];
                   }
                }
                break;
            default:
                operationalState = 0;
                break;
            }
            break;
        default:
            Console::log("UNKNOWN STATE! %d", currentState);
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
        if(rcvdMsg->getDataPayload()[0] == 0){ //Command = OK  //
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

void StateMachine::addOneSecWait(){
    waitTime += 1;
}

void StateMachine::overrideTotalUptime(unsigned long newUptime){
    totalUptime = newUptime;
}
