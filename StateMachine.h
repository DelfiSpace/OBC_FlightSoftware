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
#include "Telemetry/EPSTelemetryContainer.h"
#include "Telemetry/ADBTelemetryContainer.h"
#include "Telemetry/COMMSTelemetryContainer.h"
#include "OBCTelemetryContainer.h"
#include "HousekeepingService.h"
#include "MB85RS.h"
#include "FRAMBackedVar.h"
#include "FRAMMap.h"
#include "LittleFS.h"


#define ACTIVATION_TIME             15*60 //2*60//

#define ADB_DEPLOY_MAXTEMPWAIT      45*60 //1*60//
#define ADB_DEPLOY_MINTEMP          100   //mC
#define ADB_DEPLOY_TIMEOUT          10*60 //1*60//

#define DEPLOYMENT_VOLTAGE          3600
#define SAFE_VOLTAGE                3600
#define LOG_INTERVAL                20

#define BEACON_INTERVAL             5*60

#define FRAM_OBC_STATE              FRAM_DEVICE_SPECIFIC_SPACE
#define FRAM_CURRENT_DEPLOY_TIME    FRAM_OBC_STATE + 1
#define FRAM_BEACON_ENABLED         FRAM_CURRENT_DEPLOY_TIME + 4

enum OBCState {Activation = 0x00, Deploy = 0x01, Normal = 0x02 };

class StateMachine : public PeriodicTask
{
public:
    StateMachine(MB85RS &fram, BusMaster<PQ9Frame, PQ9Message> &busMaster, InternalCommandHandler<PQ9Frame, PQ9Message> &internalCmdHandler);
    void StateMachineRun();
    virtual bool notified();
    void init();

    FRAMBackedVar<uint8_t> currentState;
    uint8_t operationalState = 0;

    void addOneSecWait();
    void overrideTotalUptime(unsigned long newUptime);

    FRAMBackedVar<uint8_t> beaconEnabled;


private:
    void processCOMMBuffer();
    bool PowerBusControl(uint8_t bus, bool status);
    bool getTelemetry(unsigned char destination, uint8_t* targetContainer);

    BusMaster<PQ9Frame, PQ9Message>* busHandler;
    InternalCommandHandler<PQ9Frame, PQ9Message>* intCmdHandler;
    MB85RS* fram;

    PQ9Message* rcvdMsg;
    int MsgsInQue = 0;
    bool runPeriodic = false; //safety flag to make sure the periodic function runs.
    int waitTime = 0;

    int deployMode=0;
    int stoppedDeployTime=0;

    FRAMBackedVar<unsigned long> currentDeployTime;

    LFSTask logTask;
    lfs_file_t logFile;
    uint8_t logFileBuffer[512];
    const struct lfs_file_config logFilecfg = {.buffer=logFileBuffer};

    unsigned long correctedUptime;

    EPSTelemetryContainer EPSContainer;
    ADBTelemetryContainer ADBContainer;

    uint8_t telemetrySizes[4] = {OBC_CONTAINER_SIZE, EPS_CONTAINER_SIZE, ADB_CONTAINER_SIZE, COMMS_CONTAINER_SIZE};
    uint8_t totalTelemetrySize = OBC_CONTAINER_SIZE + EPS_CONTAINER_SIZE + ADB_CONTAINER_SIZE + COMMS_CONTAINER_SIZE;
    uint8_t totalTelemetryContainer[OBC_CONTAINER_SIZE + EPS_CONTAINER_SIZE + ADB_CONTAINER_SIZE + COMMS_CONTAINER_SIZE];




};

#endif /* STATEMACHINE_H_ */
