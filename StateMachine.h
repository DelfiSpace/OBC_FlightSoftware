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
#include "MB85RS.h"
#include "FRAMBackedVar.h"
#include "FRAMMap.h"
#include "LittleFS.h"


#define ACTIVATION_TIME             1*60
#define MAX_ADB_TEMPERATURE_WAIT    1*60
#define DEPLOYMENT_VOLTAGE          3200
#define LOG_INTERVAL                10

#define FRAM_OBC_STATE          FRAM_DEVICE_SPECIFIC_SPACE

enum OBCState {Activation = 0x00, Deploy = 0x01, Normal = 0x02 };

class StateMachine : public PeriodicTask
{
public:
    StateMachine(MB85RS &fram, BusMaster<PQ9Frame, PQ9Message> &busMaster, InternalCommandHandler<PQ9Frame, PQ9Message> &internalCmdHandler);
    void StateMachineRun();
    virtual bool notified();
    void init();

private:
    void processCOMMBuffer();
    bool PowerBusControl(bool Line1, bool Line2, bool Line3, bool Line4);
    bool getTelemetry(unsigned char destination, TelemetryContainer& targetBuffer);

    BusMaster<PQ9Frame, PQ9Message>* busHandler;
    InternalCommandHandler<PQ9Frame, PQ9Message>* intCmdHandler;
    MB85RS* fram;

    PQ9Message* rcvdMsg;
    int MsgsInQue = 0;
    bool runPeriodic = false; //safety flag to make sure the periodic function runs.

    FRAMBackedVar<uint8_t> currentState;

    LFSTask logTask;
    lfs_file_t logFile;
    uint8_t logFileBuffer[512];
    const struct lfs_file_config logFilecfg = {.buffer=logFileBuffer};

    unsigned long correctedUptime;

    EPSTelemetryContainer EPSContainer;
    ADBTelemetryContainer ADBContainer;


};

#endif /* STATEMACHINE_H_ */
