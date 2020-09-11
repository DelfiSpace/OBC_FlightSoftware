/*
 * TelemetryRequestService.h
 *
 *  Created on: 10 Sep 2020
 *      Author: casperbroekhuizen
 */

#ifndef TELEMETRYREQUESTSERVICE_H_
#define TELEMETRYREQUESTSERVICE_H_

#include "Service.h"
#include "LittleFS.h"
#include "SDCard.h"
#include "OBCTelemetryContainer.h"
#include "Telemetry/EPSTelemetryContainer.h"
#include "Telemetry/ADBTelemetryContainer.h"
#include "Telemetry/COMMSTelemetryContainer.h"



#define TELEMETRY_REQUEST_SERVICE               80
#define TELEMETRY_NO_ERROR                      0
#define TELEMETRY_UNKNOWN_CMD                   55
#define TELEMETRY_NOT_READY                     56
#define TELEMETRY_ERROR                         57

class TelemetryRequestService: public Service
{
protected:
    LittleFS *fs;
    SDCard *sdcard;

    LFSTask requestTask;
    lfs_file_t requestFile;
    uint8_t requestFileBuffer[512];
    const struct lfs_file_config requestFilecfg = {.buffer=requestFileBuffer};

    EPSTelemetryContainer EPSbuffer;
    uint8_t telemetrySizes[4] = {OBC_CONTAINER_SIZE, EPS_CONTAINER_SIZE, ADB_CONTAINER_SIZE, COMMS_CONTAINER_SIZE};
    uint8_t totalTelemetrySize = OBC_CONTAINER_SIZE + EPS_CONTAINER_SIZE + ADB_CONTAINER_SIZE + COMMS_CONTAINER_SIZE;
    uint8_t totalTelemetryContainer[OBC_CONTAINER_SIZE + EPS_CONTAINER_SIZE + ADB_CONTAINER_SIZE + COMMS_CONTAINER_SIZE];


public:
    TelemetryRequestService(LittleFS &fs_in, SDCard &sdcard_in);

    virtual bool process( DataMessage &command, DataMessage &workingBbuffer );

    void getTelemetry(unsigned long targetUptime);
};



#endif /* TELEMETRYREQUESTSERVICE_H_ */
